#include "modbus.h"
#include "main.h"
#include "settings.h"
#include "pc_communication.h"
//#include "pc_communication.h"

//ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
//=====================================================================
extern Modbus_struct MODBUS;
extern unsigned short AO_Registers[];
extern unsigned short DI_Registers[];
extern Settings_struct_type Settings_struct;
extern Uart_settings_struct_type Uart_settings_struct;
extern unsigned char Own_ID;

work_mode_t_m current_work_mode=PRESTART_MODE;
mode_source_t_m current_control_source=BUTTON_CONTROL;
uint8_t flag_changed_work_mode=0;
uint16_t serial_number=0;

uint8_t buzz_vol=0;

//переменные высчитываются в основном цикле, а используются в прерывании
uint32_t buz_count_on=0;
uint32_t buz_count_off=0;
uint32_t slc_count=0;

uint8_t reset_flag=0; //флаг необходимости сделать ресет
//=====================================================================



//=====================================================================

//=====================================================================
int main( void )
{

  WriteProgramVersion();
  serial_number=ReadSerialNumber();
  //проверяем, если записаны все нули то устанавливаем настройки по от производителя
  if (CheckSettingInEEPROM((unsigned char*)(ADRESS_SETTINGS_STRUCT), (unsigned char*)&Settings_struct)) {SetManufactureSettings();WriteSettingsStructInToEEPROM((unsigned char*)(ADRESS_SETTINGS_STRUCT), &Settings_struct);}
  else ReadSettingsStructFromEEPROM((unsigned char*)(ADRESS_SETTINGS_STRUCT), &Settings_struct);
  //проверяем, если записаны все нули то устанавливаем настройки по от производителя
  if (CheckSettingInEEPROM((unsigned char*)(ADRESS_UART_SETTINGS_STRUCT), (unsigned char*)&Uart_settings_struct)) {SetUartDefaultSettings();WriteUartSettingsStructInToEEPROM((unsigned char*)(ADRESS_UART_SETTINGS_STRUCT), &Uart_settings_struct);}
  else ReadUartSettingsStructFromEEPROM((unsigned char*)(ADRESS_UART_SETTINGS_STRUCT), &Uart_settings_struct);
  current_work_mode=PRESTART_MODE;
  PERIPH_INIT();
  CopySettingsStructInToModbus(AO_Registers);
  ADC_Cmd(ADC1, ENABLE);
  ONE_TV_ON(NORMAL_MODE);
  PrestartMode();
  TIM3_Cmd(ENABLE);
  TIM4_Cmd(ENABLE);
  LedControl(0);
  Own_ID=Uart_settings_struct.own_id;
  IWDG_ReloadCounter();
  
 

  while(1)
  {
    //=========================================================================== 
    current_work_mode=DefineCurrentWorkMode();
    current_control_source=DefineCurrentControlSource();
    ConstCount();
    //===========================================================================    
    ModbusTaskLoop();
    //=========================================================================== 
    READ_DI_REGISTERS();
    //===========================================================================  
    if(reset_flag) ResetMcu();
    VolumeVontrol();
    //=========================================================================== 
    IWDG_ReloadCounter();
    if(Uart_settings_struct.own_id >20) Uart_settings_struct.own_id=20;
    //===========================================================================  
  }
  return 0;
}

//инициализация переферии
//=====================================================================
void CLK_INIT(void)
{
  CLK_HSICmd(ENABLE);
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
}
void GPIO_INIT(void)
{
  //output_init
  GPIO_Init(GPIOA, LED_GR_PIN|LED7_PIN|LED8_PIN|LED9_PIN|LED10_PIN, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(GPIOB, VT_GRN_PIN|VT_RED_PIN|DCDC_EN_PIN|LED3_PIN, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(GPIOC, LED1_PIN|LED11_PIN|LED12_PIN, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(GPIOD, VT_YLW_PIN|LED2_PIN|RS_DE_PIN, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(GPIOE, VT_GEN_PIN|RELAY_PIN|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Fast);
  //ADC_init
  GPIO_Init(GPIOD, GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3, GPIO_Mode_In_FL_No_IT);
  //uart init
  GPIO_Init(UART_PORT, UART_RX_PIN, GPIO_Mode_In_PU_No_IT); //rx
  GPIO_Init(UART_PORT, UART_TX_PIN, GPIO_Mode_Out_PP_High_Fast); //tx
  GPIO_Init(RS_DE_PORT, RS_DE_PIN, GPIO_Mode_Out_PP_Low_Fast); //sw
  //input_init
  GPIO_Init(BUTTON_PORT, BUTTON_PIN, GPIO_Mode_In_PU_No_IT); //rx
  
}
void TIM1_INIT(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
  if (current_work_mode==PRESTART_MODE) { TIM1_TimeBaseInit(32767, TIM1_CounterMode_Up, 244, 0); TIM1_ITConfig(TIM1_IT_Update, DISABLE);}
  if (current_work_mode==SETTING_MODE) { TIM1_TimeBaseInit(32767, TIM1_CounterMode_Up, 2444, 0); TIM1->CNTRH=0; TIM1->CNTRL=0x00; TIM1_ITConfig(TIM1_IT_Update, ENABLE);}
  ITC_SetSoftwarePriority(TIM1_UPD_OVF_TRG_IRQn, ITC_PriorityLevel_1);
  TIM1_ClearFlag(TIM1_FLAG_Update);
}

void TIM3_INIT(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
  TIM3_TimeBaseInit(TIM3_Prescaler_64, TIM3_CounterMode_Up, 50);
  ITC_SetSoftwarePriority(TIM3_UPD_OVF_TRG_BRK_IRQn, ITC_PriorityLevel_2);
  TIM3_ITConfig(TIM3_IT_Update, ENABLE);  
  TIM3_ClearFlag(TIM3_FLAG_Update);
}

void TIM4_INIT(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
  TIM4_TimeBaseInit(TIM4_Prescaler_16, 50);
  ITC_SetSoftwarePriority(TIM4_UPD_OVF_TRG_IRQn, ITC_PriorityLevel_1);
  TIM4_ITConfig(TIM4_IT_Update, ENABLE);  
  TIM4_ClearFlag(TIM4_FLAG_Update);
}   
 
void UART_INIT(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
  USART_Init(USART1, Uart_settings_struct.speed, Uart_settings_struct.word_lenth, Uart_settings_struct.stop_bits, Uart_settings_struct.parity,  (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));
  switch (Uart_settings_struct.speed)
  {
    case 2400: MODBUS.delay=ANSWER_DELAY_2400; break;
    case 4800: MODBUS.delay=ANSWER_DELAY_4800; break;
    case 9600: MODBUS.delay=ANSWER_DELAY_9600; break;
    case 19200: MODBUS.delay=ANSWER_DELAY_19200; break;
    default: MODBUS.delay=ANSWER_DELAY_9600;
  }
  ITC_SetSoftwarePriority(USART1_RX_IRQn, ITC_PriorityLevel_2);
  ITC_SetSoftwarePriority(USART1_TX_IRQn, ITC_PriorityLevel_2);
  USART_Cmd(USART1, ENABLE);
  USART_ITConfig(USART1, (USART_IT_TypeDef)(USART_IT_IDLE  | USART_IT_RXNE), ENABLE);
  USART_ClearFlag(USART1,(USART_FLAG_TypeDef)(USART_FLAG_TC | USART_FLAG_RXNE));  
}

//объявление функции настройки ацп
void ADC_INIT(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
  ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_8Bit, ADC_Prescaler_1);
}

void PERIPH_INIT(void)
{
  CLK_INIT();
  GPIO_INIT();
  UART_INIT();
  IWDG_Enable();
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  IWDG_SetReload(0xFF); // LSI  = 38 кГц, ~1724.63 ms
  TIM1_INIT();
  TIM3_INIT();
  TIM4_INIT();
  enableInterrupts();
  ADC_INIT();
}
//=====================================================================

//ОПИСАНИЕ ФУНКЦИЙ
//=====================================================================

//выключает все светодиоды и включает только нужные
uint8_t LedControl(uint8_t number_of_led)       
{
  ALL_LED_OFF;
  switch (Settings_struct.led_work_count)
  {
    case 1: switch (number_of_led)
    {
      case 1:     LED1_ON; break; 
      case 2:     LED2_ON; break; 
      case 3:     LED3_ON; break; 
      case 4:     LED4_ON; break; 
      case 5:     LED5_ON; break; 
      case 6:     LED6_ON; break; 
      case 7:     LED7_ON; break; 
      case 8:     LED8_ON; break; 
      case 9:     LED9_ON; break;  
      case 10:    LED10_ON; break;  
      case 11:    LED11_ON; break;  
      case 12:    LED12_ON; break;  
      default:   return 0;
    } break;
    case 2: switch (number_of_led)
    {
      case 1:     LED1_ON; LED7_ON; break; 
      case 2:     LED2_ON; LED8_ON;break; 
      case 3:     LED3_ON; LED9_ON;break; 
      case 4:     LED4_ON; LED10_ON;break; 
      case 5:     LED5_ON; LED11_ON;break; 
      case 6:     LED6_ON; LED12_ON;break; 
      case 7:     LED7_ON; LED1_ON;break; 
      case 8:     LED8_ON; LED2_ON;break; 
      case 9:     LED9_ON; LED3_ON;break;  
      case 10:    LED10_ON; LED4_ON;break;  
      case 11:    LED11_ON; LED5_ON;break;  
      case 12:    LED12_ON; LED6_ON; break;  
      default:   return 0;
    } break;
    case 3: switch (number_of_led)
    {
      case 1:     LED1_ON; LED5_ON; LED9_ON;break; 
      case 2:     LED2_ON; LED6_ON; LED10_ON;break; 
      case 3:     LED3_ON; LED7_ON; LED11_ON;break; 
      case 4:     LED4_ON; LED8_ON; LED12_ON;break; 
      case 5:     LED5_ON; LED9_ON; LED1_ON;break; 
      case 6:     LED6_ON; LED10_ON; LED2_ON;break; 
      case 7:     LED7_ON; LED11_ON; LED3_ON;break; 
      case 8:     LED8_ON; LED12_ON; LED4_ON;break; 
      case 9:     LED9_ON; LED1_ON; LED5_ON;break;  
      case 10:    LED10_ON; LED2_ON; LED6_ON;break;  
      case 11:    LED11_ON; LED3_ON; LED7_ON;break;  
      case 12:    LED12_ON; LED4_ON; LED8_ON;break;  
      default:   return 0;
    } break;
    case 4: switch (number_of_led)
    {
      case 1:     LED1_ON; LED7_ON; LED4_ON; LED10_ON; break;          
      case 2:     LED2_ON; LED8_ON; LED5_ON; LED11_ON; break;  
      case 3:     LED3_ON; LED9_ON; LED6_ON; LED12_ON; break;  
      case 4:     LED4_ON; LED10_ON; LED7_ON; LED1_ON; break;  
      case 5:     LED5_ON; LED11_ON; LED8_ON; LED2_ON; break;  
      case 6:     LED6_ON; LED12_ON; LED9_ON; LED3_ON; break;  
      case 7:     LED7_ON; LED1_ON; LED10_ON; LED4_ON; break;  
      case 8:     LED8_ON; LED2_ON; LED11_ON; LED5_ON; break;  
      case 9:     LED9_ON; LED3_ON; LED12_ON; LED6_ON; break;  
      case 10:    LED10_ON; LED4_ON; LED1_ON; LED7_ON; break;  
      case 11:    LED11_ON; LED5_ON; LED2_ON; LED8_ON; break;  
      case 12:    LED12_ON; LED6_ON; LED3_ON; LED9_ON; break;  
      default:   return 0;
    } break;
  }
  return number_of_led;
}

//возвращает значение АЦП с выбранного канала
uint8_t ADC_READ_VALUE(ADC_Channel_TypeDef ADC_CHANEL)
{
  ADC_ChannelCmd(ADC1, ADC_CHANEL, ENABLE);
  ADC_SoftwareStartConv(ADC1);
  ADC_ChannelCmd(ADC1, ADC_CHANEL, DISABLE);
  return ADC_GetConversionValue(ADC1);
}

void DELAY(uint16_t count)
{
  uint16_t i=0;
  for(i=0; i<=count; i++)
  {}
}

void READ_DI_REGISTERS(void)
{
  uint8_t tmp=0;
  if (ADC_READ_VALUE(RED_IN) > ADC_IN_TRESHOLD) DI_Registers[0] |= (1<<RED_DI_BIT);
  else DI_Registers[0] &= ~(1<<RED_DI_BIT);
  if (ADC_READ_VALUE(GRN_IN) > ADC_IN_TRESHOLD) DI_Registers[0] |= (1<<GREEN_DI_BIT);
  else DI_Registers[0] &= ~(1<<GREEN_DI_BIT);
  if (ADC_READ_VALUE(YLW_IN) > ADC_IN_TRESHOLD) DI_Registers[0] |= (1<<YELLOW_DI_BIT);
  else DI_Registers[0] &= ~(1<<YELLOW_DI_BIT);
  tmp = (BUTTON_PORT->IDR &= BUTTON_PIN);
  if (tmp) DI_Registers[0] |= (1<<BUTTON_DI_BIT);
  else DI_Registers[0] &= ~(1<<BUTTON_DI_BIT);
  
}

void ResetMcu(void)
{
  IWDG_SetPrescaler(IWDG_Prescaler_4);
  while(1){};
}

void BuzzerControl(void)
{
  static uint32_t tmp=0;
  switch(current_work_mode)
  {
  case RED_MODE: 
    if(Settings_struct.input1_buz) 
    {
      tmp++; 
      //стандартная версия пищалки
      /*if(tmp==buz_count_on) {VT_GEN_OFF; BUZZ_VOL_MAX;}
      if(tmp>=buz_count_off) {tmp=0; VT_GEN_ON; if(buzz_vol) BUZZ_VOL_MAX; else BUZZ_VOL_MIN;}*/
      //версия с пищалкой Вадима
      if(tmp==buz_count_on) 
      {
        VT_GEN_OFF; 
        BUZZ_VOL_MAX;
      }
      if(tmp>=buz_count_off) 
      {
        tmp=0; 
        if(buzz_vol) VT_GEN_ON; 
        else BUZZ_VOL_MIN;
      }
    } 
    else {VT_GEN_OFF; BUZZ_VOL_MAX; return;}
    break;
  case YLW_MODE:
    if(Settings_struct.input2_buz) 
    {
      tmp++; 
      //стандартная версия пищалки
      /*if(tmp==buz_count_on) {VT_GEN_OFF; BUZZ_VOL_MAX;} 
      if(tmp>=buz_count_off) {tmp=0; VT_GEN_ON;if(buzz_vol) BUZZ_VOL_MAX; else BUZZ_VOL_MIN;}*/
      //версия с пищалкой Вадима
      if(tmp==buz_count_on) 
      {
        VT_GEN_OFF; 
        BUZZ_VOL_MAX;
      }
      if(tmp>=buz_count_off) 
      {
        tmp=0; 
        if(buzz_vol) VT_GEN_ON; 
        else BUZZ_VOL_MIN;
      }
    } 
    else {VT_GEN_OFF; BUZZ_VOL_MAX; return;}
    break;
  case GRN_MODE:
    if(Settings_struct.input3_buz) 
    {
      tmp++; 
      //стандартная версия пищалки
      /*if(tmp==buz_count_on) {VT_GEN_OFF; BUZZ_VOL_MAX;}
      if(tmp>=buz_count_off) {tmp=0; VT_GEN_ON;if(buzz_vol) BUZZ_VOL_MAX; else BUZZ_VOL_MIN;}*/
      //версия с пищалкой Вадима
      if(tmp==buz_count_on) 
      {
        VT_GEN_OFF; 
        BUZZ_VOL_MAX;
      }
      if(tmp>=buz_count_off) 
      {
        tmp=0; 
        if(buzz_vol) VT_GEN_ON; 
        else BUZZ_VOL_MIN;
      }
    } 
    else {VT_GEN_OFF; BUZZ_VOL_MAX; return;}
    break;
  default: VT_GEN_OFF; BUZZ_VOL_MAX; return;
    break;
  }
}

void BrightnessControl(void)
{
  static uint32_t tmp=0;
  tmp++;
  if((tmp>(Settings_struct.brightness))) ONE_TV_ON(NORMAL_MODE);
  else 
  {
    switch(current_work_mode)
    {
    case RED_MODE: ONE_TV_ON(RED_MODE);break;
    case YLW_MODE: ONE_TV_ON(YLW_MODE);break;
    case GRN_MODE: ONE_TV_ON(GRN_MODE);break;
    }
  }
  if(tmp>=100) tmp=0;
}

void CentralGreenControl(void)
{
  static uint32_t tmp=0;
  switch(current_work_mode)
  {
  case NORMAL_MODE: tmp++; if(tmp==(CGC_TIME_COEF_10000)) LED_GR_ON; if(tmp>=(2*CGC_TIME_COEF_10000)) {tmp=0;LED_GR_OFF;}
    break;
  case SETTING_MODE: tmp++; if(tmp==(CGC_TIME_COEF_2000)) LED_GR_ON; if(tmp>=(2*CGC_TIME_COEF_2000)) {tmp=0;LED_GR_OFF;}
    break;
  default: LED_GR_OFF;
    break;
  }
  return;

}

void SpeedLedControl(void)
{
  if((current_work_mode==NORMAL_MODE) || (current_work_mode==PRESTART_MODE) || (current_work_mode==SETTING_MODE)) {ALL_LED_OFF; return;}
  static uint32_t tmp1=0;
  static uint8_t tmp2=0;
  if(tmp1>=slc_count)//коэффициенты подобраны экспериментально
  {
    tmp2++;
    if (tmp2>=13) tmp2=1;
    if(Settings_struct.led_speed!=0) LedControl(tmp2);
    tmp1=0;
  }
  else tmp1++;
}

void PrestartMode(void)
{
  TIM1_Cmd(ENABLE);
  uint8_t tmp1=1;
  DELAY(10000);
  VT_GEN_ON;
  for (uint8_t i=0; i<3; i++)
  {
    TIM1->CNTRL=0x01;
    switch (i)
    {
    case 0: ONE_TV_ON(RED_MODE); break;
    case 1: ONE_TV_ON(YLW_MODE); break;
    case 2: ONE_TV_ON(GRN_MODE); break;
    }
    while(TIM1->CNTRL)
    {
      IWDG_ReloadCounter();
      LedControl(tmp1);
      tmp1++;
      if(tmp1>=13) tmp1=1; 
    }
    VT_GEN_TGL;
  }
  TIM1->CNTRL=0x00;
  TIM1->CNTRH=0x00;
  TIM1_Cmd(DISABLE);
  ONE_TV_ON(NORMAL_MODE);
  VT_GEN_OFF;
}

void ConstCount(void)
{
  buz_count_on=((uint32_t)Settings_struct.buzzer_on_time*BUZZ_TIME_COEF);
  buz_count_off=(((uint32_t)Settings_struct.buzzer_on_time*BUZZ_TIME_COEF)+((uint32_t)Settings_struct.buzzer_off_time*BUZZ_TIME_COEF));
  slc_count=(5030-(50*Settings_struct.led_speed));
}

void RelayControl(void)
{
  switch(current_work_mode)
  {
  case RED_MODE: if(Settings_struct.input1_relay) RELAY_ON; else RELAY_OFF; return; break;
  case YLW_MODE: if(Settings_struct.input2_relay) RELAY_ON; else RELAY_OFF; return; break;
  case GRN_MODE: if(Settings_struct.input3_relay) RELAY_ON; else RELAY_OFF; return; break;
  default:RELAY_OFF; return; break;
  }
}

void WriteProgramVersion(void)
{
  //записываем версию прошивки
  FLASH->DUKR = FLASH_RASS_KEY2; /*разблокируем память еепром*/
  FLASH->DUKR = FLASH_RASS_KEY1;
  *(uint32_t*)ADRESS_PROGRAMM_VERSION=PROGRAMM_VERSION;
  FLASH->IAPSR &= (uint8_t)0xF7; /*блокируем память еепром*/
}

uint16_t ReadSerialNumber(void)
{
  return *(uint16_t*)ADRESS_SERIAL_NUMBER;
}

void WriteSerialNumber(uint16_t number)
{
  FLASH->DUKR = FLASH_RASS_KEY2; /*разблокируем память еепром*/
  FLASH->DUKR = FLASH_RASS_KEY1;
  *(uint16_t*)ADRESS_SERIAL_NUMBER=number;
  FLASH->IAPSR &= (uint8_t)0xF7; /*блокируем память еепром*/
}

uint32_t ReadProgramVersion(void)
{
  return *(uint32_t*)ADRESS_PROGRAMM_VERSION;
}

void VolumeVontrol(void)
{
  static uint8_t previous_volume_di_state=0;
  static uint8_t previous_volume_ao_state=0;
  uint8_t tmp = 0;
  switch(current_control_source)
  {
  case BUTTON_CONTROL:
    previous_volume_di_state = DI_Registers[0] & (1<<BUTTON_DI_BIT);
    if (previous_volume_di_state) {Settings_struct.volume=1; buzz_vol=1; AO_Registers[INDEX_VOLUME]=1;}
    else {Settings_struct.volume=0; buzz_vol=0; AO_Registers[INDEX_VOLUME]=0;}
    break;
  case MODBUS_CONTROL:
    previous_volume_ao_state = AO_Registers[INDEX_VOLUME];
    if (previous_volume_ao_state==0) buzz_vol=0;
    else buzz_vol=1;
    break;
  case BOTH_CONTROL:
    if(previous_volume_di_state != (DI_Registers[0] & (1<<BUTTON_DI_BIT)))
    {
      previous_volume_di_state = (DI_Registers[0] & (1<<BUTTON_DI_BIT));
      previous_volume_ao_state = previous_volume_di_state;
      if (previous_volume_di_state) {Settings_struct.volume=1; buzz_vol=1; AO_Registers[INDEX_VOLUME]=1;}
      else {Settings_struct.volume=0; buzz_vol=0; AO_Registers[INDEX_VOLUME]=0; }
      
    }
    if(previous_volume_ao_state != AO_Registers[INDEX_VOLUME])
    {
      previous_volume_ao_state = AO_Registers[INDEX_VOLUME];
      //previous_volume_di_state = previous_volume_ao_state;
      if (previous_volume_ao_state==0) buzz_vol=0;
      else buzz_vol=1;
    }
    
    break;
  }
}
//=====================================================================

//инобработчики прерывания
//=====================================================================
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_IRQHandler, 23)
{
  if(current_work_mode==SETTING_MODE)
  {
    current_work_mode=NORMAL_MODE;
    TIM1_Cmd(DISABLE);
  }
  TIM1_ClearITPendingBit(TIM1_IT_Update);
}

INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_IRQHandler, 21)
{
  ModbusTimInterruptHandler();
  TIM3_ClearITPendingBit(TIM3_IT_Update);
}

INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler, 25)
{
  SpeedLedControl();
  BrightnessControl();
  BuzzerControl();
  CentralGreenControl();
  RelayControl();
  TIM4_ClearITPendingBit(TIM4_IT_Update);
}

INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler, 27)
{
  ModbusUartIntTxHandler();
  
}

INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler, 28)
{
   ModbusUartIntRxHandler();
  
}

//=====================================================================