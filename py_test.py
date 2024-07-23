import serial
from serial import tools
from serial.tools import list_ports
from time import sleep


a=[0x19,0xe6, 0x00, 0x00]
v=[40, 215, 0 , 0]
sernum=[85, 170, 1, 89, 2, 89]
reset=[145, 110, 0, 0]
b='SIU_LED_V2_START_PROGRAMMING'
magic_word=[]

def crc16(buf, lenth):
    crc16=0xFFFF
    ui16=0xA001
    flag=0
    i=0
    k=1
    while lenth!=0:
        ui16=buf[i]
        i+=1
        k=0
        crc16 ^=ui16
        while k<8:
            flag=(crc16 & 1)
            crc16>>=1
            if flag==1: crc16 ^= 0xA001
            k+=1
            pass
        lenth-=1
        pass
    buf.append((crc16&0x00FF))
    buf.append((crc16&0xFF00)>>8)
    pass


ser = serial.Serial()
ser.port='COM5'
ser.baudrate = 9600
ser.parity = 'E'
ser.open()

for w in b:
    magic_word.append(ord(w))
    pass

print(ser)
crc16(a, 4)
crc16(v, 4)
crc16(reset, 4)
crc16(sernum, 6)

ser.write(sernum)

sleep(0.1)
answer=ser.read(8)
print(answer)
ser.close()
