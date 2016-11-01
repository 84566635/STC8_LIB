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
#ifndef _STC8_UART4_H_
#define _STC8_UART4_H_
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
*////////////////////////////////////////////////////////////////////////////////////
void UART4_Conf(unsigned long baud, unsigned char timer);
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
*////////////////////////////////////////////////////////////////////////////////////
void UART4_SendString(unsigned char *dat, unsigned char len);
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：UART4_Driver
*函数功能：串口4通信监控函数，在主循环中调用。
*         如果接收到字符串，会自动调用另行编写的UART4_Action(unsigned char *dat,unsigned char len)
*////////////////////////////////////////////////////////////////////////////////////
void UART4_Driver();
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：UART_RxMonitor
*函数功能：串口4字符串接收结束判断，在定时器0中断函数中调用
*参数列表：
*   ms
*       参数类型：unsigned char型数据
*       参数描述：定时器延时时长（单位：ms）
*////////////////////////////////////////////////////////////////////////////////////
void UART4_RxMonitor(unsigned char ms);
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
*////////////////////////////////////////////////////////////////////////////////////
void UART4_IOPortSwitch(unsigned char choose);
#endif