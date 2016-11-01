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
	功能描述：STC8单片机EEPROM操作模块测试程序
	注意：使用此测试程序时，请将用户EEPROM设置到20K以上
*////////////////////////////////////////////////////////////////////////////////////////
#include<STC8.h>
#include<STC8_EEPROM.h>
#include<../UART/UART1/STC8_UART1.h>
#include<../UART/UART1/STC8_UART1.c>
void Timer0Init(void);
xdata unsigned char temp1[512] = {0};
xdata unsigned char temp2[512] = {0};

void EEPROMTest_SingleByte()
{
	unsigned int i = 0;	
	EEPROM_EreasePage(0x1000);	
	for(i = 0; i < 512; i++)
		EEPROM_WriteByte(0x1000 + i, i);	
	
	for(i = 0; i < 512; i++)
		temp1[i] = EEPROM_ReadByte(0x1000 + i);	
	UART1_SendString(temp1, 512);
	
	EEPROM_EreasePage(0x1000);
	for(i = 0; i < 512; i++)
		temp1[i] = EEPROM_ReadByte(0x1000 + i);	
	UART1_SendString(temp1, 512);
}
void EEPROMTest_Bytes()
{
	unsigned int i;
	for(i = 0; i < 512; i++)
		temp1[i] = i + 233;
	EEPROM_WriteBytes(0x0010, temp1, 300);
	EEPROM_ReadBytes(0x0010, temp2, 300);
	UART1_SendString(temp2, 300);
	EEPROM_EreasePage(0x0010);
}
void main()
{	
	UART1_Conf(9600, 1);
	Timer0Init();
	
	EEPROMTest_SingleByte();
	EEPROMTest_Bytes();
	
	while(1)
	{
		UART1_Driver();
	}
}
void UART1_Action(unsigned char *dat,unsigned int len)
{
	EEPROM_WriteBytes(0x2000, dat, len);
	EEPROM_ReadBytes(0x2000, temp1, len);
	UART1_SendString(temp1, len);
	EEPROM_EreasePage(0x2000);
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
	UART1_RxMonitor(1);
}