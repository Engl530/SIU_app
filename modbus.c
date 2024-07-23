#include "modbus.h"
#include "settings.h"
#include "pc_communication.h"

//=========================================================================
// ����� �������� � ������� �����: 
// ���������� ����� �����
// ���������� ����� �������
// ������ ������ ���������� �������� ������
// � ����������� ���������� �������� ���������������� ������� �� ����� �����
// � ������� ���� �������� ������� �������� ������ ���������
//=========================================================================
//extern uint8_t CheckAndWrightParameters(uint8_t index, uint16_t value, unsigned short* reg); // ������� �� ����� ��������
//extern uint8_t CheckRxBufWithMW(uint8_t* buf);
//extern uint8_t AnswerStartCommunication_19(uint8_t* buf);

extern work_mode_t_m current_work_mode;
extern void TIM1_INIT(void);
//extern TIM1_Cmd();

//extern void SetInputPriority(uint8_t index, uint16_t value);
//����������� ������������ ���������
//=========================================================================
#define MODBUS_TIMER TIM3
#define MODBUS_UART USART1
//���������� �������� � ���������
//=========================================================================
#define DI_OBJECTS 0    //bool
//#define DO_OBJECTS 3  //bool
//#define AI_OBJECTS 0  //int
#define AO_OBJECTS 21    //int
//=========================================================================

//���������� ����������
unsigned char Own_ID=1; //����������� ����� ����������
Modbus_struct MODBUS;
#ifdef AI_OBJECTS
  unsigned short AI_Registers[AI_OBJECTS];
#endif

#ifdef AO_OBJECTS
unsigned short AO_Registers[AO_OBJECTS];
#endif

#ifdef DI_OBJECTS
#define DI_REGS      ((DI_OBJECTS/16)+1)
unsigned short DI_Registers[DI_REGS];
#endif

#ifdef DO_OBJECTS
#define DO_REGS      ((DO_OBJECTS/16)+1)
unsigned short DO_Registers[((DO_REGS/16)+1)]={0};
#endif

//������� �������� ��������� ���������
void ModbusCheckMsg(void)
{
  MODBUS_TIMER->CR1 &= ~0x01;    //��������� ������
  unsigned int tmp;
  if(current_work_mode==SETTING_MODE) {MODBUS.txlen=CheckPCmessage(MODBUS.buffer, MODBUS.rxcnt); if(MODBUS.txlen != 0) {tmp=Crc16(MODBUS.buffer,MODBUS.txlen-2);MODBUS.buffer[MODBUS.txlen-2]=tmp;MODBUS.buffer[MODBUS.txlen-1]=tmp>>8;MODBUS.txcnt=0;}}
  //���� ������ ���������� �����, �� ������������� SETTING_MODE � �������� ������ �� ��������� �� ����� ������ 
  if (CheckRxBufWithMW(MODBUS.buffer))
  {
    current_work_mode=SETTING_MODE;
    TIM1_INIT(); 
    TIM1_Cmd(ENABLE);
    MODBUS.txlen=AnswerStartCommunication_19(MODBUS.buffer);
    tmp=Crc16(MODBUS.buffer,MODBUS.txlen-2);
    MODBUS.buffer[MODBUS.txlen-2]=tmp;
    MODBUS.buffer[MODBUS.txlen-1]=tmp>>8;
    MODBUS.txcnt=0;
    current_work_mode=SETTING_MODE;
    
  }
  //���� ������� ����� ��������� �� ��������� ������� �� ������� ��������� � ��
  
  //recive and checking rx query
  if((MODBUS.buffer[0]!=0)&(MODBUS.rxcnt>5)& ((MODBUS.buffer[0]==Own_ID)|(MODBUS.buffer[0]==255)))
  {
    tmp=Crc16(MODBUS.buffer,MODBUS.rxcnt-2);
    if((MODBUS.buffer[MODBUS.rxcnt-2]==(tmp&0x00FF)) & (MODBUS.buffer[MODBUS.rxcnt-1]==(tmp>>8)))
    {
     //���� �� ���� ������ ������ ����� ��� � crc ������� - ���� ��������� ������������ �� �� ����� ������
      //choosing function
      switch(MODBUS.buffer[1])
      {
      case 1:   ModbusCreateMsg01();break;
      case 2:   ModbusCreateMsg02();break;
      case 3:   ModbusCreateMsg03();break;
      case 4:   ModbusCreateMsg04();break;
      case 5:   ModbusCreateMsg05();break;
      case 6:   ModbusCreateMsg06();break;
      case 16:   ModbusCreateMsg16();break;
      default: ModbusCreateMsgError(ERROR_01);//���� ��� �� ������ ������
      }
      //��������� CRC � ������� � �������
      //adding CRC16 to reply
      tmp=Crc16(MODBUS.buffer,MODBUS.txlen-2);
      MODBUS.buffer[MODBUS.txlen-2]=tmp;
      MODBUS.buffer[MODBUS.txlen-1]=tmp>>8;
      MODBUS.txcnt=0;
    }
  }
  //����� ����������� ��������� �������
  MODBUS.rxgap=0;
  MODBUS.rxcnt=0;
  MODBUS_TIMER->CR1 |= 0x01;    //�������� ������
  MODBUS.rxtimer=0xFFFF;
}

//������� �������� ������ �� ������� 01 (������ DO)
void ModbusCreateMsg01(void)
{
#ifdef DO_OBJECTS
  unsigned int count_to_read,start_read_adress,count_to_read_reg,offset;
  unsigned int m=0,n=0,tmp_val1;

  //2-3  - starting address
  start_read_adress=((MODBUS.buffer[2]<<8)+MODBUS.buffer[3]); //��������� ����� ��� ������
  offset=(start_read_adress-16*(start_read_adress/16));
  if(start_read_adress>=DO_OBJECTS)
  {
    ModbusCreateMsgError(ERROR_02);
    return;
  }
  //4-5 - number of registers
  count_to_read=((MODBUS.buffer[4]<<8)+MODBUS.buffer[5]);//���������� ��������� ��� ������
  if((start_read_adress+count_to_read)>DO_OBJECTS)
  {
    ModbusCreateMsgError(ERROR_02);
    return;
  }
  //default answer length if error
  n=3; 
  count_to_read_reg=((count_to_read/8)+1);
  //���� ��� ���������� ����� ������� � � ��������� �����
  if((((count_to_read+start_read_adress)<=DO_OBJECTS)&(start_read_adress<=DO_OBJECTS)))
  {
    for(m=0;m<count_to_read_reg;m++)
    {
      tmp_val1=DO_Registers[m]>>offset;
      
      //tmp_val1|=(DO_Registers[m+1]<<(8-offset));
      MODBUS.buffer[n+m] = tmp_val1;
      n++;
    }
     //������� ����� ���������� ������ � ������ � ������� ����������
     MODBUS.buffer[2]=m; //byte count
     //���������� � ��������
     MODBUS.txlen=m+5; //responce length
  }  
#else
  ModbusCreateMsgError(ERROR_01);
  return;  
#endif //DO_OBJECTS  
}

//������� �������� ������ �� ������� 02 (������ DI)
void ModbusCreateMsg02(void)
{
  unsigned int count_to_read,start_read_adress,count_to_read_reg,offset;
  unsigned int m=0,n=0,tmp_val1;

  //2-3  - starting address
  start_read_adress=((MODBUS.buffer[2]<<8)+MODBUS.buffer[3]); //��������� ����� ��� ������
  offset=(start_read_adress-16*(start_read_adress/16));
  if(start_read_adress>=DI_OBJECTS)
  {
    ModbusCreateMsgError(ERROR_02);
    return;
  }
  //4-5 - number of registers
  count_to_read=((MODBUS.buffer[4]<<8)+MODBUS.buffer[5]);//���������� ��������� ��� ������
  if((start_read_adress+count_to_read)>DI_OBJECTS)
  {
    ModbusCreateMsgError(ERROR_02);
    return;
  }
  //default answer length if error
  n=3; 
  count_to_read_reg=((count_to_read/16)+1);
  //���� ��� ���������� ����� ������� � � ��������� �����
  if((((count_to_read+start_read_adress)<=DI_OBJECTS)&(start_read_adress<=DI_OBJECTS)))
  {
    for(m=0;m<count_to_read_reg;m++)
    {
      tmp_val1=DI_Registers[m]>>offset;
      MODBUS.buffer[n+m] = tmp_val1;
      n++;
    }
     //������� ����� ���������� ������ � ������ � ������� ����������
     MODBUS.buffer[2]=m; //byte count
     //���������� � ��������
     MODBUS.txlen=m+5; //responce length
  }     
}

//������� �������� ������ �� ������� 03 (������ AO)
void ModbusCreateMsg03(void)
{
  unsigned int count_to_read,start_read_adress;
  unsigned int m=0,n=0;
  int tmp_val,tmp_val_pos;

  //2-3  - starting address
  count_to_read=((MODBUS.buffer[2]<<8)+MODBUS.buffer[3]); //��������� ����� ��� ������
  if(count_to_read>=AO_OBJECTS)
  {
    ModbusCreateMsgError(ERROR_02);
    return;
  }
  //4-5 - number of registers
  start_read_adress=((MODBUS.buffer[4]<<8)+MODBUS.buffer[5]);//���������� ��������� ��� ������
  if((start_read_adress+count_to_read)>AO_OBJECTS)
  {
    ModbusCreateMsgError(ERROR_02);
    return;
  }
  //default answer length if error
  n=3; 

  //���� ��� ���������� ����� ������� � � ��������� �����
  if((((count_to_read+start_read_adress)<=AO_OBJECTS)&(start_read_adress<=AO_OBJECTS)))
  {
    for(m=0;m<start_read_adress;m++)
    {
      tmp_val=AO_Registers[m+count_to_read];//������ ������� �������� 
      if(tmp_val<0)
      {
        //������ �������������
        tmp_val_pos=tmp_val;
        MODBUS.buffer[n]=(tmp_val_pos>>8)|(0x80);
        MODBUS.buffer[n+1]=tmp_val_pos;
      }
      else
      {
        //������ �������������
        MODBUS.buffer[n+1]=tmp_val;
        MODBUS.buffer[n]=tmp_val>>8;
      }
      n=n+2;
    }
     //������� ����� ���������� ������ � ������ � ������� ����������
     MODBUS.buffer[2]=m*2; //byte count
     //���������� � ��������
     MODBUS.txlen=m*2+5; //responce length
  }   
}

//������� �������� ������ �� ������� 04 (������ AI)
void ModbusCreateMsg04(void)
{
#ifdef AI_OBJECTS  
  unsigned int count_to_read,start_read_adress;
  unsigned int m=0,n=0;
  int tmp_val,tmp_val_pos;

  //2-3  - starting address
  count_to_read=((MODBUS.buffer[2]<<8)+MODBUS.buffer[3]); //��������� ����� ��� ������
  if(count_to_read>=AI_OBJECTS)
  {
    ModbusCreateMsgError(ERROR_02);
    return;
  }
  //4-5 - number of registers
  start_read_adress=((MODBUS.buffer[4]<<8)+MODBUS.buffer[5]);//���������� ��������� ��� ������
  if((start_read_adress+count_to_read)>AI_OBJECTS)
  {
    ModbusCreateMsgError(ERROR_02);
    return;
  }
  //default answer length if error
  n=3; 

  //���� ��� ���������� ����� ������� � � ��������� �����
  if((((count_to_read+start_read_adress)<=AI_OBJECTS)&(start_read_adress<=AI_OBJECTS)))
  {
    for(m=0;m<start_read_adress;m++)
    {
      tmp_val=AI_Registers[m+count_to_read];//������ ������� �������� 
      if(tmp_val<0)
      {
        //������ �������������
        tmp_val_pos=tmp_val;
        MODBUS.buffer[n]=(tmp_val_pos>>8)|(0x80);
        MODBUS.buffer[n+1]=tmp_val_pos;
      }
      else
      {
        //������ �������������
        MODBUS.buffer[n+1]=tmp_val;
        MODBUS.buffer[n]=tmp_val>>8;
      }
      n=n+2;
    }
     //������� ����� ���������� ������ � ������ � ������� ����������
     MODBUS.buffer[2]=m*2; //byte count
     //���������� � ��������
     MODBUS.txlen=m*2+5; //responce length
  }     
#else
  ModbusCreateMsgError(ERROR_01);
  return;
#endif //AI_OBJECTS  
}

//������� �������� ������ �� ������� 05 (������ ������ DO)
void ModbusCreateMsg05(void)
{
#ifdef DO_OBJECTS  
  unsigned int start_read_adress,offset;
  unsigned int tmp_val1;

  //2-3  - starting address
  start_read_adress=((MODBUS.buffer[2]<<8)+MODBUS.buffer[3]); //��������� ����� ��� ������
  offset=(start_read_adress-16*(start_read_adress/16));
  if(start_read_adress>=DO_OBJECTS)
  {
    ModbusCreateMsgError(ERROR_02);
    return;
  }
  //default answer length if error
  tmp_val1=(MODBUS.buffer[4]<<8)+MODBUS.buffer[5];
  switch (tmp_val1)
  {
    case 0x0000: DO_Registers[start_read_adress/16]&=~(1<<offset); break;
    case 0xFF00: DO_Registers[start_read_adress/16]|=(1<<offset); break;
    default: ModbusCreateMsgError(ERROR_03);return;
  }
  //DO_Registers[start_read_adress]=(MODBUS.buffer[4]<<8)+MODBUS.buffer[5];
  //���������� � ��������
  MODBUS.txlen=8; //responce length default answer length if no error
#else
  ModbusCreateMsgError(ERROR_01);
  return;
#endif //DO_OBJECTS
}

//������� �������� ������ �� ������� 06 (������ ������ AO)
void ModbusCreateMsg06(void)
{
  unsigned int read_adress, tmp=0;
  //2-3  - starting address
  read_adress=((MODBUS.buffer[2]<<8)+MODBUS.buffer[3]); //����� ��� ������
  if(read_adress>=AO_OBJECTS)
  {
    ModbusCreateMsgError(ERROR_02);
    return;
  }
  tmp=(MODBUS.buffer[4]<<8)+MODBUS.buffer[5];
  if (CheckAndWrightParameters((uint8_t)read_adress,tmp, AO_Registers)) 
  {
    ModbusCreateMsgError(ERROR_03);
    return;
  }
  else  AO_Registers[read_adress]=tmp;


  //���������� � ��������
  MODBUS.txlen=8; //responce length default answer length if no error
}

//������� �������� ������ �� ������� 15 (������ ���������� DO)
void ModbusCreateMsg15(void)
{
}

//������� �������� ������ �� ������� 16 (������ ���������� AO)
void ModbusCreateMsg16(void)
{
  unsigned int count_to_write,start_read_adress;
  unsigned int m=0,n=0;
  int tmp_val,tmp_val_pos;

  //2-3  - starting address
  start_read_adress=((MODBUS.buffer[2]<<8)+MODBUS.buffer[3]); //��������� ����� ��� ������
  if(start_read_adress>=AO_OBJECTS)
  {
    ModbusCreateMsgError(ERROR_02);
    return;
  }
  //4-5 - number of registers
  count_to_write=((MODBUS.buffer[4]<<8)+MODBUS.buffer[5]);//���������� ��������� ��� ������
  if((count_to_write+start_read_adress)>AO_OBJECTS)
  {
    ModbusCreateMsgError(ERROR_02);
    return;
  }
  //default answer length if error
  n=3; 

  //���� ��� ���������� ����� ������� � � ��������� �����
  if((((count_to_write+start_read_adress)<=AO_OBJECTS)&(start_read_adress<=AO_OBJECTS)))
  {
    for(m=0;m<count_to_write;m++)
    {
      if (CheckAndWrightParameters((uint8_t)(start_read_adress+m),((MODBUS.buffer[7+2*m]<<8) + MODBUS.buffer[8+2*m]), AO_Registers)) 
      {
        ModbusCreateMsgError(ERROR_03);
        return;
      }
      else  AO_Registers[start_read_adress+m]=((MODBUS.buffer[7+2*m]<<8) + MODBUS.buffer[8+2*m]);	//7 � 8 ������ �������� ������ ��� ������
    }
     MODBUS.txlen=8; //responce length
  }   	  
}

//������� �������� ��������������� ���������
void ModbusSendMsg(void)
{
  if((MODBUS.txlen>0)&(MODBUS.txcnt==0))
  {
    MODBUS_TIMER->IER &= ~0x01; // ��������� ���������� �� �������
    USART_ITConfig(MODBUS_UART, USART_IT_RXNE, DISABLE);//���� ���������� �� �����
    USART_ITConfig(MODBUS_UART, USART_IT_TC, ENABLE);//�������� �� �������� ��������
    RS_DE_ON;// �������� rs485 �� ��������
    USART_SendData8(MODBUS_UART, MODBUS.buffer[MODBUS.txcnt++]);
  }  
}

//������� �������� ������ ���������
void ModbusCreateMsgError(unsigned char ERROR_CODE)
{
  MODBUS.buffer[1] |= 0x80; //�������� ������� ��� ���� �������
  MODBUS.buffer[2] = ERROR_CODE; //��� ������ "�������� �������"
  MODBUS.txlen=5; //responce length  
}

//������� ��������� ���������� ������� ������� ���������� ������
void ModbusTimInterruptHandler(void)
{
  if((MODBUS.rxtimer++ > MODBUS.delay)&(MODBUS.rxcnt>1))  MODBUS.rxgap=1;
  else MODBUS.rxgap=0;
  MODBUS_TIMER->SR1 &= ~0x01;//���������� ���� ����������
}

//������� ��������� ���������� ������ ���� ������� ���������� ������
void ModbusUartIntRxHandler(void)
{
  MODBUS.rxtimer=0;
  if(MODBUS.rxcnt>(BUF_SZ-2))
    MODBUS.rxcnt=0;
  if(MODBUS_UART->SR &= USART_SR_PE){}
  else
  MODBUS.buffer[MODBUS.rxcnt++]=USART_ReceiveData8 (MODBUS_UART);
  USART_ClearFlag(MODBUS_UART, USART_FLAG_RXNE);  
}

//������� ��������� ���������� �������� ���� ������� ���������� ������ 
void ModbusUartIntTxHandler(void)
{
  if(MODBUS.txcnt<MODBUS.txlen)
  {
    USART_SendData8(MODBUS_UART,MODBUS.buffer[MODBUS.txcnt++]);//��������
  }
  else
  {
    //������� ����������� � �� ������� ������� ������� �RS485 TXE
    MODBUS.txlen=0;
    RS_DE_OFF;
    //LEDG1_OFF;
    USART_ITConfig(MODBUS_UART, USART_IT_RXNE, ENABLE);
    USART_ITConfig(MODBUS_UART, USART_IT_TC, DISABLE);
    MODBUS_TIMER->IER |= 0x01; // �������� ���������� �� �������
  }
  USART_ClearFlag(MODBUS_UART, USART_FLAG_TC);  
}

//������� �������� ������ ��������� � �������� �����
void ModbusTaskLoop(void)
{
  if(MODBUS.rxgap==1)//���� gap - �.�. ����������
  {
    ModbusCheckMsg();//��������� � ���� ����� ��� �� ������ � �� ����� �� ��������� �����
    ModbusSendMsg();//���� ���� ������� ���������� ������� �������������� ��������
  }  
}

//������� ���������� crc16
unsigned int Crc16(unsigned char* pData,unsigned char len)
{
  unsigned int crc16=0xFFFF;
  unsigned int ui16=0xA001;
  unsigned char flag=0;
  while (len--) 
  {
    ui16 = *pData++ ;
    crc16 ^= ui16;
    for( int i=0;i < 8;++i)
    {
      flag = (crc16 & 1);
      crc16 >>= 1;
      if (flag) 
      {
        crc16 ^= 0xA001;
      }
    }
  }
  return crc16;
}