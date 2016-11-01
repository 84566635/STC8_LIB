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
	功能描述：STC8单片机EEPROM操作模块
	备注：尽量使用封装好的函数进行操作，而不要使用直接对串口进行操作。
        使用该模块，请在config.h中定义XTAL常量为晶振频率（单位：兆赫兹）。
            如 #define XTAL 11.059200
*////////////////////////////////////////////////////////////////////////////////////////

#ifndef XTAL
#define XTAL 11.059200
#endif //如果没有定义晶振频率，则默认为11.0592M晶振

#ifndef _STC8_EEPROM_H_
#define _STC8_EEPROM_H_

/*///////////////////////////////////////////////////////////////////////////////////
*函数名：EEPROM_ReadByte
*函数功能：从STC8单片机EEPROM读取一个字节的数据
*参数列表：
*   addr
*       参数类型：unsigned int型数据
*       参数描述：要读取的数据的地址
*返回值：unsigned int型数据，读取到的数据(读出的数据为后8位，读取出错时，返回0x2333
*////////////////////////////////////////////////////////////////////////////////////
unsigned int EEPROM_ReadByte(unsigned int addr);
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：EEPROM_WriteByte
*函数功能：向STC8单片机EEPROM写入一个字节的数据
*参数列表：
*   addr
*       参数类型：unsigned int型数据
*       参数描述：要写入的数据的地址
*   dat
*       参数类型：unsigned char型数据
*       参数描述：要写入的数据
*返回值：unsigned char型数据，是否成功（成功：0；失败：1）
*////////////////////////////////////////////////////////////////////////////////////
unsigned char EEPROM_WriteByte(unsigned int addr, unsigned char dat);
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：EEPROM_EreasePage
*函数功能：擦除STC8单片机EEPROM中一页的数据
*参数列表：
*   addr
*       参数类型：unsigned int型数据
*       参数描述：要擦除的数据的地址（注意！！！系统擦除的是该地址所在的一页！！！并不是此地址后512个字节！！！）
*返回值：unsigned char型数据，是否成功（成功：0；失败：1）
*////////////////////////////////////////////////////////////////////////////////////
unsigned char EEPROM_EreasePage(unsigned int addr);
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：EEPROM_ReadBytes
*函数功能：从STC8单片机EEPROM读出一段数据
*参数列表：
*   addr
*       参数类型：unsigned int型数据
*       参数描述：要写入的数据的地址
*   *to
*       参数类型：unsigned char型指针
*       参数描述：存储读取出数据的位置
*   len
*       参数类型：unsigned int型数据
*       参数描述：要读取的数据的长度
*返回值：unsigned char型数据，是否成功（成功：0；失败：1）
*////////////////////////////////////////////////////////////////////////////////////
unsigned char EEPROM_ReadBytes(unsigned char addr, unsigned char *to, unsigned int len);
/*///////////////////////////////////////////////////////////////////////////////////
*函数名：EEPROM_WriteBytes
*函数功能：向STC8单片机EEPROM写入一段数据
*参数列表：
*   addr
*       参数类型：unsigned int型数据
*       参数描述：将写入的数据存储的地址
*   *dat
*       参数类型：unsigned char型指针
*       参数描述：要写入的数据的首地址
*   len
*       参数类型：unsigned int型数据
*       参数描述：要写入的字符串的长度
*返回值：unsigned char型数据，是否成功（成功：0；失败：1）
*////////////////////////////////////////////////////////////////////////////////////
unsigned char EEPROM_WriteBytes(unsigned char addr, unsigned char *dat, unsigned int len);
#endif