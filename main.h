#ifndef MAIN_H
#define MAIN_H

//константы
//=====================================================================
#define BUZZ_TIME_COEF      16//корректировочный коэффициент дл€ правильной работы времени пищалки
#define PROGRAMM_VERSION        (uint32_t)3130 //(2-верси€ платы, 1-верси€ прошивки, 0,0-если несильные изменени€ внесены то верси€ прошивки не мен€етс€)
#define ADRESS_PROGRAMM_VERSION (uint16_t)(0x1030)
#define ADRESS_SERIAL_NUMBER    (uint16_t)(0x1040)
#define CGC_TIME_COEF_10000     (uint32_t)(10000)
#define CGC_TIME_COEF_2000      (uint32_t)(2000)
//=====================================================================

//ќЅя¬Ћ≈Ќ»≈ —“–” “”–
//=====================================================================



//=====================================================================

//ќЅя¬Ћ≈Ќ»≈ ‘”Ќ ÷»…
//=====================================================================
void CLK_INIT(void);                                            //объ€вление функции настройки тактировани€
void GPIO_INIT(void);                                           //объ€вление функции настройки gpio
void UART_INIT(void);
void TIM1_INIT(void);                                 //объ€вление функции настройки таймера 1
void TIM3_INIT(void);                                 //объ€вление функции настройки таймера 2
void TIM4_INIT(void);                                 //объ€вление функции настройки таймера 4
void ADC_INIT(void);                                            //объ€вление функции настройки ацп
void PERIPH_INIT(void);                                         //функци€ инициализации все периферии
void READ_DI_REGISTERS(void);                                   //читает дискретные входы и записывает в регистры дл€ передачи в модбас
void DELAY(uint16_t count);                                     //функци€ задержки

uint8_t LedControl(uint8_t number_of_led);                      //выключает все светодиоды и включает нужные
void BuzzerControl(void);                 //функци€ контрол€ пищалки
void BrightnessControl(void);                 //функци€ контрол€ траанзисторов led
void SpeedLedControl(void);     //функци€ контрол€ скорости бегущих огней
void RelayControl(void);     //функци€ контрол€ реле подтверждени€
void PrestartMode(void);        //‘ункци€ предстарта, загор€ютс€ все светодиоды по очереди и работает пищалка.
void CentralGreenControl(void);//функци€ управлени€ центральным светодиодом
void ResetMcu(void);    //функци€ ресета        
void WriteProgramVersion(void);        //записывает версиб прошивки в еепром(адрес и верси€ константы)
uint8_t ADC_READ_VALUE(ADC_Channel_TypeDef ADC_CHANEL);         //возвращает значение ј÷ѕ с выбранного канала
void ConstCount(void);//высчитывает переменные дл€ переключени€ переферии
uint16_t ReadSerialNumber(void);        //чтение серийного номера
uint32_t ReadProgramVersion(void);        //чтение версии прошивки
void VolumeVontrol(void);               //контроль громкости
void WriteSerialNumber(uint16_t number);        // запись серийного номера
//=====================================================================
#endif // MAIN_H