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
	����������STC8��Ƭ������2�ַ���ͨ��ģ��
	��ע������ʹ�÷�װ�õĺ������в���������Ҫʹ��ֱ�ӶԴ��ڽ��в�����
        ʹ�ø�ģ�飬����config.h�ж���UART_BUFF_MAX����Ϊ���ݻ���������󳤶ȡ�
            �� #define UART2_BUFF_MAX 64
        ʹ�ø�ģ�飬����config.h�ж���XTAL����Ϊ����Ƶ�ʣ���λ���׺��ȣ���
            �� #define XTAL 11.059200
*////////////////////////////////////////////////////////////////////////////////////////
#include<STC8.h>
#include<STC8_UART2.h>

#ifndef UART2_BUFF_MAX
#define UART2_BUFF_MAX 64
#endif // ���û�ж���BUFFMAX����Ĭ��Ϊ64

#ifndef XTAL
#define XTAL 11.059200
#endif //���û�ж��徧��Ƶ�ʣ���Ĭ��Ϊ11.0592M����

extern void UART2_Action(unsigned char *dat, unsigned char len);
//�˺��������б�д�����������һ���ַ�����������Զ�����

unsigned char xdata UART2_Buff[UART2_BUFF_MAX];     //����2���ջ�����
unsigned char UART2_BuffIndex = 0;           //����2���ջ�������ǰλ��

bit UART2_SendFlag;                          //����2������ɱ�־
bit UART2_ResiveFlag;                        //����2������ɱ�־
bit UART2_ResiveStringEndFlag;               //����2�ַ�������ȫ����ɱ�־
bit UART2_ResiveStringFlag;                  //����2�ַ������ڽ��ձ�־

/*///////////////////////////////////////////////////////////////////////////////////
*��������UART2_Conf
*�������ܣ�����STC8��Ƭ������2
*�����б��
*   baud
*       �������ͣ�unsigned long������
*       ����������Ҫ���õĲ�����
*   timer
*       �������ͣ�unsigned char������
*       ����������Ҫʹ�õ����ڲ��������ʵĶ�ʱ����ʵ����Ч������ѡ��ʱ��2���˴���˲�������һ���Կ��ǣ�
*		ע�⣺��Ҫ�������������ѡ�õĶ�ʱ��������ͻ�����ҽ�����ͬ���ڵĲ�����һ��ʱ�ɹ���һ����ʱ����
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��1��
*////////////////////////////////////////////////////////////////////////////////////
void UART2_Conf(unsigned long baud, unsigned char timer) 
{
	unsigned long temp;
	timer = 2;
	S2CON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	
	temp = 65536 - XTAL * 1000000 / 4 / baud;    //���㶨ʱ����ֵ
	T2L = temp & 0xFF;
	T2H = temp >> 8;
	
	IE2 |= 0x01;		//ʹ�ܴ���2�ж�
	EA = 1;         	//ʹ�����ж�
	AUXR |= 0x10;		//�����ʱ��2
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART2_SendString
*�������ܣ��򴮿�2����һ���ַ���
*�����б��
*   *dat
*       �������ͣ�unsigned char��ָ��
*       ����������Ҫ���͵��ַ������׵�ַ
*   len
*       �������ͣ�unsigned char������
*       ����������Ҫ���͵��ַ����ĳ���
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2014��12��9��
*////////////////////////////////////////////////////////////////////////////////////
void UART2_SendString(unsigned char *dat, unsigned char len)
{
	while(len)
	{
		len --;                     //ÿ����һλ�����ȼ�1
		S2BUF = *dat;               //����һλ����
		dat ++;                     //����ָ��������һλ
		while(! UART2_SendFlag);    //�ȴ����ڷ�����ɱ�־
		UART2_SendFlag = 0;         //��մ��ڷ�����ɱ�־
	}
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART2_Read
*�������ܣ�������2�ݴ������е����ݶ�ȡ������
*�����б��
*   *to
*       �������ͣ�unsigned char��ָ��
*       �����������洢���յ����ַ���λ��
*   len
*       �������ͣ�unsigned char������
*       ����������Ҫ��ȡ���ַ����ĳ���
*����ֵ��unsigned char�����ݣ��ַ�����ʵ�ʳ���
*�汾��1.0
*���ߣ�������
*���ڣ�2014��12��9��
*////////////////////////////////////////////////////////////////////////////////////
unsigned char UART2_Read(unsigned char *to, unsigned char len)
{
	unsigned char i;
	if(UART2_BuffIndex < len)len = UART2_BuffIndex;   //��ȡ����2��ǰ�������ݵ�λ��
	for(i = 0;i < len;i ++)                           //�������ݵ�Ŀ������
		{
			*to = UART2_Buff[i];
			to ++;
		}
	UART2_BuffIndex = 0;                              //��մ���2���ջ�������ǰλ��
	return len;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART2_Driver
*�������ܣ�����2ͨ�ż�غ���������ѭ���е��á�
*         ������յ��ַ��������Զ��������б�д��UART2_Action(unsigned char *dat,unsigned char len)
*�����б��
*   ��
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2014��12��9��
*////////////////////////////////////////////////////////////////////////////////////
void UART2_Driver()
{
	unsigned char xdata dat[UART2_BUFF_MAX];       //���������ݴ�����
	unsigned char len;                       //���ݵĳ���
	if(UART2_ResiveStringEndFlag)            //�������2���յ�һ���������ַ���
		{
			UART2_ResiveStringEndFlag = 0;   //��ս�����ɱ�־
			len = UART2_Read(dat, UART2_BUFF_MAX);  //�����ݴ�ԭ������������õ����ݵĳ���
			UART2_Action(dat, len);          //�����û���д��UART_Action�����������յ������ݼ����ݳ�����Ϊ����
		}
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART2_RxMonitor
*�������ܣ�����2�ַ������ս����жϣ��ڶ�ʱ��0�жϺ����е���
*�����б��
*   ms
*       �������ͣ�unsigned char������
*       ������������ʱ����ʱʱ������λ��ms��
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2014��12��9��
*////////////////////////////////////////////////////////////////////////////////////
void UART2_RxMonitor(unsigned char ms)
{
	static unsigned char ms30 = 0;                   //30�����ʱ
	static unsigned char UART2_BuffIndex_Backup;     //����2�����ݴ�����λ�ñ���
	if(! UART2_ResiveStringFlag)return ;             //�����ǰû���ڽ������ݣ�ֱ���˳�����
    ms30 += ms;                                      //ÿһ�ζ�ʱ���жϣ���ʾʱ���ȥ����ɺ���
	if(UART2_BuffIndex_Backup != UART2_BuffIndex)    //�������2�����ݴ�����λ�ñ��ݲ����ڴ���2���ջ�������ǰλ�ã����յ���������λ��
	{
		UART2_BuffIndex_Backup = UART2_BuffIndex;    //��¼����2��ǰ�Ľ��ջ�����λ��
		ms30 = 0;                                    //��λ30�����ʱ
	}
	if(ms30 > 30)                                    //30���뵽��
		{
			ms30 = 0;                                //��λ30�����ʱ
			UART2_ResiveStringEndFlag = 1;           //���ô���2�ַ�������ȫ����ɱ�־
			UART2_ResiveStringFlag = 0;              //��մ���2�ַ������ڽ��ձ�־
		}
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART2_IOPortSwitch
*�������ܣ�����2���ܽ�ѡ��
*�����б��
*   choose
*       �������ͣ�unsigned char������
*       ����������Ҫѡ��Ĺ��ܽ�λ��(��ע�ⲻҪ���������ܵ����ŷ�����ͻ��
					ȡֵ	RxDλ��		TxDλ��
					0		P1.0		P1.1
					1		P4.0		P4.2
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��1��
*////////////////////////////////////////////////////////////////////////////////////
void UART2_IOPortSwitch(unsigned char choose)
{
	P_SW2 &= 0xFE;
	P_SW2 |= choose;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������interrupt_UART2
*�������ܣ�����2�жϺ���
*�����б��
*   ��
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2014��12��9��
*////////////////////////////////////////////////////////////////////////////////////
void interrupt_UART2() interrupt 8
{
	if(S2CON & 0x02)                          //�������2�������
	{
		S2CON &= 0xFD;                        //���ϵͳ��־λ
		UART2_SendFlag = 1;                   //���ô���2������ɱ�־
	}
	if(S2CON & 0x01)                          //�������2�������
	{
		S2CON &= 0xFE;                        //���ϵͳ��־λ
		UART2_ResiveFlag = 1;                 //���ô���2������ɱ�־
		UART2_Buff[UART2_BuffIndex] = S2BUF;  //�����յ������ݷŵ��ݴ�����
		UART2_ResiveStringFlag = 1;           //���ô���2�ַ������ڽ��ձ�־
		UART2_BuffIndex ++;                   //����2���ջ�������ǰλ������
	}
}
