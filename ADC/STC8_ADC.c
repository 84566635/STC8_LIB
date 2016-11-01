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
	功能描述：STC8单片机ADC操作模块
*////////////////////////////////////////////////////////////////////////////////////////
#include<STC8.h>

#ifndef XTAL
#define XTAL 11.059200
#endif //如果没有定义晶振频率，则默认为11.0592M晶振
bit ADC_Finished = 0;			//ADC转换完成标志
unsigned int ADC_Result = 0;	//ADC转换结果
extern void ADC_Action(unsigned char channel, unsigned int result);
//此函数须另行编写：当ADC完成一次异步转换后会自动调用

/*///////////////////////////////////////////////////////////////////////////////////
*函数名：ADC_Start
*函数功能：启动STC8单片机ADC（中断法、异步）
*参数列表：
*   channel
*       参数类型：unsigned char型数据
*       参数描述：要转换的通道
*			0:P1.0		1:P1.1		2:P1.2		3:P1.3
*			4:P1.4		5:P1.5		6:P1.6		7:P1.7
*			8:P0.0		9:P0.1		10:P0.2		11:P0.3
*			12:P0.4		13:P0.5		14:P0.6		15:P0.7
*   speed
*       参数类型：unsigned char型数据
*       参数描述：ADC转换的速度（取值0～15，数值越小越快。最快32时钟1次转换；最慢512时钟一次）
*			
*返回值：无
*版本：1.0
*作者：何相龙
*日期：2016年11月1日
*////////////////////////////////////////////////////////////////////////////////////
void ADC_Start(unsigned char channel, unsigned char speed) 
{
	EADC = 1;
	ADCCFG = 0x20 | speed;		//结果右对齐
	ADC_CONTR = 0xC0 | channel;	
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：ADC_Get
*函数功能：获取STC8单片机ADC结果值（中断法、异步）
*参数列表：
*   无
*返回值：unsigned int型数据，ADC转换的结果
*版本：1.0
*作者：何相龙
*日期：2016年11月1日
*////////////////////////////////////////////////////////////////////////////////////
unsigned int ADC_Get()
{
	unsigned int temp = 0x0000;
	EADC = 0;
	temp = ADC_RES;
	temp <<= 8;
	temp |= ADC_RESL;
	return temp;
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：ADC_Start
*函数功能：立即从STC8单片机ADC获取一次转换结果（不通过中断、查询法）
*参数列表：
*   channel
*       参数类型：unsigned char型数据
*       参数描述：要转换的通道
*			0:P1.0		1:P1.1		2:P1.2		3:P1.3
*			4:P1.4		5:P1.5		6:P1.6		7:P1.7
*			8:P0.0		9:P0.1		10:P0.2		11:P0.3
*			12:P0.4		13:P0.5		14:P0.6		15:P0.7
*返回值：unsigned int型数据，ADC转换的结果
*版本：1.0
*作者：何相龙
*日期：2016年11月1日
*////////////////////////////////////////////////////////////////////////////////////
unsigned int ADC_GetNow(unsigned char channel) 
{
	unsigned int temp = 0x0000;
	bit EADCBackup;
	EADCBackup = EADC;
	EADC = 0;
	ADCCFG = 0x20;				//结果右对齐
	ADC_CONTR = 0xC0 | channel;	
	
	while(!(ADC_CONTR & 0x20));	//等待转换完成
	ADC_CONTR &= 0xDF;
	
	temp = ADC_RES;
	temp <<= 8;
	temp |= ADC_RESL;
	EADC = EADCBackup;
	return temp;
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：ADC_Driver
*函数功能：ADC监控函数，在主循环中调用。
*         如果ADC异步转换完成，会自动调用另行编写的void ADC_Action(unsigned char channel, unsigned int result)
*参数列表：
*   无
*返回值：无
*版本：1.0
*作者：何相龙
*日期：2016年11月1日
*////////////////////////////////////////////////////////////////////////////////////
void ADC_Driver()
{
	if(ADC_Finished)
	{
		ADC_Finished = 0;
		ADC_Action(ADC_CONTR & 0x0F, ADC_Result);
	}
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：interrupt_ADC
*函数功能：ADC中断函数
*参数列表：
*   无
*返回值：无
*版本：1.0
*作者：何相龙
*日期：2016年11月1日
*////////////////////////////////////////////////////////////////////////////////////
void interrupt_ADC() interrupt 5
{
	ADC_CONTR &= 0xDF;
	ADC_Result = ADC_Get();
	ADC_Finished = 1;
}