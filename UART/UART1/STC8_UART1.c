/*//////////////////////////GPL开源许可证////////////////////////////////////////////////
    Copyright (C) <2016>  <Xianglong He>
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	版权所有（C） <2016>   <何相龙>
	这一程序是自由软件，你可以遵照自由软件基金会出版的GNU通用公共许可证
	条款来修改和重新发布这一程序。你可以使用许可证的第三版，或者（根据你
	的选择）用任何更新的版本。
	发布这一程序的目的是希望它有用，但没有任何担保。甚至没有适合特定目的
	的隐含的担保。更详细的情况请参阅GNU通用公共许可证。
	你应该已经和程序一起收到一份GNU通用公共许可证的副本。如果还没有，请访
	问<http://www.gnu.org/licenses/>。
	作者：何相龙
	邮箱：qwgg9654@gmail.com
		  568629794@qq.com
	GitHub：hxl9654
	功能描述：STC8单片机串口1字符串通信模块
	备注：尽量使用封装好的函数进行操作，而不要使用直接对串口进行操作。
        使用该模块，请在config.h中定义UART_BUFF_MAX常量为数据缓存数组最大长度。
            如 #define UART1_BUFF_MAX 64
        使用该模块，请在config.h中定义XTAL常量为晶振频率（单位：兆赫兹）。
            如 #define XTAL 11.059200
*////////////////////////////////////////////////////////////////////////////////////////
#include<STC8.h>

#ifndef UART1_BUFF_MAX
#define UART1_BUFF_MAX 64
#endif // 如果没有定义BUFFMAX，则默认为64

#ifndef XTAL
#define XTAL 11.059200
#endif //如果没有定义晶振频率，则默认为11.0592M晶振

extern void UART1_Action(unsigned char *dat, unsigned int len);
//此函数须另行编写：当串口完成一个字符串结束后会自动调用

unsigned char xdata UART1_Buff[UART1_BUFF_MAX];     //串口1接收缓冲区
unsigned int UART1_BuffIndex = 0;            //串口1接收缓冲区当前位置

bit UART1_SendFlag;                          //串口1发送完成标志
bit UART1_ResiveFlag;                        //串口1接收完成标志
bit UART1_ResiveStringEndFlag;               //串口1字符串接收全部完成标志
bit UART1_ResiveStringFlag;                  //串口1字符串正在接收标志

/*///////////////////////////////////////////////////////////////////////////////////
*函数名：UART1_Conf
*函数功能：配置STC8单片机串口1
*参数列表：
*   baud
*       参数类型：unsigned long型数据
*       参数描述：要设置的波特率
*   timer
*       参数类型：unsigned char型数据
*       参数描述：要使用的用于产生波特率的定时器（可选择1、2）
*		注意：不要与程序其他部分选用的定时器发生冲突、当且仅当不同串口的波特率一样时可共用一个定时器。
*返回值：无
*版本：1.0
*作者：何相龙
*日期：2016年11月1日
*////////////////////////////////////////////////////////////////////////////////////
void UART1_Conf(unsigned long baud, unsigned char timer) 
{
	unsigned long temp;
	SCON = 0X50;    //配置串口工作模式
	temp = 65536 - XTAL * 1000000 / 4 / baud;    //计算定时器初值
	EA = 1;         //使能总中断
	ES = 1;         //使能串口中断
	
	if(timer == 1)
	{
WrongTimerChoosed:
		AUXR |= 0x40;	//定时器1时钟为Fosc,即1T
		AUXR &= 0xFE;	//串口1选择定时器1为波特率发生器
		TMOD &= 0x0F;	//设定定时器1为16位自动重装方式	
		TL1 = temp & 0xFF;
		TH1 = temp >> 8;
		TR1 = 1;        //使能定时器1
	}
	else if(timer == 2)
	{
		AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
		AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
		T2L = temp & 0xFF;
		T2H = temp >> 8;
		AUXR |= 0x10;		//启动定时器2
	}
	else goto WrongTimerChoosed;
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：UART1_SendString
*函数功能：向串口1发送一个字符串
*参数列表：
*   *dat
*       参数类型：unsigned char型指针
*       参数描述：要发送的字符串的首地址
*   len
*       参数类型：unsigned int型数据
*       参数描述：要发送的字符串的长度
*返回值：无
*版本：1.0
*作者：何相龙
*日期：2014年12月9日
*////////////////////////////////////////////////////////////////////////////////////
void UART1_SendString(unsigned char *dat, unsigned int len)
{
	while(len)
	{
		len --;                     //每发送一位，长度减1
		SBUF = *dat;                //发送一位数据
		dat ++;                     //数据指针移向下一位
		while(! UART1_SendFlag);    //等待串口发送完成标志
		UART1_SendFlag = 0;         //清空串口发送完成标志
	}
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：UART1_Read
*函数功能：将串口1暂存数组中的数据读取出来。
*参数列表：
*   *to
*       参数类型：unsigned char型指针
*       参数描述：存储接收到的字符的位置
*   len
*       参数类型：unsigned int型数据
*       参数描述：要读取的字符串的长度
*返回值：unsigned int型数据，字符串的实际长度
*版本：1.0
*作者：何相龙
*日期：2014年12月9日
*////////////////////////////////////////////////////////////////////////////////////
unsigned int UART1_Read(unsigned char *to, unsigned int len)
{
	unsigned int i;
	if(UART1_BuffIndex < len)len = UART1_BuffIndex;   //获取串口1当前接收数据的位数
	for(i = 0;i < len;i ++)                           //复制数据的目标数组
		{
			*to = UART1_Buff[i];
			to ++;
		}
	UART1_BuffIndex = 0;                              //清空串口1接收缓冲区当前位置
	return len;
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：UART1_Driver
*函数功能：串口1通信监控函数，在主循环中调用。
*         如果接收到字符串，会自动调用另行编写的UART1_Action(unsigned char *dat,unsigned int len)
*参数列表：
*   无
*返回值：无
*版本：1.0
*作者：何相龙
*日期：2014年12月9日
*////////////////////////////////////////////////////////////////////////////////////
void UART1_Driver()
{
	unsigned char xdata dat[UART1_BUFF_MAX];       //定义数据暂存数组
	unsigned int len;                        //数据的长度
	if(UART1_ResiveStringEndFlag)            //如果串口1接收到一个完整的字符串
		{
			UART1_ResiveStringEndFlag = 0;   //清空接收完成标志
			len = UART1_Read(dat, UART1_BUFF_MAX);  //将数据从原数组读出，并得到数据的长度
			UART1_Action(dat, len);          //调用用户编写的UART_Action函数，将接收到的数据及数据长度作为参数
		}
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：UART1_RxMonitor
*函数功能：串口1字符串接收结束判断，在定时器0中断函数中调用
*参数列表：
*   ms
*       参数类型：unsigned char型数据
*       参数描述：定时器延时时长（单位：ms）
*返回值：无
*版本：1.0
*作者：何相龙
*日期：2014年12月9日
*////////////////////////////////////////////////////////////////////////////////////
void UART1_RxMonitor(unsigned char ms)
{
	static unsigned char ms30 = 0;                   //30毫秒计时
	static unsigned int UART1_BuffIndex_Backup;     //串口1数据暂存数组位置备份
	if(! UART1_ResiveStringFlag)return ;             //如果当前没有在接受数据，直接退出函数
    ms30 += ms;                                      //每一次定时器中断，表示时间过去了若干毫秒
	if(UART1_BuffIndex_Backup != UART1_BuffIndex)    //如果串口1数据暂存数组位置备份不等于串口1接收缓冲区当前位置（接收到了新数据位）
	{
		UART1_BuffIndex_Backup = UART1_BuffIndex;    //记录串口1当前的接收缓冲区位置
		ms30 = 0;                                    //复位30毫秒计时
	}
	if(ms30 > 30)                                    //30毫秒到了
		{
			ms30 = 0;                                //复位30毫秒计时
			UART1_ResiveStringEndFlag = 1;           //设置串口1字符串接收全部完成标志
			UART1_ResiveStringFlag = 0;              //清空串口1字符串正在接收标志
		}
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：UART1_IOPortSwitch
*函数功能：串口1功能脚选择
*参数列表：
*   choose
*       参数类型：unsigned char型数据
*       参数描述：要选择的功能脚位置(请注意不要跟其他功能的引脚发生冲突）
					取值	RxD位置		TxD位置
					0		P3.0		P3.1
					1		P3.6		P3.7
					2		P1.6		P1.7
					3		P4.3		P4.4
*返回值：无
*版本：1.0
*作者：何相龙
*日期：2016年11月1日
*////////////////////////////////////////////////////////////////////////////////////
void UART1_IOPortSwitch(unsigned char choose)
{
	P_SW1 &= 0x3F;
	P_SW1 |= (choose << 6);
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：interrupt_UART1
*函数功能：串口1中断函数
*参数列表：
*   无
*返回值：无
*版本：1.0
*作者：何相龙
*日期：2014年12月9日
*////////////////////////////////////////////////////////////////////////////////////
void interrupt_UART1() interrupt 4
{
	if(TI)                                    //如果串口1发送完成
	{
		TI = 0;                               //清空系统标志位
		UART1_SendFlag = 1;                   //设置串口1发送完成标志
	}
	if(RI)                                    //如果串口1接收完成
	{
		RI = 0;                               //清空系统标志位
		UART1_ResiveFlag = 1;                 //设置串口1接收完成标志
		UART1_Buff[UART1_BuffIndex] = SBUF;   //将接收到的数据放到暂存数组
		UART1_ResiveStringFlag = 1;           //设置串口1字符串正在接收标志
		UART1_BuffIndex ++;                   //串口1接收缓冲区当前位置右移
	}
}