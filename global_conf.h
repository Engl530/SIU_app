#include "stm8l15x.h"

//адреса памяти еепром для настроек
#define DEFAULT_PARAMETERS_ADRESS       (uint16_t)0x1000  
#define DEFAULT_UART_SETTINGS_ADRESS       (uint16_t)0x1020  

#define RED_DI_BIT              0
#define YELLOW_DI_BIT           1
#define GREEN_DI_BIT            2
#define BUTTON_DI_BIT           3
#define ADC_IN_TRESHOLD         150     //порог срабатывания входа

//определяем номера пинов подключенных по схеме
#define VT_YLW_PIN      GPIO_Pin_5
#define VT_GRN_PIN      GPIO_Pin_3        
#define VT_RED_PIN      GPIO_Pin_2
#define VT_GEN_PIN      GPIO_Pin_5
#define RELAY_PIN       GPIO_Pin_4
#define DCDC_EN_PIN     GPIO_Pin_6
#define BUTTON_PIN      GPIO_Pin_0

#define LED_GR_PIN      GPIO_Pin_7
#define LED1_PIN        GPIO_Pin_4
#define LED2_PIN        GPIO_Pin_4
#define LED3_PIN        GPIO_Pin_7
#define LED4_PIN        GPIO_Pin_2
#define LED5_PIN        GPIO_Pin_1
#define LED6_PIN        GPIO_Pin_0
#define LED7_PIN        GPIO_Pin_6
#define LED8_PIN        GPIO_Pin_5
#define LED9_PIN        GPIO_Pin_4
#define LED10_PIN       GPIO_Pin_3
#define LED11_PIN       GPIO_Pin_6
#define LED12_PIN       GPIO_Pin_5

#define RS_DE_PIN       GPIO_Pin_7
#define UART_RX_PIN     GPIO_Pin_2
#define UART_TX_PIN     GPIO_Pin_3

//определяем номера портов подключенных по схеме

#define VT_YLW_PORT     GPIOD
#define VT_GRN_PORT     GPIOB
#define VT_RED_PORT     GPIOB
#define VT_GEN_PORT     GPIOE
#define RELAY_PORT      GPIOE
#define DCDC_EN_PORT    GPIOB
#define BUTTON_PORT     GPIOC

#define LED_GR_PORT     GPIOA
#define LED1_PORT       GPIOC
#define LED2_PORT       GPIOD
#define LED3_PORT       GPIOB
#define LED4_PORT       GPIOE
#define LED5_PORT       GPIOE
#define LED6_PORT       GPIOE
#define LED7_PORT       GPIOA
#define LED8_PORT       GPIOA
#define LED9_PORT       GPIOA
#define LED10_PORT      GPIOA
#define LED11_PORT      GPIOC
#define LED12_PORT      GPIOC

#define RS_DE_PORT      GPIOD
#define UART_PORT       GPIOC

//определяем основные команды для портов
#define VT_YLW_ON       VT_YLW_PORT->ODR |= VT_YLW_PIN          //GPIO_WriteBit(VT_YLW_PORT, VT_YLW_PIN, SET)
#define VT_GRN_ON       VT_GRN_PORT->ODR |= VT_GRN_PIN          //GPIO_WriteBit(VT_GRN_PORT, VT_GRN_PIN, SET)
#define VT_RED_ON       VT_RED_PORT->ODR |= VT_RED_PIN          //GPIO_WriteBit(VT_RED_PORT, VT_RED_PIN, SET)
#define VT_GEN_ON       VT_GEN_PORT->ODR |= VT_GEN_PIN          //GPIO_WriteBit(VT_GEN_PORT, VT_GEN_PIN, SET)
#define RELAY_ON        RELAY_PORT->ODR |= RELAY_PIN            //GPIO_WriteBit(RELAY_PORT, RELAY_PIN, SET)
#define BUZZ_VOL_MIN    DCDC_EN_PORT->ODR |= DCDC_EN_PIN            //

#define LED_GR_ON       LED_GR_PORT->ODR |= LED_GR_PIN          //GPIO_WriteBit(LED_GR_PORT, LED_GR_PIN, SET)
#define LED1_ON         LED1_PORT->ODR &= ~LED1_PIN             //GPIO_WriteBit(LED1_PORT, LED1_PIN, RESET)
#define LED2_ON         LED2_PORT->ODR &= ~LED2_PIN             //GPIO_WriteBit(LED2_PORT, LED2_PIN, RESET)
#define LED3_ON         LED3_PORT->ODR &= ~LED3_PIN             //GPIO_WriteBit(LED3_PORT, LED3_PIN, RESET)
#define LED4_ON         LED4_PORT->ODR &= ~LED4_PIN             //GPIO_WriteBit(LED4_PORT, LED4_PIN, RESET)
#define LED5_ON         LED5_PORT->ODR &= ~LED5_PIN             //GPIO_WriteBit(LED5_PORT, LED5_PIN, RESET)
#define LED6_ON         LED6_PORT->ODR &= ~LED6_PIN             //GPIO_WriteBit(LED6_PORT, LED6_PIN, RESET)
#define LED7_ON         LED7_PORT->ODR &= ~LED7_PIN             //GPIO_WriteBit(LED7_PORT, LED7_PIN, RESET)
#define LED8_ON         LED8_PORT->ODR &= ~LED8_PIN             //GPIO_WriteBit(LED8_PORT, LED8_PIN, RESET)
#define LED9_ON         LED9_PORT->ODR &= ~LED9_PIN             //GPIO_WriteBit(LED9_PORT, LED9_PIN, RESET)
#define LED10_ON        LED10_PORT->ODR &= ~LED10_PIN           //GPIO_WriteBit(LED10_PORT, LED10_PIN, RESET)
#define LED11_ON        LED11_PORT->ODR &= ~LED11_PIN           //GPIO_WriteBit(LED11_PORT, LED11_PIN, RESET)
#define LED12_ON        LED12_PORT->ODR &= ~LED12_PIN           //GPIO_WriteBit(LED12_PORT, LED12_PIN, RESET)

#define VT_YLW_OFF      VT_YLW_PORT->ODR &= ~VT_YLW_PIN         //GPIO_WriteBit(VT_YLW_PORT, VT_YLW_PIN, RESET)
#define VT_GRN_OFF      VT_GRN_PORT->ODR &= ~VT_GRN_PIN         //GPIO_WriteBit(VT_GRN_PORT, VT_GRN_PIN, RESET)
#define VT_RED_OFF      VT_RED_PORT->ODR &= ~VT_RED_PIN         //GPIO_WriteBit(VT_RED_PORT, VT_RED_PIN, RESET)
#define VT_GEN_OFF      VT_GEN_PORT->ODR &= ~VT_GEN_PIN         //GPIO_WriteBit(VT_GEN_PORT, VT_GEN_PIN, RESET)
#define RELAY_OFF       RELAY_PORT->ODR &= ~RELAY_PIN           //GPIO_WriteBit(RELAY_PORT, RELAY_PIN, RESET)
#define BUZZ_VOL_MAX    DCDC_EN_PORT->ODR &= ~DCDC_EN_PIN            //

#define LED_GR_OFF      LED_GR_PORT->ODR &= ~LED_GR_PIN         //GPIO_WriteBit(LED_GR_PORT, LED_GR_PIN, RESET)
#define LED1_OFF        LED1_PORT->ODR |= LED1_PIN              //GPIO_WriteBit(LED1_PORT, LED1_PIN, SET)
#define LED2_OFF        LED2_PORT->ODR |= LED2_PIN              //GPIO_WriteBit(LED2_PORT, LED2_PIN, SET)
#define LED3_OFF        LED3_PORT->ODR |= LED3_PIN              //GPIO_WriteBit(LED3_PORT, LED3_PIN, SET)
#define LED4_OFF        LED4_PORT->ODR |= LED4_PIN              //GPIO_WriteBit(LED4_PORT, LED4_PIN, SET)
#define LED5_OFF        LED5_PORT->ODR |= LED5_PIN              //GPIO_WriteBit(LED5_PORT, LED5_PIN, SET)
#define LED6_OFF        LED6_PORT->ODR |= LED6_PIN              //GPIO_WriteBit(LED6_PORT, LED6_PIN, SET)
#define LED7_OFF        LED7_PORT->ODR |= LED7_PIN              //GPIO_WriteBit(LED7_PORT, LED7_PIN, SET)
#define LED8_OFF        LED8_PORT->ODR |= LED8_PIN              //GPIO_WriteBit(LED8_PORT, LED8_PIN, SET)
#define LED9_OFF        LED9_PORT->ODR |= LED9_PIN              //GPIO_WriteBit(LED9_PORT, LED9_PIN, SET)
#define LED10_OFF       LED10_PORT->ODR |= LED10_PIN            //GPIO_WriteBit(LED10_PORT, LED10_PIN, SET)
#define LED11_OFF       LED11_PORT->ODR |= LED11_PIN            //GPIO_WriteBit(LED11_PORT, LED11_PIN, SET)
#define LED12_OFF       LED12_PORT->ODR |= LED12_PIN            //GPIO_WriteBit(LED12_PORT, LED12_PIN, SET)
#define ALL_LED_OFF     LED1_OFF; LED2_OFF; LED3_OFF; LED4_OFF; LED5_OFF; LED6_OFF; LED7_OFF; LED8_OFF; LED9_OFF; LED10_OFF; LED11_OFF; LED12_OFF;

#define LED_GR_TGL      LED_GR_PORT->ODR ^=LED_GR_PIN           //GPIO_ToggleBits(LED_GR_PORT, LED_GR_PIN)
#define VT_GEN_TGL      VT_GEN_PORT->ODR ^=VT_GEN_PIN           //GPIO_ToggleBits(VT_GEN_PORT, VT_GEN_PIN)
#define RELAY_TGL       RELAY_PORT->ODR ^=RELAY_PIN           //GPIO_ToggleBits(VT_GEN_PORT, VT_GEN_PIN)

#define RED_IN          ADC_Channel_21
#define YLW_IN          ADC_Channel_20
#define GRN_IN          ADC_Channel_19

#define RS_DE_ON        RS_DE_PORT->ODR |= RS_DE_PIN            //GPIO_WriteBit(RS_DE_PORT, RS_DE_PIN, SET)
#define RS_DE_OFF       RS_DE_PORT->ODR &= ~RS_DE_PIN           //GPIO_WriteBit(RS_DE_PORT, RS_DE_PIN, RESET)


