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
	功能描述：STC8单片机串口4字符串通信模块
	备注：尽量使用封装好的函数进行操作，而不要使用直接对串口进行操作。
        使用该模块，请在config.h中定义UART_BUFF_MAX常量为数据缓存数组最大长度。
            如 #define UART4_BUFF_MAX 64
        使用该模块，请在config.h中定义XTAL常量为晶振频率（单位：兆赫兹）。
            如 #define XTAL 11.059200
*////////////////////////////////////////////////////////////////////////////////////////
#include<STC8.h>
#include<STC8_UART4.h>

#ifndef UART4_BUFF_MAX
#define UART4_BUFF_MAX 64
#endif // 如果没有定义BUFFMAX，则默认为64

#ifndef XTAL
#define XTAL 11.059200
#endif //如果没有定义晶振频率，则默认为11.0592M晶振

extern void UART4_Action(unsigned char *dat, unsigned char len);
//此函数须另行编写：当串口完成一个字符串结束后会自动调用

unsigned char xdata UART4_Buff[UART4_BUFF_MAX];     //串口4接收缓冲区
unsigned char UART4_BuffIndex = 0;           //串口4接收缓冲区当前位置

bit UART4_SendFlag;                          //串口4发送完成标志
bit UART4_ResiveFlag;                        //串口4接收完成标志
bit UART4_ResiveStringEndFlag;               //串口4字符串接收全部完成标志
bit UART4_ResiveStringFlag;                  //串口4字符串正在接收标志

/*///////////////////////////////////////////////////////////////////////////////////
*函数名：UART4_Conf
*函数功能：配置STC8单片机串口4
*参数列表：
*   baud
*       参数类型：unsigned long型数据
*       参数描述：要设置的波特率
*   timer
*       参数类型：unsigned char型数据
*       参数描述：要使用的用于产生波特率的定时器（可选择定时器2、4）
*		注意：不要与程序其他部分选用的定时器发生冲突、当且仅当不同串口的波特率一样时可共用一个定时器。
*返回值：无
*版本：1.0
*作者：何相龙
*日期：2016年11月1日
*////////////////////////////////////////////////////////////////////////////////////
void UART4_Conf(unsigned long baud, unsigned char timer) 
{
	unsigned long temp;
	
	S4CON = 0x10;		//8位数据,可变波特率
	IE2 |= 0x10;		//使能串口4中断
	EA = 1;         	//使能总中断
	
	temp = 65536 - XTAL * 1000000 / 4 / baud;    //计算定时器初值
	if(timer == 4)
	{
WrongTimerChoosed:
		S4CON |= 0x40;		//串口4选择定时器4为波特率发生器
		T4T3M |= 0x20;		//定时器4时钟为Fosc,即1T
		T4L = temp & 0xFF;
		T4H = temp >> 8;
		T4T3M |= 0x80;		//启动定时器4
	}
	else if(timer == 2)
	{
		S4CON &= 0xBF;		//串口4选择定时器2为波特率发生器
		AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
		T2L = temp & 0xFF;	//设定定时初值
		T2H = temp >> 8;	//设定定时初值
		AUXR |= 0x10;		//启动定时器2
	}
	else goto WrongTimerChoosed; 
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：UART4_SendString
*函数功能：向串口4发送一个字符串
*参数列表：
*   *dat
*       参数类型：unsigned char型指针
*       参数描述：要发送的字符串的首地址
*   len
*       参数类型：unsigned char型数据
*       参数描述：要发送的字符串的长度
*返回值：无
*版本：1.0
*作者：何相龙
*日期：2014年12月9日
*////////////////////////////////////////////////////////////////////////////////////
void UART4_SendString(unsigned char *dat, unsigned char len)
{
	while(len)
	{
		len --;                     //每发送一位，长度减1
		S4BUF = *dat;               //发送一位数据
		dat ++;                     //数据指针移向下一位
		while(! UART4_SendFlag);    //等待串口发送完成标志
		UART4_SendFlag = 0;         //清空串口发送完成标志
	}
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：UART4_Read
*函数功能：将串口4暂存数组中的数据读取出来。
*参数列表：
*   *to
*       参数类型：unsigned char型指针
*       参数描述：存储接收到的字符的位置
*   len
*       参数类型：unsigned char型数据
*       参数描述：要读取的字符串的长度
*返回值：unsigned char型数据，字符串的实际长度
*版本：1.0
*作者：何相龙
*日期：2014年12月9日
*////////////////////////////////////////////////////////////////////////////////////
unsigned char UART4_Read(unsigned char *to, unsigned char len)
{
	unsigned char i;
	if(UART4_BuffIndex < len)len = UART4_BuffIndex;   //获取串口4当前接收数据的位数
	for(i = 0;i < len;i ++)                           //复制数据的目标数组
		{
			*to = UART4_Buff[i];
			to ++;
		}
	UART4_BuffIndex = 0;                              //清空串口4接收缓冲区当前位置
	return len;
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：UART4_Driver
*函数功能：串口4通信监控函数，在主循环中调用。
*         如果接收到字符串，会自动调用另行编写的UART4_Action(unsigned char *dat,unsigned char len)
*参数列表：
*   无
*返回值：无
*版本：1.0
*作者：何相龙
*日期：2014年12月9日
*////////////////////////////////////////////////////////////////////////////////////
void UART4_Driver()
{
	unsigned char xdata dat[UART4_BUFF_MAX];       //定义数据暂存数组
	unsigned char len;                       //数据的长度
	if(UART4_ResiveStringEndFlag)            //如果串口4接收到一个完整的字符串
		{
			UART4_ResiveStringEndFlag = 0;   //清空接收完成标志
			len = UART4_Read(dat, UART4_BUFF_MAX);  //将数据从原数组读出，并得到数据的长度
			UART4_Action(dat, len);          //调用用户编写的UART_Action函数，将接收到的数据及数据长度作为参数
		}
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：UART4_RxMonitor
*函数功能：串口4字符串接收结束判断，在定时器0中断函数中调用
*参数列表：
*   ms
*       参数类型：unsigned char型数据
*       参数描述：定时器延时时长（单位：ms）
*返回值：无
*版本：1.0
*作者：何相龙
*日期：2014年12月9日
*////////////////////////////////////////////////////////////////////////////////////
void UART4_RxMonitor(unsigned char ms)
{
	static unsigned char ms30 = 0;                   //30毫秒计时
	static unsigned char UART4_BuffIndex_Backup;     //串口4数据暂存数组位置备份
	if(! UART4_ResiveStringFlag)return ;             //如果当前没有在接受数据，直接退出函数
    ms30 += ms;                                      //每一次定时器中断，表示时间过去了若干毫秒
	if(UART4_BuffIndex_Backup != UART4_BuffIndex)    //如果串口4数据暂存数组位置备份不等于串口4接收缓冲区当前位置（接收到了新数据位）
	{
		UART4_BuffIndex_Backup = UART4_BuffIndex;    //记录串口4当前的接收缓冲区位置
		ms30 = 0;                                    //复位30毫秒计时
	}
	if(ms30 > 30)                                    //30毫秒到了
		{
			ms30 = 0;                                //复位30毫秒计时
			UART4_ResiveStringEndFlag = 1;           //设置串口4字符串接收全部完成标志
			UART4_ResiveStringFlag = 0;              //清空串口4字符串正在接收标志
		}
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：UART4_IOPortSwitch
*函数功能：串口4功能脚选择
*参数列表：
*   choose
*       参数类型：unsigned char型数据
*       参数描述：要选择的功能脚位置(请注意不要跟其他功能的引脚发生冲突）
					取值	RxD位置		TxD位置
					0		P0.2		P0.3
					1		P5.2		P5.3
*返回值：无
*版本：1.0
*作者：何相龙
*日期：2016年11月1日
*////////////////////////////////////////////////////////////////////////////////////
void UART4_IOPortSwitch(unsigned char choose)
{
	P_SW2 &= 0xFB;
	P_SW2 |= (choose << 2);
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：interrupt_UART4
*函数功能：串口4中断函数
*参数列表：
*   无
*返回值：无
*版本：1.0
*作者：何相龙
*日期：2014年12月9日
*////////////////////////////////////////////////////////////////////////////////////
void interrupt_UART4() interrupt 18
{
	if(S4CON & 0x02)                          //如果串口4发送完成
	{
		S4CON &= 0xFD;                        //清空系统标志位
		UART4_SendFlag = 1;                   //设置串口4发送完成标志
	}
	if(S4CON & 0x01)                          //如果串口4接收完成
	{
		S4CON &= 0xFE;                        //清空系统标志位
		UART4_ResiveFlag = 1;                 //设置串口4接收完成标志
		UART4_Buff[UART4_BuffIndex] = S4BUF;  //将接收到的数据放到暂存数组
		UART4_ResiveStringFlag = 1;           //设置串口4字符串正在接收标志
		UART4_BuffIndex ++;                   //串口4接收缓冲区当前位置右移
	}
}