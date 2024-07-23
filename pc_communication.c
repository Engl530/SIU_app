#include "pc_communication.h"
#include "settings.h"
#include "main.h"
extern unsigned int Crc16(unsigned char* pData,unsigned char len);//функци€ возвращает crc16

extern Settings_struct_type Settings_struct;
extern Uart_settings_struct_type Uart_settings_struct;
extern uint8_t reset_flag;
extern uint32_t ReadProgramVersion(void);

const uint8_t magic_word[]="SIU_LED_V2_START_PROGRAMMING";      /*приемна€ посылка дл€ перехода в режим настроек*/

extern uint16_t ReadSerialNumber(void);

//провер€ет буфер с кодовым словом. ¬озвращает 1 если совпало, если нет то 0
uint8_t CheckRxBufWithMW(uint8_t* buf)
{
  for(uint8_t i=0; i<(sizeof(magic_word)-1); i++)
  {
    if(buf[i]!=magic_word[i]) return 0;
  }
  return 1;
}


uint8_t AnswerStartCommunication_19(uint8_t* buf)
{
  uint16_t tmp=0;
  buf[0]=COMMAND_CONNECTION;
  buf[1]=~COMMAND_CONNECTION;
  tmp=ReadSerialNumber();
  buf[2]=tmp>>8;
  buf[3]=tmp;
  TIM1->CNTRH=0; TIM1->CNTRL=0x00;
  return 6; /*4 элемента + 2  байта црц*/
  
}

uint8_t CheckPCmessage(uint8_t* buf, uint8_t len)
{
  uint16_t tmp1=0;
  uint16_t tmp2=0;
  uint8_t tmp3=0;
  tmp1=ReadSerialNumber();
  tmp2=Crc16(buf,len-2);
  //проверка серийника, если не верно то выходим
  if(tmp1!=((buf[2]<<8)+(buf[3]))) return 0 ;
  //проверка црц, если не верно то выходим
  if(tmp2!=((buf[len-1]<<8)+buf[len-2])) return 0;
  tmp3=~buf[1];
  if(buf[0]!=tmp3) return 0;
  //если пришли сюда значит посылка верна  она нам
  switch(buf[0])
  {
  case COMMAND_CONNECTION:              return AnswerStartCommunication_19(buf); break;
  case COMMAND_READ_ALL_SETTINGS:       return ReadAndSendAllSettings(buf); break;
  case COMMAND_WRITE_WORK_SETTINGS:     return AnswerWriteWorkSetting(buf); break;
  case COMMAND_WRITE_UART_SETTINGS:     return AnswerWriteUartSetting(buf); break;
  case COMMAND_WRITE_SERIAL_NUMBER:     return AnswerWriteSerialNumber(buf); break;
  case COMMAND_RESET_MCU:               return AnswerRsetMCU(); break;    
  default: return 0; break;
  }
}

uint8_t ReadAndSendAllSettings(uint8_t* buf)
{
  uint8_t* tmp=(uint8_t*)ADRESS_SETTINGS_STRUCT;
  uint8_t i=4; //начинаем писать в буфер с четвертого байта
  for(uint8_t k=0; k<sizeof(Settings_struct); k++)
  {
    buf[i++]= *tmp++;
  }
  tmp=(uint8_t*)ADRESS_UART_SETTINGS_STRUCT;
  for(uint8_t k=0; k<sizeof(Uart_settings_struct); k++)
  {
    buf[i++]= *tmp++;
  }
  tmp=(uint8_t*)ADRESS_PROGRAMM_VERSION;
  for(uint8_t k=0; k<4; k++)
  {
    buf[i++]= *tmp++;
  }
  i+=2;
  return i;
}

uint8_t AnswerRsetMCU()
{
  reset_flag=1;
  return 6; //что получили то и возвращаем
}

uint8_t AnswerWriteWorkSetting(uint8_t* buf)
{
  uint8_t* tmp=(uint8_t*)ADRESS_SETTINGS_STRUCT;
  uint8_t i=4; //начинаем писать в буфер с четвертого байта
  FLASH->DUKR = FLASH_RASS_KEY2; /*разблокируем пам€ть еепром*/
  FLASH->DUKR = FLASH_RASS_KEY1;
  for(uint8_t k=0; k<sizeof(Settings_struct); k++)
  {
    *tmp++=buf[i++];
  }
  FLASH->IAPSR &= (uint8_t)0xF7; /*блокируем пам€ть еепром*/
  return i+=2;
}

uint8_t AnswerWriteUartSetting(uint8_t* buf)
{
  uint8_t* tmp=(uint8_t*)ADRESS_UART_SETTINGS_STRUCT;
  uint8_t i=4; //начинаем писать в буфер с четвертого байта
  FLASH->DUKR = FLASH_RASS_KEY2; /*разблокируем пам€ть еепром*/
  FLASH->DUKR = FLASH_RASS_KEY1;
  for(uint8_t k=0; k<sizeof(Uart_settings_struct); k++)
  {
    *tmp++=buf[i++];
  }
  FLASH->IAPSR &= (uint8_t)0xF7; /*блокируем пам€ть еепром*/
  return i+=2;
}

uint8_t AnswerWriteSerialNumber(uint8_t* buf)
{
  uint8_t* tmp=(uint8_t*)ADRESS_SERIAL_NUMBER;
  uint8_t i=4; //начинаем писать в буфер с четвертого байта
  FLASH->DUKR = FLASH_RASS_KEY2; /*разблокируем пам€ть еепром*/
  FLASH->DUKR = FLASH_RASS_KEY1;
  for(uint8_t k=0; k<sizeof(uint16_t); k++)
  {
    *tmp++=buf[i++];
  }
  FLASH->IAPSR &= (uint8_t)0xF7; /*блокируем пам€ть еепром*/
  return i+=2;
}

//-----