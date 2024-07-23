#ifndef MODBUS_H
#define MODBUS_H

#define BUF_SZ 256 //������ ������

//���� ������ �������
//=========================================================================
#define ERROR_01 0x01
#define ERROR_02 0x02
#define ERROR_03 0x03
//=========================================================================

//�������� ������� (x*0.1 ms)
//=========================================================================
#define ANSWER_DELAY_2400       63
#define ANSWER_DELAY_4800       32
#define ANSWER_DELAY_9600       14
#define ANSWER_DELAY_19200      6
//=========================================================================

//uart structure
typedef struct {
unsigned char buffer[BUF_SZ];//�����
unsigned int rxtimer;//���� �� ������� �������
unsigned char rxcnt; //���������� �������� ��������
unsigned char txcnt;//���������� ���������� ��������
unsigned char txlen;//����� ������� �� ��������
unsigned char rxgap;//���� ��������� ������
unsigned char delay;//��������
} Modbus_struct;

void ModbusCheckMsg(void);//������� �������� ��������� ���������
void ModbusCreateMsg01(void);//������� �������� ������ �� ������� 01 (������ DO)
void ModbusCreateMsg02(void);//������� �������� ������ �� ������� 02 (������ DI)
void ModbusCreateMsg03(void);//������� �������� ������ �� ������� 03 (������ AO)
void ModbusCreateMsg04(void);//������� �������� ������ �� ������� 04 (������ AI)
void ModbusCreateMsg05(void);//������� �������� ������ �� ������� 05 (������ ������ DO)
void ModbusCreateMsg06(void);//������� �������� ������ �� ������� 06 (������ ������ AO)
void ModbusCreateMsg15(void);//������� �������� ������ �� ������� 15 (������ ���������� DO)
void ModbusCreateMsg16(void);//������� �������� ������ �� ������� 16 (������ ���������� AO)
void ModbusSendMsg(void);//������� �������� ��������������� ���������
void ModbusCreateMsgError(unsigned char ERROR_CODE);//������� �������� ������ ���������
void ModbusTaskLoop(void);//������� �������� ������ ��������� � �������� �����

//��� ������� ���������� �������� � ��������������� ����������
//=====================================================================================
void ModbusTimInterruptHandler(void);//������� ��������� ���������� ������� ������� ���������� ������
void ModbusUartIntRxHandler(void);//������� ��������� ���������� ������ ���� ������� ���������� ������
void ModbusUartIntTxHandler(void);//������� ��������� ���������� �������� ���� ������� ���������� ������ 
//=====================================================================================
unsigned int Crc16(unsigned char* pData,unsigned char len);//������� ���������� crc16
//=========================================================================


#endif // MODBUS_H