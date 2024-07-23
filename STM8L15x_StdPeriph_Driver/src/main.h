

//определяем номера пинов подключенных по схеме
#define VT_YLW_PIN      GPIO_Pin_6
#define VT_GRN_PIN      GPIO_Pin_5        
#define VT_RED_PIN      GPIO_Pin_4
#define VT_GEN_PIN      GPIO_Pin_0

#define LED_GR_PIN      GPIO_Pin_6
#define LED1_PIN        GPIO_Pin_4
#define LED2_PIN        GPIO_Pin_5
#define LED3_PIN        GPIO_Pin_4
#define LED4_PIN        GPIO_Pin_2
#define LED5_PIN        GPIO_Pin_1
#define LED6_PIN        GPIO_Pin_0
#define LED7_PIN        GPIO_Pin_5
#define LED8_PIN        GPIO_Pin_4
#define LED9_PIN        GPIO_Pin_3
#define LED10_PIN       GPIO_Pin_2
#define LED11_PIN       GPIO_Pin_6
#define LED12_PIN       GPIO_Pin_5

//определяем номера портов подключенных по схеме

#define VT_YLW_PORT     GPIOD
#define VT_GRN_PORT     GPIOB
#define VT_RED_PORT     GPIOB
#define VT_GEN_PORT     GPIOB

#define LED_GR_PORT     GPIOA
#define LED1_PORT       GPIOC
#define LED2_PORT       GPIOD
#define LED3_PORT       GPIOD
#define LED4_PORT       GPIOD
#define LED5_PORT       GPIOD
#define LED6_PORT       GPIOD
#define LED7_PORT       GPIOA
#define LED8_PORT       GPIOA
#define LED9_PORT       GPIOA
#define LED10_PORT      GPIOA
#define LED11_PORT      GPIOC
#define LED12_PORT      GPIOC

//определяем основные команды для портов
#define VT_YLW_ON       GPIO_WriteBit(VT_YLW_PORT, VT_YLW_PIN, SET)
#define VT_GRN_ON       GPIO_WriteBit(VT_GRN_PORT, VT_GRN_PIN, SET)
#define VT_RED_ON       GPIO_WriteBit(VT_RED_PORT, VT_RED_PIN, SET)
#define VT_GEN_ON       GPIO_WriteBit(VT_GEN_PORT, VT_GEN_PIN, SET)

#define LED_GR_ON       GPIO_WriteBit(LED_GR_PORT, LED_GR_PIN, SET)
#define LED1_ON         GPIO_WriteBit(LED1_PORT, LED1_PIN, RESET)
#define LED2_ON         GPIO_WriteBit(LED2_PORT, LED2_PIN, RESET)
#define LED3_ON         GPIO_WriteBit(LED3_PORT, LED3_PIN, RESET)
#define LED4_ON         GPIO_WriteBit(LED4_PORT, LED4_PIN, RESET)
#define LED5_ON         GPIO_WriteBit(LED5_PORT, LED5_PIN, RESET)
#define LED6_ON         GPIO_WriteBit(LED6_PORT, LED6_PIN, RESET)
#define LED7_ON         GPIO_WriteBit(LED7_PORT, LED7_PIN, RESET)
#define LED8_ON         GPIO_WriteBit(LED8_PORT, LED8_PIN, RESET)
#define LED9_ON         GPIO_WriteBit(LED9_PORT, LED9_PIN, RESET)
#define LED10_ON        GPIO_WriteBit(LED10_PORT, LED10_PIN, RESET)
#define LED11_ON        GPIO_WriteBit(LED11_PORT, LED11_PIN, RESET)
#define LED12_ON        GPIO_WriteBit(LED12_PORT, LED12_PIN, RESET)

#define VT_YLW_OFF      GPIO_WriteBit(VT_YLW_PORT, VT_YLW_PIN, RESET)
#define VT_GRN_OFF      GPIO_WriteBit(VT_GRN_PORT, VT_GRN_PIN, RESET)
#define VT_RED_OFF      GPIO_WriteBit(VT_RED_PORT, VT_RED_PIN, RESET)
#define VT_GEN_OFF      GPIO_WriteBit(VT_GEN_PORT, VT_GEN_PIN, RESET)

#define LED_GR_OFF      GPIO_WriteBit(LED_GR_PORT, LED_GR_PIN, RESET)
#define LED1_OFF        GPIO_WriteBit(LED1_PORT, LED1_PIN, SET)
#define LED2_OFF        GPIO_WriteBit(LED2_PORT, LED2_PIN, SET)
#define LED3_OFF        GPIO_WriteBit(LED3_PORT, LED3_PIN, SET)
#define LED4_OFF        GPIO_WriteBit(LED4_PORT, LED4_PIN, SET)
#define LED5_OFF        GPIO_WriteBit(LED5_PORT, LED5_PIN, SET)
#define LED6_OFF        GPIO_WriteBit(LED6_PORT, LED6_PIN, SET)
#define LED7_OFF        GPIO_WriteBit(LED7_PORT, LED7_PIN, SET)
#define LED8_OFF        GPIO_WriteBit(LED8_PORT, LED8_PIN, SET)
#define LED9_OFF        GPIO_WriteBit(LED9_PORT, LED9_PIN, SET)
#define LED10_OFF       GPIO_WriteBit(LED10_PORT, LED10_PIN, SET)
#define LED11_OFF       GPIO_WriteBit(LED11_PORT, LED11_PIN, SET)
#define LED12_OFF       GPIO_WriteBit(LED12_PORT, LED12_PIN, SET)

#define LED_GR_TGL      GPIO_ToggleBits(LED_GR_PORT, LED_GR_PIN)
#define VT_GEN_TGL      GPIO_ToggleBits(VT_GEN_PORT, VT_GEN_PIN)

#define RED_IN          ADC_Channel_17
#define YLW_IN          ADC_Channel_16
#define GRN_IN          ADC_Channel_15

