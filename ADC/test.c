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
	功能描述：STC8单片机串口1字符串通信模块测试程序
*////////////////////////////////////////////////////////////////////////////////////////
#include<STC8.h>
#include<STC8_ADC.h>
#include<../UART/UART1/STC8_UART1.h>
void Timer0Init(void);
int main()
{
	UART1_Conf(9600, 1);
	Timer0Init();
	UART1_SendStringNow("Ready!\r\n", 8);
	while(1)
	{
		UART1_Driver();
		ADC_Driver();
	}
}
void ADC_Action(unsigned char channel, unsigned int result)
{
	unsigned char volatile pdata str[30] = "Channel00:0000AUTO\r\n";
	str[7] = channel / 10 + '0';
	str[8] = channel % 10 + '0';
	
	str[10] = result / 1000 + '0';
	str[11] = result / 100 % 10 + '0';
	str[12] = result / 10 % 10 + '0';
	str[13] = result % 10 + '0';
	
	UART1_AddStringToSendBuffer(str, 20);
}
void UART1_Action(unsigned char *dat, unsigned int len)
{
	unsigned int result;
	unsigned char volatile pdata str[30] = "Channel00:0000\r\n";
	str[7] = dat[0] / 0x10 + '0';
	str[8] = dat[0] % 0x10 + '0';
	result = ADC_GetNow(dat[0]);
	str[10] = result / 1000 + '0';
	str[11] = result / 100 % 10 + '0';
	str[12] = result / 10 % 10 + '0';
	str[13] = result % 10 + '0';
	UART1_AddStringToSendBuffer(str, 16);
}
void Timer0Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xD4;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
}
void Interrupt_Timer0() interrupt 1
{
	static unsigned int t = 0;
	static unsigned char c = 0;
	UART1_RxMonitor(1);
	t++;
	if(t >= 1000)
	{
		t = 0;
		ADC_Start(c, 0);
		c++;
		c %= 16;
	}
}