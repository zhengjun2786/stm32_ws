#ifndef __UART3_H_
#define __UART3_H_

//static int16_t C = 0;

//extern  static int16_t C;
	
typedef struct{
	uint8_t buff[100];
	uint8_t flag;		//接收到数据的标志位
	uint8_t len;
}TypeUsart3;
extern TypeUsart3 MyUsart3;

void canmv_Receive_Data(uint8_t Com_Data);
void USART3_init(u32 baud);                   //串口初始化函数
void usatr3_send_data(u8 c);                  //发送一个字节的函数
void uatr3_send_data(char * data,u8 len);     //串口3发送一个字符串的函数
//void canmv_Receive_Data(int16_t Com_Data);    //协议解析
#endif
