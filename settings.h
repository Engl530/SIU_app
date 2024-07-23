#ifndef SETTINGS_H
#define SETTINGS_H

//�������� �������� �������������
//===========================================================================
#define CONTROL_SOURCE          0       /*�������� ���������� 0-�����, 1-������, 2-���������*/
#define LED_WORK_COUNT          4       /*���������� ������� ����� �� 1 �� 4*/
#define BRIGHTNESS              100     /*������� �� 0 �� 100%*/
#define LED_SPEED               70     /*�������� ������� ����� �� 0 �� 100%*/
#define BUZZER_ON_TIME          500    /*����� ����� ���������� �� 200 �� 2000�� */
#define BUZZER_OFF_TIME         500    /*����� ����� ���������� �� 200 �� 2000�� */
#define PRIORITY_INPUT1         3       /*��������� ����� �� 1 �� 3*/
#define PRIORITY_INPUT2         2       /*��������� ����� �� 1 �� 3*/
#define PRIORITY_INPUT3         1       /*��������� ����� �� 1 �� 3*/
#define INVERT_INPUT1           0       /*�������� ����� 1 - ��� �������� 0 - ���� ��������*/
#define INVERT_INPUT2           1       /*�������� ����� 1 - ��� �������� 0 - ���� ��������*/
#define INVERT_INPUT3           1       /*�������� ����� 1 - ��� �������� 0 - ���� ��������*/
#define INPUT1_BUZ              1       /*���� ��� ������������ ����� 1*/
#define INPUT2_BUZ              0       /*���� ��� ������������ ����� 2*/
#define INPUT3_BUZ              0       /*���� ��� ������������ ����� 3*/
#define INPUT1_RELAY            1       /*���� ������������� ��� ������������ ����� 1*/
#define INPUT2_RELAY            0       /*���� ������������� ��� ������������ ����� 2*/
#define INPUT3_RELAY            0       /*���� ������������� ��� ������������ ����� 3*/
#define WORK_MODE_REG           10      /* ������� �����*/
#define VOLUME                  1       /* ������� ���������*/

//===========================================================================

//��������� �������� ��������
//===========================================================================
#define CONTROL_SOURCE_MAX_VALUE        2       /*�������� ���������� 0-�����, 1-������, 2-���������*/
#define CONTROL_SOURCE_MIN_VALUE        0

#define LED_WORK_COUNT_MAX_VALUE        4       /*���������� ������� ����� �� 1 �� 4*/
#define LED_WORK_COUNT_MIN_VALUE        1

#define BRIGHTNESS_MAX_VALUE            100     /*������� �� 0 �� 100%*/
#define BRIGHTNESS_MIN_VALUE            0

#define LED_SPEED_MAX_VALUE             100     /*�������� ������� ����� �� 0 �� 100%*/
#define LED_SPEED_MIN_VALUE             0

#define BUZZER_ON_TIME_MAX_VALUE        2000    /*����� ����� ���������� �� 200 �� 2000�� */
#define BUZZER_ON_TIME_MIN_VALUE        200

#define BUZZER_OFF_TIME_MAX_VALUE       2000    /*����� ����� ���������� �� 200 �� 2000�� */
#define BUZZER_OFF_TIME_MIN_VALUE       200

#define PRIORITY_INPUT_MAX_VALUE        3       /*��������� ����� �� 1 �� 3*/
#define PRIORITY_INPUT_MIN_VALUE        1
  
#define INVERT_INPUT_MAX_VALUE          1       /*�������� ����� 0 - ��� �������� 1 - ���� ��������*/
#define INVERT_INPUT_MIN_VALUE          0

#define INPUT_BUZ_MAX_VALUE             1       /*������� ����� 0 - ��� ����� 1 - ���� ����*/
#define INPUT_BUZ_MIN_VALUE             0

#define INPUT_RELAY_MAX_VALUE           1       /*���� ������������� ����� 0 - ��� ������������� 1 - ���� �������������*/
#define INPUT_RELAY_MIN_VALUE           0

#define WORK_MODE_REG_MAX_VALUE         13       /*������� ����� 10-Normoal mode, 11-Red mode, 12-yellow mode, 13-green mode, 14-prestart mode, 15-settnig mode. ���� �������� ���������� �����, �� ������ �������������.*/
#define WORK_MODE_REG_MIN_VALUE         10

#define VOLUME_MAX_VALUE                1       /*������� ���������*/
#define VOLUME_MIN_VALUE                0
//===========================================================================


//������ ������ � ������ ��� �������� ��������
//===========================================================================
#define ADRESS_SETTINGS_STRUCT          (uint16_t)(0x1000)
#define ADRESS_UART_SETTINGS_STRUCT     (uint16_t)(0x1020)

//===========================================================================

//������� ��������� ������
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

//���������
//===========================================================================
typedef struct {
  unsigned char control_source;   /*�������� ���������� 0-�����, 1-������, 2-���������*/
  unsigned char led_work_count;   /*���������� ������� ����� �� 1 �� 4*/
  unsigned char brightness;       /*������� �� 0 �� 100%*/
  unsigned char led_speed;        /*�������� ������� ����� �� 0 �� 100%*/
  unsigned short buzzer_on_time;   /*����� ����� ���������� �� 200 �� 2000�� */
  unsigned short buzzer_off_time;  /*����� ����� ���������� �� 200 �� 2000��*/
  unsigned char priority_input1;  /*��������� �����1*/
  unsigned char priority_input2;  /*��������� �����2*/
  unsigned char priority_input3;  /*��������� �����3*/
  unsigned char invert_input1;    /*�������� �����1*/
  unsigned char invert_input2;    /*�������� �����2*/
  unsigned char invert_input3;    /*�������� �����3*/
  unsigned char input1_buz;     /*������� �����1*/
  unsigned char input2_buz;     /*������� �����2*/
  unsigned char input3_buz;     /*������� �����3*/
  unsigned char input1_relay;     /*���� ������������� �����1*/
  unsigned char input2_relay;     /*���� �������������  �����2*/
  unsigned char input3_relay;     /*���� �������������  �����3*/
  unsigned char work_mode_reg;      /*������� ����� 10-Normoal mode, 11-Red mode, 12-yellow mode, 13-green mode, 14-prestart mode, 15-settnig mode. ���� �������� ���������� �����, �� ������ �������������.*/
  unsigned char volume;         /*������� ������ ���������*/
} Settings_struct_type;

typedef struct {
  uint8_t own_id;                         /*���� ��*/            
  uint32_t speed;                         /*��������*/
  USART_WordLength_TypeDef word_lenth;    /*����� �������*/
  USART_StopBits_TypeDef stop_bits;       /*���� ����*/
  USART_Parity_TypeDef parity;            /*�������*/
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

//�������
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
uint8_t ONE_TV_ON(work_mode_t_m color);                        //��������� ��� ����������� ����������� � �������� ������ ����
work_mode_t_m DefineCurrentWorkMode(void);      //��������� ������� ����� ������
mode_source_t_m DefineCurrentControlSource(void);
uint8_t CheckInput(uint8_t input);      //��������� ������������ �����
uint8_t CheckAndWrightParameters(uint8_t index, uint16_t value, unsigned short* reg);
uint8_t CheckSettingInEEPROM(unsigned char *adress, unsigned char *strct);      //��������� ������ �� ������, ���� ��� ��� ���� �� ���������� 1 ���� ������ �� 0;
//===========================================================================

#endif // SETTINGS_H