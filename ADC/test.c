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
	����������STC8��Ƭ������1�ַ���ͨ��ģ����Գ���
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
void Timer0Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xCD;		//���ö�ʱ��ֵ
	TH0 = 0xD4;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
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