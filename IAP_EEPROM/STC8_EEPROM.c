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
#include<STC8_EEPROM.h>

#ifndef XTAL
#define XTAL 11.059200
#endif //如果没有定义晶振频率，则默认为11.0592M晶振

/*///////////////////////////////////////////////////////////////////////////////////
*函数名：EEPROM_ConfigWaitTime
*函数功能：配置STC8单片机EEPROM的等待时间
*参数列表：
*   无
*返回值：无
*版本：1.0
*作者：何相龙
*日期：2016年11月1日
*////////////////////////////////////////////////////////////////////////////////////
void EEPROM_ConfigWaitTime()
{
	if(XTAL >= 30)
		IAP_CONTER = 0x00;
	else if(XTAL >= 24)
		IAP_CONTER = 0x01;
	else if(XTAL >= 20)
		IAP_CONTER = 0x02;
	else if(XTAL >= 12)
		IAP_CONTER = 0x03;
	else if(XTAL >= 6)
		IAP_CONTER = 0x04;
	else if(XTAL >= 3)
		IAP_CONTER = 0x05;
	else if(XTAL >= 2)
		IAP_CONTER = 0x06;
	else
		IAP_CONTER = 0x07;
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：EEPROM_ReadByte
*函数功能：从STC8单片机EEPROM读取一个字节的数据
*参数列表：
*   addr
*       参数类型：unsigned int型数据
*       参数描述：要读取的数据的地址
*返回值：unsigned char型数据，读取到的数据
*版本：1.0
*作者：何相龙
*日期：2016年11月1日
*////////////////////////////////////////////////////////////////////////////////////
unsigned char EEPROM_ReadByte(unsigned int addr) 
{
	EEPROM_ConfigWaitTime();
	IAP_CMD = 0x01;				//EEPROM读命令
	IAP_ADDRL = addr & 0xFF;
	IAP_ADDRH = addr >> 8;
	IAP_TRIG = 0x5A;			//触发指令
	IAP_TRIG = 0xA5;
	return IAP_DATA;
}
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：EEPROM_WriteByte
*函数功能：向STC8单片机EEPROM写入一个字节的数据
*参数列表：
*   addr
*       参数类型：unsigned int型数据
*       参数描述：要写入的数据的地址
*   dat
*       参数类型：unsigned int型数据
*       参数描述：要写入的数据
*返回值：unsigned char型数据，是否成功（成功：0；失败：1）
*版本：1.0
*作者：何相龙
*日期：2016年11月1日
*////////////////////////////////////////////////////////////////////////////////////
unsigned char EEPROM_WriteByte(unsigned int addr, unsigned char dat) 
{
	EEPROM_ConfigWaitTime();
	IAP_CMD = 0x02;				//EEPROM读命令
	IAP_ADDRL = addr & 0xFF;
	IAP_ADDRH = addr >> 8;
	IAP_DATA = dat;
	IAP_TRIG = 0x5A;			//触发指令
	IAP_TRIG = 0xA5;
	if(IAP_CMD & 0x10)
		return 1;
	else return 0;
}
