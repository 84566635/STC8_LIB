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
	����������STC8��Ƭ��ADC����ģ��
*////////////////////////////////////////////////////////////////////////////////////////
#ifndef _STC8_ADC_H_
#define _STC8_ADC_H_

/*///////////////////////////////////////////////////////////////////////////////////
*��������ADC_Start
*�������ܣ�����STC8��Ƭ��ADC���жϷ����첽��
*�����б�
*   channel
*       �������ͣ�unsigned char������
*       ����������Ҫת����ͨ��
*			0:P1.0		1:P1.1		2:P1.2		3:P1.3
*			4:P1.4		5:P1.5		6:P1.6		7:P1.7
*			8:P0.0		9:P0.1		10:P0.2		11:P0.3
*			12:P0.4		13:P0.5		14:P0.6		15:P0.7
*   speed
*       �������ͣ�unsigned char������
*       ����������ADCת�����ٶȣ�ȡֵ0��15����ֵԽСԽ�졣���32ʱ��1��ת��������512ʱ��һ�Σ�
*////////////////////////////////////////////////////////////////////////////////////
void ADC_Start(unsigned char channel, unsigned char speed);
/*///////////////////////////////////////////////////////////////////////////////////
*��������ADC_Get
*�������ܣ���ȡSTC8��Ƭ��ADC���ֵ���жϷ����첽��
*����ֵ��unsigned int�����ݣ�ADCת���Ľ��
*////////////////////////////////////////////////////////////////////////////////////
unsigned int ADC_Get();
/*///////////////////////////////////////////////////////////////////////////////////
*��������ADC_Start
*�������ܣ�������STC8��Ƭ��ADC��ȡһ��ת���������ͨ���жϡ���ѯ����
*�����б�
*   channel
*       �������ͣ�unsigned char������
*       ����������Ҫת����ͨ��
*			0:P1.0		1:P1.1		2:P1.2		3:P1.3
*			4:P1.4		5:P1.5		6:P1.6		7:P1.7
*			8:P0.0		9:P0.1		10:P0.2		11:P0.3
*			12:P0.4		13:P0.5		14:P0.6		15:P0.7
*����ֵ��unsigned int�����ݣ�ADCת���Ľ��
*////////////////////////////////////////////////////////////////////////////////////
unsigned int ADC_GetNow(unsigned char channel);
/*///////////////////////////////////////////////////////////////////////////////////
*��������ADC_Driver
*�������ܣ�ADC��غ���������ѭ���е��á�
*         ���ADC�첽ת����ɣ����Զ��������б�д��ADC_Action(unsigned char channel, unsigned int result)
*////////////////////////////////////////////////////////////////////////////////////
void ADC_Driver();
#endif