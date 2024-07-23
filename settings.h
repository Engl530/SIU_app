#ifndef SETTINGS_H
#define SETTINGS_H

//значения настроек производителя
//===========================================================================
#define CONTROL_SOURCE          0       /*источник управления 0-входы, 1-модбас, 2-смешанный*/
#define LED_WORK_COUNT          4       /*количество бегущих огней от 1 до 4*/
#define BRIGHTNESS              100     /*яркость от 0 до 100%*/
#define LED_SPEED               70     /*скорость бегущих огней от 0 до 100%*/
#define BUZZER_ON_TIME          500    /*время писка излучателя от 200 до 2000мс */
#define BUZZER_OFF_TIME         500    /*время паузы излучателя от 200 до 2000мс */
#define PRIORITY_INPUT1         3       /*приоритет входа от 1 до 3*/
#define PRIORITY_INPUT2         2       /*приоритет входа от 1 до 3*/
#define PRIORITY_INPUT3         1       /*приоритет входа от 1 до 3*/
#define INVERT_INPUT1           0       /*инверсия входа 1 - нет инверсии 0 - есть инверсия*/
#define INVERT_INPUT2           1       /*инверсия входа 1 - нет инверсии 0 - есть инверсия*/
#define INVERT_INPUT3           1       /*инверсия входа 1 - нет инверсии 0 - есть инверсия*/
#define INPUT1_BUZ              1       /*писк при срабатывании входа 1*/
#define INPUT2_BUZ              0       /*писк при срабатывании входа 2*/
#define INPUT3_BUZ              0       /*писк при срабатывании входа 3*/
#define INPUT1_RELAY            1       /*реле подтверждения при срабатывании входа 1*/
#define INPUT2_RELAY            0       /*реле подтверждения при срабатывании входа 2*/
#define INPUT3_RELAY            0       /*реле подтверждения при срабатывании входа 3*/
#define WORK_MODE_REG           10      /* текущий режим*/
#define VOLUME                  1       /* текущая громкость*/

//===========================================================================

//пороговые значения настроек
//===========================================================================
#define CONTROL_SOURCE_MAX_VALUE        2       /*источник управления 0-входы, 1-модбас, 2-смешанный*/
#define CONTROL_SOURCE_MIN_VALUE        0

#define LED_WORK_COUNT_MAX_VALUE        4       /*количество бегущих огней от 1 до 4*/
#define LED_WORK_COUNT_MIN_VALUE        1

#define BRIGHTNESS_MAX_VALUE            100     /*яркость от 0 до 100%*/
#define BRIGHTNESS_MIN_VALUE            0

#define LED_SPEED_MAX_VALUE             100     /*скорость бегущих огней от 0 до 100%*/
#define LED_SPEED_MIN_VALUE             0

#define BUZZER_ON_TIME_MAX_VALUE        2000    /*время писка излучателя от 200 до 2000мс */
#define BUZZER_ON_TIME_MIN_VALUE        200

#define BUZZER_OFF_TIME_MAX_VALUE       2000    /*время паузы излучателя от 200 до 2000мс */
#define BUZZER_OFF_TIME_MIN_VALUE       200

#define PRIORITY_INPUT_MAX_VALUE        3       /*приоритет входа от 1 до 3*/
#define PRIORITY_INPUT_MIN_VALUE        1
  
#define INVERT_INPUT_MAX_VALUE          1       /*инверсия входа 0 - нет инверсии 1 - есть инверсия*/
#define INVERT_INPUT_MIN_VALUE          0

#define INPUT_BUZ_MAX_VALUE             1       /*пищалка входа 0 - нет писка 1 - есть писк*/
#define INPUT_BUZ_MIN_VALUE             0

#define INPUT_RELAY_MAX_VALUE           1       /*реле подтверждения входа 0 - нет подтверждения 1 - есть подтверждения*/
#define INPUT_RELAY_MIN_VALUE           0

#define WORK_MODE_REG_MAX_VALUE         13       /*текущий режим 10-Normoal mode, 11-Red mode, 12-yellow mode, 13-green mode, 14-prestart mode, 15-settnig mode. Если источник управления входы, то нельхя редактировать.*/
#define WORK_MODE_REG_MIN_VALUE         10

#define VOLUME_MAX_VALUE                1       /*текущая громкость*/
#define VOLUME_MIN_VALUE                0
//===========================================================================


//адреса памяти в ЕЕПРОМ для хранения структур
//===========================================================================
#define ADRESS_SETTINGS_STRUCT          (uint16_t)(0x1000)
#define ADRESS_UART_SETTINGS_STRUCT     (uint16_t)(0x1020)

//===========================================================================

//индексы регистров модбас
//===========================================================================
#define INDEX_CONTROL_SOURCE    0
#define INDEX_LED_WORK_COUNT    1
#define INDEX_BRIGHTNESS        2
#define INDEX_LED_SPEED         3
#define INDEX_BUZZER_ON_TIME    4
#define INDEX_BUZZER_Off_TIME   5
#define INDEX_PRIORITY_INPUT1   6
#define INDEX_PRIORITY_INPUT2   7
#define INDEX_PRIORITY_INPUT3   8
#define INDEX_INVERT_INPUT1     9
#define INDEX_INVERT_INPUT2     10
#define INDEX_INVERT_INPUT3     11
#define INDEX_INPUT1_BUZ        12
#define INDEX_INPUT2_BUZ        13
#define INDEX_INPUT3_BUZ        14
#define INDEX_INPUT1_RELAY      15
#define INDEX_INPUT2_RELAY      16
#define INDEX_INPUT3_RELAY      17
#define INDEX_WORK_MODE_REG     18
#define INDEX_VOLUME            19
#define INDEX_SERIAL_NUM        20
//===========================================================================

//структуры
//===========================================================================
typedef struct {
  unsigned char control_source;   /*источник управления 0-входы, 1-модбас, 2-смешанный*/
  unsigned char led_work_count;   /*количество бегущих огней от 1 до 4*/
  unsigned char brightness;       /*яркость от 0 до 100%*/
  unsigned char led_speed;        /*скорость бегущих огней от 0 до 100%*/
  unsigned short buzzer_on_time;   /*время писка излучателя от 200 до 2000мс */
  unsigned short buzzer_off_time;  /*время паузы излучателя от 200 до 2000мс*/
  unsigned char priority_input1;  /*приоритет входа1*/
  unsigned char priority_input2;  /*приоритет входа2*/
  unsigned char priority_input3;  /*приоритет входа3*/
  unsigned char invert_input1;    /*инверсия входа1*/
  unsigned char invert_input2;    /*инверсия входа2*/
  unsigned char invert_input3;    /*инверсия входа3*/
  unsigned char input1_buz;     /*пищание входа1*/
  unsigned char input2_buz;     /*пищание входа2*/
  unsigned char input3_buz;     /*пищание входа3*/
  unsigned char input1_relay;     /*реле подтверждения входа1*/
  unsigned char input2_relay;     /*реле подтверждения  входа2*/
  unsigned char input3_relay;     /*реле подтверждения  входа3*/
  unsigned char work_mode_reg;      /*текущий режим 10-Normoal mode, 11-Red mode, 12-yellow mode, 13-green mode, 14-prestart mode, 15-settnig mode. Если источник управления входы, то нельхя редактировать.*/
  unsigned char volume;         /*текущий уроень громкости*/
} Settings_struct_type;

typedef struct {
  uint8_t own_id;                         /*свой ИД*/            
  uint32_t speed;                         /*скорость*/
  USART_WordLength_TypeDef word_lenth;    /*длина посылки*/
  USART_StopBits_TypeDef stop_bits;       /*стоп биты*/
  USART_Parity_TypeDef parity;            /*паритет*/
} Uart_settings_struct_type;

typedef enum {PRESTART_MODE, SETTING_MODE, RED_MODE, YLW_MODE, GRN_MODE, NORMAL_MODE}   work_mode_t_m;
typedef enum {BUTTON_CONTROL, MODBUS_CONTROL, BOTH_CONTROL}   mode_source_t_m;

typedef struct
{
  unsigned char buttons;
  unsigned char inverts;
  unsigned char work_mode;
} control_source_struct;
//===========================================================================

//функции
//===========================================================================
void SetManufactureSettings(void);
void SetUartDefaultSettings(void);
void WriteSettingsStructInToEEPROM(unsigned char *adress, Settings_struct_type *strct);
void WriteUartSettingsStructInToEEPROM(unsigned char *adress, Uart_settings_struct_type *strct);
void ReadSettingsStructFromEEPROM(unsigned char *adress, Settings_struct_type *strct);
void ReadUartSettingsStructFromEEPROM(unsigned char *adress, Uart_settings_struct_type *strct);
void CopySettingsStructInToModbus(unsigned short* reg);
void CopySettingsStructFromModbus(unsigned short* reg,  Settings_struct_type *strct);
void SetInputPriority(uint8_t index, uint16_t value);
void CheckWorkmodeRegister(uint16_t value);
uint8_t ONE_TV_ON(work_mode_t_m color);                        //выключает все транзисторы светодиодов и включает только один
work_mode_t_m DefineCurrentWorkMode(void);      //вычесляет текущий режим работы
mode_source_t_m DefineCurrentControlSource(void);
uint8_t CheckInput(uint8_t input);      //проверяет срабатывание входа
uint8_t CheckAndWrightParameters(uint8_t index, uint16_t value, unsigned short* reg);
uint8_t CheckSettingInEEPROM(unsigned char *adress, unsigned char *strct);      //проверяет данные по адресу, если там все нули то возвращает 1 если другое то 0;
//===========================================================================

#endif // SETTINGS_H