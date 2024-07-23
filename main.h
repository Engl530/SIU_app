#ifndef MAIN_H
#define MAIN_H

//���������
//=====================================================================
#define BUZZ_TIME_COEF      16//���������������� ����������� ��� ���������� ������ ������� �������
#define PROGRAMM_VERSION        (uint32_t)3130 //(2-������ �����, 1-������ ��������, 0,0-���� ��������� ��������� ������� �� ������ �������� �� ��������)
#define ADRESS_PROGRAMM_VERSION (uint16_t)(0x1030)
#define ADRESS_SERIAL_NUMBER    (uint16_t)(0x1040)
#define CGC_TIME_COEF_10000     (uint32_t)(10000)
#define CGC_TIME_COEF_2000      (uint32_t)(2000)
//=====================================================================

//��������� ��������
//=====================================================================



//=====================================================================

//��������� �������
//=====================================================================
void CLK_INIT(void);                                            //���������� ������� ��������� ������������
void GPIO_INIT(void);                                           //���������� ������� ��������� gpio
void UART_INIT(void);
void TIM1_INIT(void);                                 //���������� ������� ��������� ������� 1
void TIM3_INIT(void);                                 //���������� ������� ��������� ������� 2
void TIM4_INIT(void);                                 //���������� ������� ��������� ������� 4
void ADC_INIT(void);                                            //���������� ������� ��������� ���
void PERIPH_INIT(void);                                         //������� ������������� ��� ���������
void READ_DI_REGISTERS(void);                                   //������ ���������� ����� � ���������� � �������� ��� �������� � ������
void DELAY(uint16_t count);                                     //������� ��������

uint8_t LedControl(uint8_t number_of_led);                      //��������� ��� ���������� � �������� ������
void BuzzerControl(void);                 //������� �������� �������
void BrightnessControl(void);                 //������� �������� ������������� led
void SpeedLedControl(void);     //������� �������� �������� ������� �����
void RelayControl(void);     //������� �������� ���� �������������
void PrestartMode(void);        //������� ����������, ���������� ��� ���������� �� ������� � �������� �������.
void CentralGreenControl(void);//������� ���������� ����������� �����������
void ResetMcu(void);    //������� ������        
void WriteProgramVersion(void);        //���������� ������ �������� � ������(����� � ������ ���������)
uint8_t ADC_READ_VALUE(ADC_Channel_TypeDef ADC_CHANEL);         //���������� �������� ��� � ���������� ������
void ConstCount(void);//����������� ���������� ��� ������������ ���������
uint16_t ReadSerialNumber(void);        //������ ��������� ������
uint32_t ReadProgramVersion(void);        //������ ������ ��������
void VolumeVontrol(void);               //�������� ���������
void WriteSerialNumber(uint16_t number);        // ������ ��������� ������
//=====================================================================
#endif // MAIN_H