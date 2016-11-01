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
	����������STC8��Ƭ������1�ַ���ͨ��ģ��
	��ע������ʹ�÷�װ�õĺ������в���������Ҫʹ��ֱ�ӶԴ��ڽ��в�����
        ʹ�ø�ģ�飬����config.h�ж���UART_BUFF_MAX����Ϊ���ݻ���������󳤶ȡ�
            �� #define UART1_BUFF_MAX 64
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
*��������EEPROM_ReadByte
*�������ܣ���STC8��Ƭ��EEPROM��ȡһ���ֽڵ�����
*�����б�
*   addr
*       �������ͣ�unsigned int������
*       ����������Ҫ��ȡ�����ݵĵ�ַ
*����ֵ��unsigned char�����ݣ���ȡ��������
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��1��
*////////////////////////////////////////////////////////////////////////////////////
unsigned char EEPROM_ReadByte(unsigned int addr) 
{
	EEPROM_ConfigWaitTime();
	IAP_CMD = 0x01;				//EEPROM������
	IAP_ADDRL = addr & 0xFF;
	IAP_ADDRH = addr >> 8;
	IAP_TRIG = 0x5A;			//����ָ��
	IAP_TRIG = 0xA5;
	return IAP_DATA;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������EEPROM_WriteByte
*�������ܣ���STC8��Ƭ��EEPROMд��һ���ֽڵ�����
*�����б�
*   addr
*       �������ͣ�unsigned int������
*       ����������Ҫд������ݵĵ�ַ
*   dat
*       �������ͣ�unsigned int������
*       ����������Ҫд�������
*����ֵ��unsigned char�����ݣ��Ƿ�ɹ����ɹ���0��ʧ�ܣ�1��
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��1��
*////////////////////////////////////////////////////////////////////////////////////
unsigned char EEPROM_WriteByte(unsigned int addr, unsigned char dat) 
{
	EEPROM_ConfigWaitTime();
	IAP_CMD = 0x02;				//EEPROM������
	IAP_ADDRL = addr & 0xFF;
	IAP_ADDRH = addr >> 8;
	IAP_DATA = dat;
	IAP_TRIG = 0x5A;			//����ָ��
	IAP_TRIG = 0xA5;
	if(IAP_CMD & 0x10)
		return 1;
	else return 0;
}
