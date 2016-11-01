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

#ifndef XTAL
#define XTAL 11.059200
#endif //���û�ж��徧��Ƶ�ʣ���Ĭ��Ϊ11.0592M����

#ifndef _STC8_EEPROM_H_
#define _STC8_EEPROM_H_

/*///////////////////////////////////////////////////////////////////////////////////
*��������EEPROM_ReadByte
*�������ܣ���STC8��Ƭ��EEPROM��ȡһ���ֽڵ�����
*�����б�
*   addr
*       �������ͣ�unsigned int������
*       ����������Ҫ��ȡ�����ݵĵ�ַ
*����ֵ��unsigned int�����ݣ���ȡ��������(����������Ϊ��8λ����ȡ����ʱ������0x2333
*////////////////////////////////////////////////////////////////////////////////////
unsigned int EEPROM_ReadByte(unsigned int addr);
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
*////////////////////////////////////////////////////////////////////////////////////
unsigned char EEPROM_WriteByte(unsigned int addr, unsigned char dat);
/*///////////////////////////////////////////////////////////////////////////////////
*��������EEPROM_EreasePage
*�������ܣ�����STC8��Ƭ��EEPROM��һҳ������
*�����б�
*   addr
*       �������ͣ�unsigned int������
*       ����������Ҫ���������ݵĵ�ַ��ע�⣡����ϵͳ�������Ǹõ�ַ���ڵ�һҳ�����������Ǵ˵�ַ��512���ֽڣ�������
*����ֵ��unsigned char�����ݣ��Ƿ�ɹ����ɹ���0��ʧ�ܣ�1��
*////////////////////////////////////////////////////////////////////////////////////
unsigned char EEPROM_EreasePage(unsigned int addr);
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
*////////////////////////////////////////////////////////////////////////////////////
unsigned char EEPROM_ReadBytes(unsigned char addr, unsigned char *to, unsigned int len);
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
*////////////////////////////////////////////////////////////////////////////////////
unsigned char EEPROM_WriteBytes(unsigned char addr, unsigned char *dat, unsigned int len);
#endif