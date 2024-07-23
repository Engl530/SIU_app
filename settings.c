#include "settings.h"
#include "main.h"
#include <string.h>

Settings_struct_type Settings_struct;
Uart_settings_struct_type Uart_settings_struct;
control_source_struct last_control_struct;
control_source_struct new_control_struct;

extern unsigned short AO_Registers[];
extern unsigned short DI_Registers[];
extern work_mode_t_m current_work_mode;
extern mode_source_t_m current_control_source;

// записываем в структуру настройки производителя
void SetManufactureSettings(void)
{
  Settings_struct.control_source=CONTROL_SOURCE;
  Settings_struct.led_work_count=LED_WORK_COUNT;
  Settings_struct.brightness=BRIGHTNESS;
  Settings_struct.led_speed=LED_SPEED;
  Settings_struct.buzzer_on_time=BUZZER_ON_TIME;
  Settings_struct.buzzer_off_time=BUZZER_OFF_TIME;
  Settings_struct.priority_input1=PRIORITY_INPUT1;
  Settings_struct.priority_input2=PRIORITY_INPUT2;
  Settings_struct.priority_input3=PRIORITY_INPUT3;
  Settings_struct.invert_input1=INVERT_INPUT1;
  Settings_struct.invert_input2=INVERT_INPUT2;
  Settings_struct.invert_input3=INVERT_INPUT3;
  Settings_struct.input1_buz=INPUT1_BUZ;
  Settings_struct.input2_buz=INPUT2_BUZ;
  Settings_struct.input3_buz=INPUT3_BUZ;
  Settings_struct.input1_relay=INPUT1_RELAY;
  Settings_struct.input2_relay=INPUT2_RELAY;
  Settings_struct.input3_relay=INPUT3_RELAY;
  Settings_struct.work_mode_reg=WORK_MODE_REG;
  Settings_struct.volume=VOLUME;
}

// записываем структуру настрек в еепром
void WriteSettingsStructInToEEPROM(unsigned char *adress,  Settings_struct_type *strct)
{
  unsigned char* tpr_temp = (unsigned char*)strct;
  FLASH->DUKR = FLASH_RASS_KEY2; /*разблокируем память еепром*/
  FLASH->DUKR = FLASH_RASS_KEY1;
  for(unsigned char i=0; i<(sizeof(*strct)); i++) /*чистим еепром*/
  {
    *adress=0x00;
  }
  memcpy(adress, tpr_temp, sizeof(*strct));
  FLASH->IAPSR &= (uint8_t)0xF7; /*блокируем память еепром*/
}

// записываем структуру юарт настрек в еепром
void WriteUartSettingsStructInToEEPROM(unsigned char *adress, Uart_settings_struct_type *strct)
{
  unsigned char* tpr_temp = (unsigned char*)strct;
  FLASH->DUKR = FLASH_RASS_KEY2; /*разблокируем память еепром*/
  FLASH->DUKR = FLASH_RASS_KEY1;
  for(unsigned char i=0; i<(sizeof(*strct)); i++) /*чистим еепром*/
  {
    *adress=0x00;
  }
  memcpy(adress, tpr_temp, sizeof(*strct));
  FLASH->IAPSR &= (uint8_t)0xF7; /*блокируем память еепром*/
}

// записываем структуру настрек в регистры модбаса
void CopySettingsStructInToModbus(unsigned short* reg)
{
  reg[INDEX_CONTROL_SOURCE]=Settings_struct.control_source;
  reg[INDEX_LED_WORK_COUNT]=Settings_struct.led_work_count;
  reg[INDEX_BRIGHTNESS]=Settings_struct.brightness;
  reg[INDEX_LED_SPEED]=Settings_struct.led_speed;
  reg[INDEX_BUZZER_ON_TIME]=Settings_struct.buzzer_on_time;
  reg[INDEX_BUZZER_Off_TIME]=Settings_struct.buzzer_off_time;
  reg[INDEX_PRIORITY_INPUT1]=Settings_struct.priority_input1;
  reg[INDEX_PRIORITY_INPUT2]=Settings_struct.priority_input2;
  reg[INDEX_PRIORITY_INPUT3]=Settings_struct.priority_input3;
  reg[INDEX_INVERT_INPUT1]=Settings_struct.invert_input1;
  reg[INDEX_INVERT_INPUT2]=Settings_struct.invert_input2;
  reg[INDEX_INVERT_INPUT3]=Settings_struct.invert_input3;
  reg[INDEX_INPUT1_BUZ]=Settings_struct.input1_buz;
  reg[INDEX_INPUT2_BUZ]=Settings_struct.input2_buz;
  reg[INDEX_INPUT3_BUZ]=Settings_struct.input3_buz;
  reg[INDEX_INPUT1_RELAY]=Settings_struct.input1_relay;
  reg[INDEX_INPUT2_RELAY]=Settings_struct.input2_relay;
  reg[INDEX_INPUT3_RELAY]=Settings_struct.input3_relay;
  reg[INDEX_WORK_MODE_REG]=Settings_struct.work_mode_reg;
  reg[INDEX_VOLUME]=Settings_struct.volume;
  reg[INDEX_SERIAL_NUM]= (*(unsigned short*)ADRESS_SERIAL_NUMBER);
}

// записываем структуру настрек из регистров модбаса
void CopySettingsStructFromModbus(unsigned short* reg,  Settings_struct_type *strct)
{
  strct->control_source=reg[INDEX_CONTROL_SOURCE];
  strct->led_work_count=reg[INDEX_LED_WORK_COUNT];
  strct->brightness=reg[INDEX_BRIGHTNESS];
  strct->led_speed=reg[INDEX_LED_SPEED];
  strct->buzzer_on_time=reg[INDEX_BUZZER_ON_TIME];
  strct->buzzer_off_time=reg[INDEX_BUZZER_Off_TIME];
  strct->priority_input1=reg[INDEX_PRIORITY_INPUT1];
  strct->priority_input2=reg[INDEX_PRIORITY_INPUT2];
  strct->priority_input3=reg[INDEX_PRIORITY_INPUT3];
  strct->invert_input1=reg[INDEX_INVERT_INPUT1];
  strct->invert_input2=reg[INDEX_INVERT_INPUT2];
  strct->invert_input3=reg[INDEX_INVERT_INPUT3];
  strct->input1_buz=reg[INDEX_INPUT1_BUZ];
  strct->input2_buz=reg[INDEX_INPUT2_BUZ];
  strct->input3_buz=reg[INDEX_INPUT3_BUZ];
  strct->input1_relay=reg[INDEX_INPUT1_RELAY];
  strct->input2_relay=reg[INDEX_INPUT2_RELAY];
  strct->input3_relay=reg[INDEX_INPUT3_RELAY];  
  strct->work_mode_reg=reg[INDEX_WORK_MODE_REG];
  strct->volume=reg[INDEX_VOLUME];
}

// записываем в структуру юарт настройек производителя
void SetUartDefaultSettings(void)
{
  Uart_settings_struct.own_id=1;
  Uart_settings_struct.speed = 9600;
  Uart_settings_struct.parity = USART_Parity_Even;
  Uart_settings_struct.stop_bits = USART_StopBits_1;
  Uart_settings_struct.word_lenth = USART_WordLength_9b;
}

//Читаем структуру настроек из EEPROM
void ReadSettingsStructFromEEPROM(unsigned char *adress, Settings_struct_type *strct)
{
  unsigned char* tmp=(unsigned char*)strct;
  for(uint8_t i=0; i<sizeof(Settings_struct_type);i++)
  {
    *tmp=(unsigned char)*adress;
    tmp++;
    adress++;
  }
}

//Читаем структуру настроек UART из EEPROM
void ReadUartSettingsStructFromEEPROM(unsigned char *adress, Uart_settings_struct_type *strct)
{
  unsigned char* tmp=(unsigned char*)strct;
  for(uint8_t i=0; i<sizeof(Uart_settings_struct_type);i++)
  {
    *tmp=(unsigned char)*adress;
    tmp++;
    adress++;
  }
}

//Проверяет допустимые параметры по индексу( возвращает 0 если value в допустимых пределах)
uint8_t CheckAndWrightParameters(uint8_t index, uint16_t value, unsigned short* reg)
{
  SetInputPriority(index, value);
  switch (index)
  {
  case INDEX_CONTROL_SOURCE:    if(value>=CONTROL_SOURCE_MIN_VALUE &&   value<=CONTROL_SOURCE_MAX_VALUE)        {Settings_struct.control_source=value;  /*reg[INDEX_CONTROL_SOURCE]=value;*/}       else return 1; break;
  case INDEX_LED_WORK_COUNT:    if(value>=LED_WORK_COUNT_MIN_VALUE &&   value<=LED_WORK_COUNT_MAX_VALUE)        {Settings_struct.led_work_count=value;  /*reg[INDEX_LED_WORK_COUNT]=value;*/}       else return 1; break;
  case INDEX_BRIGHTNESS:        if(value>=BRIGHTNESS_MIN_VALUE &&       value<=BRIGHTNESS_MAX_VALUE)            {Settings_struct.brightness=value;      /*reg[INDEX_BRIGHTNESS]=value;*/}           else return 1; break;
  case INDEX_LED_SPEED:         if(value>=LED_SPEED_MIN_VALUE &&        value<=LED_SPEED_MAX_VALUE)             {Settings_struct.led_speed=value;       /*reg[INDEX_LED_SPEED]=value;*/}            else return 1; break;
  case INDEX_BUZZER_ON_TIME:    if(value>=BUZZER_ON_TIME_MIN_VALUE &&   value<=BUZZER_ON_TIME_MAX_VALUE)        {Settings_struct.buzzer_on_time=value;  /*reg[INDEX_BUZZER_ON_TIME]=value;*/}       else return 1; break;
  case INDEX_BUZZER_Off_TIME:   if(value>=BUZZER_OFF_TIME_MIN_VALUE &&  value<=BUZZER_OFF_TIME_MAX_VALUE)       {Settings_struct.buzzer_off_time=value; /*reg[INDEX_BUZZER_Off_TIME]=value;*/}      else return 1; break;
  case INDEX_PRIORITY_INPUT1:   if(value>=PRIORITY_INPUT_MIN_VALUE &&   value<=PRIORITY_INPUT_MAX_VALUE)        {Settings_struct.priority_input1=value; reg[INDEX_PRIORITY_INPUT1]=Settings_struct.priority_input1; reg[INDEX_PRIORITY_INPUT2]=Settings_struct.priority_input2; reg[INDEX_PRIORITY_INPUT3]=Settings_struct.priority_input3;}       else return 1; break;
  case INDEX_PRIORITY_INPUT2:   if(value>=PRIORITY_INPUT_MIN_VALUE &&   value<=PRIORITY_INPUT_MAX_VALUE)        {Settings_struct.priority_input2=value; reg[INDEX_PRIORITY_INPUT1]=Settings_struct.priority_input1; reg[INDEX_PRIORITY_INPUT2]=Settings_struct.priority_input2; reg[INDEX_PRIORITY_INPUT3]=Settings_struct.priority_input3;}       else return 1; break;
  case INDEX_PRIORITY_INPUT3:   if(value>=PRIORITY_INPUT_MIN_VALUE &&   value<=PRIORITY_INPUT_MAX_VALUE)        {Settings_struct.priority_input3=value; reg[INDEX_PRIORITY_INPUT1]=Settings_struct.priority_input1; reg[INDEX_PRIORITY_INPUT2]=Settings_struct.priority_input2; reg[INDEX_PRIORITY_INPUT3]=Settings_struct.priority_input3;}       else return 1; break;
  case INDEX_INVERT_INPUT1:     if(value>=INVERT_INPUT_MIN_VALUE &&     value<=INVERT_INPUT_MAX_VALUE)          {Settings_struct.invert_input1=value;   /*reg[INDEX_INVERT_INPUT1]=value;*/}        else return 1; break;
  case INDEX_INVERT_INPUT2:     if(value>=INVERT_INPUT_MIN_VALUE &&     value<=INVERT_INPUT_MAX_VALUE)          {Settings_struct.invert_input2=value;   /*reg[INDEX_INVERT_INPUT2]=value;*/}        else return 1; break;
  case INDEX_INVERT_INPUT3:     if(value>=INVERT_INPUT_MIN_VALUE &&     value<=INVERT_INPUT_MAX_VALUE)          {Settings_struct.invert_input3=value;   /*reg[INDEX_INVERT_INPUT3]=value;*/}        else return 1; break;
  case INDEX_INPUT1_BUZ:        if(value>=INPUT_BUZ_MIN_VALUE &&        value<=INPUT_BUZ_MAX_VALUE)             {Settings_struct.input1_buz=value;      /*reg[INDEX_INPUT1_BUZ]=value;*/}           else return 1; break;
  case INDEX_INPUT2_BUZ:        if(value>=INPUT_BUZ_MIN_VALUE &&        value<=INPUT_BUZ_MAX_VALUE)             {Settings_struct.input2_buz=value;      /*reg[INDEX_INPUT2_BUZ]=value;*/}           else return 1; break;
  case INDEX_INPUT3_BUZ:        if(value>=INPUT_BUZ_MIN_VALUE &&        value<=INPUT_BUZ_MAX_VALUE)             {Settings_struct.input3_buz=value;      /*reg[INDEX_INPUT3_BUZ]=value;*/}           else return 1; break;
  case INDEX_INPUT1_RELAY:      if(value>=INPUT_RELAY_MIN_VALUE &&      value<=INPUT_RELAY_MAX_VALUE)           {Settings_struct.input1_relay=value;      /*reg[INDEX_INPUT1_BUZ]=value;*/}           else return 1; break;
  case INDEX_INPUT2_RELAY:      if(value>=INPUT_RELAY_MIN_VALUE &&      value<=INPUT_RELAY_MAX_VALUE)           {Settings_struct.input2_relay=value;      /*reg[INDEX_INPUT2_BUZ]=value;*/}           else return 1; break;
  case INDEX_INPUT3_RELAY:      if(value>=INPUT_RELAY_MIN_VALUE &&      value<=INPUT_RELAY_MAX_VALUE)           {Settings_struct.input3_relay=value;      /*reg[INDEX_INPUT3_BUZ]=value;*/}           else return 1; break;
  case INDEX_WORK_MODE_REG:     if(value>=WORK_MODE_REG_MIN_VALUE &&    value<=WORK_MODE_REG_MAX_VALUE && (current_control_source==MODBUS_CONTROL || current_control_source==BOTH_CONTROL))         {Settings_struct.work_mode_reg=value;   /*reg[WORK_MODE_REG]=value;*/}              else return 1; break;
  case INDEX_VOLUME:            if(value>=VOLUME_MIN_VALUE &&           value<=VOLUME_MAX_VALUE && (current_control_source==MODBUS_CONTROL || current_control_source==BOTH_CONTROL))         {Settings_struct.volume=value;   /*reg[VOLUME]=value;*/}              else return 1; break;
  default: return 2;
  }
  return 0;
}

// Задает приоритет входу и делает приоритеты неповторяющимися
void SetInputPriority(uint8_t index, uint16_t value)
{
  switch (index)
  {
    case INDEX_PRIORITY_INPUT1:
      if (Settings_struct.priority_input1==value) break;
      else
      {
        if (Settings_struct.priority_input2==value) 
        {
          Settings_struct.priority_input2=Settings_struct.priority_input1;
          Settings_struct.priority_input1=value;
        }
        if (Settings_struct.priority_input3==value) 
        {
          Settings_struct.priority_input3=Settings_struct.priority_input1;
          Settings_struct.priority_input1=value;
        }
      }
      break;
    case INDEX_PRIORITY_INPUT2:
      if (Settings_struct.priority_input2==value) break;
      else
      {
        if (Settings_struct.priority_input1==value) 
        {
          Settings_struct.priority_input1=Settings_struct.priority_input2;
          Settings_struct.priority_input2=value;
        }
        if (Settings_struct.priority_input3==value) 
        {
          Settings_struct.priority_input3=Settings_struct.priority_input2;
          Settings_struct.priority_input2=value;
        }
      }
      break;
    case INDEX_PRIORITY_INPUT3:
      if (Settings_struct.priority_input3==value) break;
      else
      {
        if (Settings_struct.priority_input2==value) 
        {
          Settings_struct.priority_input2=Settings_struct.priority_input3;
          Settings_struct.priority_input3=value;
        }
        if (Settings_struct.priority_input1==value) 
        {
          Settings_struct.priority_input1=Settings_struct.priority_input3;
          Settings_struct.priority_input3=value;
        }
      }
      break;
  default: return;
  }
  return;
}

work_mode_t_m DefineCurrentWorkMode(void)
{
  if(current_work_mode==SETTING_MODE){AO_Registers[INDEX_WORK_MODE_REG]=15;return SETTING_MODE;}
  work_mode_t_m tmp_value=current_work_mode;
  new_control_struct.buttons=DI_Registers[0];
  new_control_struct.work_mode=AO_Registers[INDEX_WORK_MODE_REG];
  new_control_struct.inverts=(Settings_struct.invert_input1<<RED_DI_BIT)|(Settings_struct.invert_input2<<YELLOW_DI_BIT)|(Settings_struct.invert_input3<<GREEN_DI_BIT);
  switch(current_control_source)
  {
  case BUTTON_CONTROL:
    last_control_struct.buttons=new_control_struct.buttons;
    last_control_struct.work_mode=new_control_struct.work_mode;
    last_control_struct.inverts=new_control_struct.inverts;
    for (uint8_t i=3; i>0; i--)
    {
      if ((Settings_struct.priority_input1==i) && (CheckInput(1))) {tmp_value=RED_MODE; AO_Registers[INDEX_WORK_MODE_REG]=11; break;}
      if ((Settings_struct.priority_input2==i) && (CheckInput(2))) {tmp_value=YLW_MODE; AO_Registers[INDEX_WORK_MODE_REG]=12; break;}
      if ((Settings_struct.priority_input3==i) && (CheckInput(3))) {tmp_value=GRN_MODE; AO_Registers[INDEX_WORK_MODE_REG]=13; break;}
      else {tmp_value=NORMAL_MODE; AO_Registers[INDEX_WORK_MODE_REG]=10;}
    }
    break;
  case MODBUS_CONTROL: 
    last_control_struct.buttons=new_control_struct.buttons;
    last_control_struct.work_mode=new_control_struct.work_mode;
    last_control_struct.inverts=new_control_struct.inverts;
    if (new_control_struct.work_mode==10) return NORMAL_MODE;
    if (new_control_struct.work_mode==11) return RED_MODE;
    if (new_control_struct.work_mode==12) return YLW_MODE;
    if (new_control_struct.work_mode==13) return GRN_MODE;
    return NORMAL_MODE;
    break;
  case BOTH_CONTROL: 
    if((new_control_struct.buttons != last_control_struct.buttons) || (new_control_struct.inverts != last_control_struct.inverts))
    {
      for (uint8_t i=3; i>0; i--)
      {
        if ((Settings_struct.priority_input1==i) && (CheckInput(1))) {tmp_value=RED_MODE; AO_Registers[INDEX_WORK_MODE_REG]=11; break;}
        if ((Settings_struct.priority_input2==i) && (CheckInput(2))) {tmp_value=YLW_MODE; AO_Registers[INDEX_WORK_MODE_REG]=12; break;}
        if ((Settings_struct.priority_input3==i) && (CheckInput(3))) {tmp_value=GRN_MODE; AO_Registers[INDEX_WORK_MODE_REG]=13; break;}
        else {tmp_value=NORMAL_MODE; AO_Registers[INDEX_WORK_MODE_REG]=10;}
      }
      last_control_struct.buttons=new_control_struct.buttons;
      last_control_struct.work_mode=new_control_struct.work_mode;
      last_control_struct.inverts=new_control_struct.inverts;
    }
    if(new_control_struct.work_mode != last_control_struct.work_mode)
    {
      last_control_struct.buttons=new_control_struct.buttons;
      last_control_struct.work_mode=new_control_struct.work_mode;
      last_control_struct.inverts=new_control_struct.inverts;
      if (new_control_struct.work_mode==10) return NORMAL_MODE;
      if (new_control_struct.work_mode==11) return RED_MODE;
      if (new_control_struct.work_mode==12) return YLW_MODE;
      if (new_control_struct.work_mode==13) return GRN_MODE;
      return NORMAL_MODE;
    }
    //else tmp_value=NORMAL_MODE;
    break;
  }
  return tmp_value;
}

//определяет текущий источник управления
mode_source_t_m DefineCurrentControlSource(void)
{
  if (Settings_struct.control_source==0) return BUTTON_CONTROL;
  if (Settings_struct.control_source==1) return MODBUS_CONTROL;
  if (Settings_struct.control_source==2) return BOTH_CONTROL;
}

//выключает все транзисторы светодиодов и включает только один
uint8_t ONE_TV_ON(work_mode_t_m color)         
{
  switch (color)
  {
    case NORMAL_MODE:     VT_YLW_OFF; VT_RED_OFF; VT_GRN_OFF; break; 
    case GRN_MODE:     VT_GRN_ON; VT_YLW_OFF; VT_RED_OFF; break;          
    case RED_MODE:     VT_RED_ON; VT_YLW_OFF; VT_GRN_OFF; break;  
    case YLW_MODE:     VT_YLW_ON; VT_RED_OFF; VT_GRN_OFF; break;  
    default:   return 0;
  }
  return (uint8_t)color;
}

uint8_t CheckInput(uint8_t input)
{
  uint8_t tmp=0;
  switch(input)
  {
  case 1:
    tmp=DI_Registers[0];
    tmp &= (1<<RED_DI_BIT);
    if(Settings_struct.invert_input1==0) tmp ^= (1<<RED_DI_BIT);
    break;
  case 2:
    tmp=DI_Registers[0];
    tmp &= (1<<YELLOW_DI_BIT);
    if(Settings_struct.invert_input2==0) tmp ^= (1<<YELLOW_DI_BIT);
    break;
  case 3:
    tmp=DI_Registers[0];
    tmp &= (1<<GREEN_DI_BIT);
    if(Settings_struct.invert_input3==0) tmp ^= (1<<GREEN_DI_BIT);
    break;
  default: tmp=0;
  }    
  return tmp;   /*tmp=0 вход не активирован, tmp=1 вход активирован*/
}

//проверяет данные по адресу, если там все нули то возвращает 1 если другое то 0
uint8_t CheckSettingInEEPROM(unsigned char *adress, unsigned char *strct)
{
  uint8_t tmp=0;
  //unsigned char* tmp=(unsigned char*)strct;
  for(uint8_t i=0; i<sizeof(strct);i++)
  {
    tmp=(unsigned char)*adress;
    if(tmp!=0) return 0;
    adress++;
  }
  return 1;
}

//----