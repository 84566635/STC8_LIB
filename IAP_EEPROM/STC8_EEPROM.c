/*//////////////////////////GPL��Դ���֤////////////////////////////////////////////////
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
	��Ȩ���У�C�� <2016>   <������>
	��һ����������������������������������������GNUͨ�ù������֤
	�������޸ĺ����·�����һ���������ʹ�����֤�ĵ����棬���ߣ�������
	��ѡ�����κθ��µİ汾��
	������һ�����Ŀ����ϣ�������ã���û���κε���������û���ʺ��ض�Ŀ��
	�������ĵ���������ϸ����������GNUͨ�ù������֤��
	��Ӧ���Ѿ��ͳ���һ���յ�һ��GNUͨ�ù������֤�ĸ����������û�У����
	��<http://www.gnu.org/licenses/>��
	���ߣ�������
	���䣺qwgg9654@gmail.com
		  568629794@qq.com
	GitHub��hxl9654
	����������STC8��Ƭ��EEPROM����ģ��
	��ע������ʹ�÷�װ�õĺ������в���������Ҫʹ��ֱ�ӶԴ��ڽ��в�����
        ʹ�ø�ģ�飬����config.h�ж���XTAL����Ϊ����Ƶ�ʣ���λ���׺��ȣ���
            �� #define XTAL 11.059200
*////////////////////////////////////////////////////////////////////////////////////////
#include<STC8.h>
#include<STC8_EEPROM.h>

#ifndef XTAL
#define XTAL 11.059200
#endif //���û�ж��徧��Ƶ�ʣ���Ĭ��Ϊ11.0592M����

/*///////////////////////////////////////////////////////////////////////////////////
*��������EEPROM_ConfigWaitTime
*�������ܣ�����STC8��Ƭ��EEPROM�ĵȴ�ʱ��
*�����б�
*   ��
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��1��
*////////////////////////////////////////////////////////////////////////////////////
void EEPROM_ConfigWaitTime()
{
	unsigned char xtal = XTAL;
	if(xtal >= 30)
		IAP_CONTR = 0x80;
	else if(xtal >= 24)
		IAP_CONTR = 0x81;
	else if(xtal >= 20)
		IAP_CONTR = 0x82;
	else if(xtal >= 12)
		IAP_CONTR = 0x83;
	else if(xtal >= 6)
		IAP_CONTR = 0x84;
	else if(xtal >= 3)
		IAP_CONTR = 0x85;
	else if(xtal >= 2)
		IAP_CONTR = 0x86;
	else
		IAP_CONTR = 0x87;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������EEPROM_ReadByte
*�������ܣ���STC8��Ƭ��EEPROM��ȡһ���ֽڵ�����
*�����б�
*   addr
*       �������ͣ�unsigned int������
*       ����������Ҫ��ȡ�����ݵĵ�ַ
*����ֵ��unsigned int�����ݣ���ȡ��������(����������Ϊ��8λ����ȡ����ʱ������0x2333
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��1��
*////////////////////////////////////////////////////////////////////////////////////
unsigned int EEPROM_ReadByte(unsigned int addr) 
{
	unsigned char dat = 0x00;
	EEPROM_ConfigWaitTime();
	IAP_CMD = 0x01;				//EEPROM������
	IAP_ADDRL = addr & 0xFF;
	IAP_ADDRH = addr >> 8;
	IAP_TRIG = 0x5A;			//����ָ��
	IAP_TRIG = 0xA5;
	dat = IAP_DATA;
	if(IAP_CONTR & 0x10)
	{
		IAP_CONTR &= 0xEF;
		return 0x2333;
	}
	else return dat;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������EEPROM_WriteByte
*�������ܣ���STC8��Ƭ��EEPROMд��һ���ֽڵ�����
*�����б�
*   addr
*       �������ͣ�unsigned int������
*       ����������Ҫд������ݵĵ�ַ
*   dat
*       �������ͣ�unsigned char������
*       ����������Ҫд�������
*����ֵ��unsigned char�����ݣ��Ƿ�ɹ����ɹ���0��ʧ�ܣ�1��
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��1��
*////////////////////////////////////////////////////////////////////////////////////
unsigned char EEPROM_WriteByte(unsigned int addr, unsigned char dat) 
{
	EEPROM_ConfigWaitTime();
	IAP_CMD = 0x02;				//EEPROMд����
	IAP_ADDRL = addr & 0xFF;
	IAP_ADDRH = addr >> 8;
	IAP_DATA = dat;
	EA = 0;
	IAP_TRIG = 0x5A;			//����ָ��
	IAP_TRIG = 0xA5;
	EA = 1;
	if(IAP_CONTR & 0x10)
	{
		IAP_CONTR &= 0xEF;
		return 1;
	}
	else return 0;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������EEPROM_EreasePage
*�������ܣ�����STC8��Ƭ��EEPROM��һҳ������
*�����б�
*   addr
*       �������ͣ�unsigned int������
*       ����������Ҫ���������ݵĵ�ַ��ע�⣡����ϵͳ�������Ǹõ�ַ���ڵ�һҳ�����������Ǵ˵�ַ��512���ֽڣ�������
*����ֵ��unsigned char�����ݣ��Ƿ�ɹ����ɹ���0��ʧ�ܣ�1��
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��1��
*////////////////////////////////////////////////////////////////////////////////////
unsigned char EEPROM_EreasePage(unsigned int addr) 
{
	EEPROM_ConfigWaitTime();
	IAP_CMD = 0x03;				//EEPROMд����
	IAP_ADDRL = addr & 0xFF;
	IAP_ADDRH = addr >> 8;
	EA = 0;
	IAP_TRIG = 0x5A;			//����ָ��
	IAP_TRIG = 0xA5;
	EA = 1;
	if(IAP_CONTR & 0x10)
	{
		IAP_CONTR &= 0xEF;
		return 1;
	}
	else return 0;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������EEPROM_ReadBytes
*�������ܣ���STC8��Ƭ��EEPROM����һ������
*�����б�
*   addr
*       �������ͣ�unsigned int������
*       ����������Ҫд������ݵĵ�ַ
*   *to
*       �������ͣ�unsigned char��ָ��
*       �����������洢��ȡ�����ݵ�λ��
*   len
*       �������ͣ�unsigned int������
*       ����������Ҫ��ȡ�����ݵĳ���
*����ֵ��unsigned char�����ݣ��Ƿ�ɹ����ɹ���0��ʧ�ܣ�1��
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��1��
*////////////////////////////////////////////////////////////////////////////////////
unsigned char EEPROM_ReadBytes(unsigned char addr, unsigned char *to, unsigned int len)
{
	unsigned int pointer;
	unsigned int temp;
	for(pointer = addr; pointer < addr + len; pointer++)
	{
		temp = EEPROM_ReadByte(pointer);
		if(temp <= 0xFF)
			to[pointer - addr] = temp;
		else return 1;
	}
	return 0;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������EEPROM_WriteBytes
*�������ܣ���STC8��Ƭ��EEPROMд��һ������
*�����б�
*   addr
*       �������ͣ�unsigned int������
*       ������������д������ݴ洢�ĵ�ַ
*   *dat
*       �������ͣ�unsigned char��ָ��
*       ����������Ҫд������ݵ��׵�ַ
*   len
*       �������ͣ�unsigned int������
*       ����������Ҫд����ַ����ĳ���
*����ֵ��unsigned char�����ݣ��Ƿ�ɹ����ɹ���0��ʧ�ܣ�1��
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��1��
*////////////////////////////////////////////////////////////////////////////////////
unsigned char EEPROM_WriteBytes(unsigned char addr, unsigned char *dat, unsigned int len)
{
	unsigned int pointer;
	unsigned int temp;
	for(pointer = addr; pointer < addr + len; pointer++)
	{
		temp = EEPROM_WriteByte(pointer, dat[pointer - addr]);
		if(temp == 1)
			return 1;
	}
	return 0;
}
