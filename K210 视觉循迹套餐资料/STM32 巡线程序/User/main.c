#include "stm32f10x.h"                  // Device header
#include "LEDSEG.h"
#include "Delay.h"
#include "robot.h"
#include <string.h>
#include <stdio.h>
#include "Key.h"
#include "Serial.h"
#include "Uart3.h"

static int16_t C = 0;

int sum=0; // 数据累加

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组分2组
	Key_Init();
	LEDSEG_Init();
	Serial_Init();              // 串口初始化
	USART3_init(115200);
	robot_Init();              // 机器人初始化
	Digital_Display(1);       // 数码管显示
	while (1)
	{
		
		if(MyUsart3.flag)	//接收到一次数据了
		{
			MyUsart3.flag=0; //清空标志位
			for(uint8_t i = 0; i<5;i++)
			{
				canmv_Receive_Data(MyUsart3.buff[i]);
			}
		}
		// C为获取到的角度值
		printf("C = %d \r\n", C);
		
		// 求角度平均值，避免修正过多
		for(uint8_t x=1;x<=10;x++)
		{
			sum += C;
		}
		C = (int) sum / 10 ;
		sum = 0; // 累加值清零
		
		// 为了没有数据的时候机器人不运动
		if(C == 0)
		{
			makerobo_run(0,0);
		}
		//出界了
		else if(C == 53)
		{
			makerobo_back(50,10); // 后退
		}
		else if(C > 10)
		{
		makerobo_Right(50,0); // 右转
		}
	  else if (C < -10)
	  {
		 makerobo_Left(50,0); // 左转
	  }
	  else
	  {
		 makerobo_run(60,50); // 前进
	  }		
	}
}

// 求平均值


// 解析数据包
void canmv_Receive_Data(uint8_t Com_Data)
{
  // ?????
  uint8_t i;
  //????
  static uint8_t RxCounter1=0;//??
  /*??????*/
  static uint8_t RxBuffer1[5]={0};
  /*???????*/
  static uint8_t RxState = 0;	
  /*???????,?????????*/
  if(RxState==0&&Com_Data==0xAA)  //0xAA??
  {
    RxState=1;
    RxBuffer1[RxCounter1++]=Com_Data;
  }
  else if (RxState==1&&Com_Data==0xAE)  //0xAE??
  {
    RxState=2;
    RxBuffer1[RxCounter1++]=Com_Data;
  }
  else if(RxState==2)
  {
    RxBuffer1[RxCounter1++]=Com_Data;
    if(RxCounter1>=5||Com_Data == 0x5B)       //RxBuffer1????,??????
    {
      RxState=3;
      int Cl=RxBuffer1[RxCounter1-3];
      int Ch=RxBuffer1[RxCounter1-2];
      C = Ch;
      C <<=8;
      C |= Cl; 
    }
  }
  else if(RxState==3)//???????????
  {
    if(RxBuffer1[RxCounter1-1] == 0x5B)
    {
      RxCounter1 = 0;
      RxState = 0;
    }
    else
    {
      RxState = 0;
      RxCounter1=0;
      for(i=0;i<5;i++)
      {
        RxBuffer1[i]=0x00;      //?????????
      }
    }
		
  }
  else   //????
  {
    RxState = 0;
    RxCounter1=0;
    for(i=0;i<10;i++)
		{
			RxBuffer1[i]=0x00;      //?????????
		}
  }
}

