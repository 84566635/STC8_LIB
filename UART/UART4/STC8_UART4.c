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
	����������STC8��Ƭ������4�ַ���ͨ��ģ��
	��ע������ʹ�÷�װ�õĺ������в���������Ҫʹ��ֱ�ӶԴ��ڽ��в�����
        ʹ�ø�ģ�飬����config.h�ж���UART_BUFF_MAX����Ϊ���ݻ���������󳤶ȡ�
            �� #define UART4_BUFF_MAX 64
        ʹ�ø�ģ�飬����config.h�ж���XTAL����Ϊ����Ƶ�ʣ���λ���׺��ȣ���
            �� #define XTAL 11.059200
*////////////////////////////////////////////////////////////////////////////////////////
#include<STC8.h>
#include<STC8_UART4.h>

#ifndef UART4_BUFF_MAX
#define UART4_BUFF_MAX 64
#endif // ���û�ж���BUFFMAX����Ĭ��Ϊ64

#ifndef XTAL
#define XTAL 11.059200
#endif //���û�ж��徧��Ƶ�ʣ���Ĭ��Ϊ11.0592M����

extern void UART4_Action(unsigned char *dat, unsigned char len);
//�˺��������б�д�����������һ���ַ�����������Զ�����

unsigned char xdata UART4_Buff[UART4_BUFF_MAX];     //����4���ջ�����
unsigned char UART4_BuffIndex = 0;           //����4���ջ�������ǰλ��

bit UART4_SendFlag;                          //����4������ɱ�־
bit UART4_ResiveFlag;                        //����4������ɱ�־
bit UART4_ResiveStringEndFlag;               //����4�ַ�������ȫ����ɱ�־
bit UART4_ResiveStringFlag;                  //����4�ַ������ڽ��ձ�־

/*///////////////////////////////////////////////////////////////////////////////////
*��������UART4_Conf
*�������ܣ�����STC8��Ƭ������4
*�����б�
*   baud
*       �������ͣ�unsigned long������
*       ����������Ҫ���õĲ�����
*   timer
*       �������ͣ�unsigned char������
*       ����������Ҫʹ�õ����ڲ��������ʵĶ�ʱ������ѡ��ʱ��2��4��
*		ע�⣺��Ҫ�������������ѡ�õĶ�ʱ��������ͻ�����ҽ�����ͬ���ڵĲ�����һ��ʱ�ɹ���һ����ʱ����
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��1��
*////////////////////////////////////////////////////////////////////////////////////
void UART4_Conf(unsigned long baud, unsigned char timer) 
{
	unsigned long temp;
	
	S4CON = 0x10;		//8λ����,�ɱ䲨����
	IE2 |= 0x10;		//ʹ�ܴ���4�ж�
	EA = 1;         	//ʹ�����ж�
	
	temp = 65536 - XTAL * 1000000 / 4 / baud;    //���㶨ʱ����ֵ
	if(timer == 4)
	{
WrongTimerChoosed:
		S4CON |= 0x40;		//����4ѡ��ʱ��4Ϊ�����ʷ�����
		T4T3M |= 0x20;		//��ʱ��4ʱ��ΪFosc,��1T
		T4L = temp & 0xFF;
		T4H = temp >> 8;
		T4T3M |= 0x80;		//������ʱ��4
	}
	else if(timer == 2)
	{
		S4CON &= 0xBF;		//����4ѡ��ʱ��2Ϊ�����ʷ�����
		AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
		T2L = temp & 0xFF;	//�趨��ʱ��ֵ
		T2H = temp >> 8;	//�趨��ʱ��ֵ
		AUXR |= 0x10;		//������ʱ��2
	}
	else goto WrongTimerChoosed; 
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART4_SendString
*�������ܣ��򴮿�4����һ���ַ���
*�����б�
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
void UART4_SendString(unsigned char *dat, unsigned char len)
{
	while(len)
	{
		len --;                     //ÿ����һλ�����ȼ�1
		S4BUF = *dat;               //����һλ����
		dat ++;                     //����ָ��������һλ
		while(! UART4_SendFlag);    //�ȴ����ڷ�����ɱ�־
		UART4_SendFlag = 0;         //��մ��ڷ�����ɱ�־
	}
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART4_Read
*�������ܣ�������4�ݴ������е����ݶ�ȡ������
*�����б�
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
unsigned char UART4_Read(unsigned char *to, unsigned char len)
{
	unsigned char i;
	if(UART4_BuffIndex < len)len = UART4_BuffIndex;   //��ȡ����4��ǰ�������ݵ�λ��
	for(i = 0;i < len;i ++)                           //�������ݵ�Ŀ������
		{
			*to = UART4_Buff[i];
			to ++;
		}
	UART4_BuffIndex = 0;                              //��մ���4���ջ�������ǰλ��
	return len;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART4_Driver
*�������ܣ�����4ͨ�ż�غ���������ѭ���е��á�
*         ������յ��ַ��������Զ��������б�д��UART4_Action(unsigned char *dat,unsigned char len)
*�����б�
*   ��
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2014��12��9��
*////////////////////////////////////////////////////////////////////////////////////
void UART4_Driver()
{
	unsigned char xdata dat[UART4_BUFF_MAX];       //���������ݴ�����
	unsigned char len;                       //���ݵĳ���
	if(UART4_ResiveStringEndFlag)            //�������4���յ�һ���������ַ���
		{
			UART4_ResiveStringEndFlag = 0;   //��ս�����ɱ�־
			len = UART4_Read(dat, UART4_BUFF_MAX);  //�����ݴ�ԭ������������õ����ݵĳ���
			UART4_Action(dat, len);          //�����û���д��UART_Action�����������յ������ݼ����ݳ�����Ϊ����
		}
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART4_RxMonitor
*�������ܣ�����4�ַ������ս����жϣ��ڶ�ʱ��0�жϺ����е���
*�����б�
*   ms
*       �������ͣ�unsigned char������
*       ������������ʱ����ʱʱ������λ��ms��
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2014��12��9��
*////////////////////////////////////////////////////////////////////////////////////
void UART4_RxMonitor(unsigned char ms)
{
	static unsigned char ms30 = 0;                   //30�����ʱ
	static unsigned char UART4_BuffIndex_Backup;     //����4�����ݴ�����λ�ñ���
	if(! UART4_ResiveStringFlag)return ;             //�����ǰû���ڽ������ݣ�ֱ���˳�����
    ms30 += ms;                                      //ÿһ�ζ�ʱ���жϣ���ʾʱ���ȥ�����ɺ���
	if(UART4_BuffIndex_Backup != UART4_BuffIndex)    //�������4�����ݴ�����λ�ñ��ݲ����ڴ���4���ջ�������ǰλ�ã����յ���������λ��
	{
		UART4_BuffIndex_Backup = UART4_BuffIndex;    //��¼����4��ǰ�Ľ��ջ�����λ��
		ms30 = 0;                                    //��λ30�����ʱ
	}
	if(ms30 > 30)                                    //30���뵽��
		{
			ms30 = 0;                                //��λ30�����ʱ
			UART4_ResiveStringEndFlag = 1;           //���ô���4�ַ�������ȫ����ɱ�־
			UART4_ResiveStringFlag = 0;              //��մ���4�ַ������ڽ��ձ�־
		}
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART4_IOPortSwitch
*�������ܣ�����4���ܽ�ѡ��
*�����б�
*   choose
*       �������ͣ�unsigned char������
*       ����������Ҫѡ��Ĺ��ܽ�λ��(��ע�ⲻҪ���������ܵ����ŷ�����ͻ��
					ȡֵ	RxDλ��		TxDλ��
					0		P0.2		P0.3
					1		P5.2		P5.3
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��1��
*////////////////////////////////////////////////////////////////////////////////////
void UART4_IOPortSwitch(unsigned char choose)
{
	P_SW2 &= 0xFB;
	P_SW2 |= (choose << 2);
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������interrupt_UART4
*�������ܣ�����4�жϺ���
*�����б�
*   ��
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2014��12��9��
*////////////////////////////////////////////////////////////////////////////////////
void interrupt_UART4() interrupt 18
{
	if(S4CON & 0x02)                          //�������4�������
	{
		S4CON &= 0xFD;                        //���ϵͳ��־λ
		UART4_SendFlag = 1;                   //���ô���4������ɱ�־
	}
	if(S4CON & 0x01)                          //�������4�������
	{
		S4CON &= 0xFE;                        //���ϵͳ��־λ
		UART4_ResiveFlag = 1;                 //���ô���4������ɱ�־
		UART4_Buff[UART4_BuffIndex] = S4BUF;  //�����յ������ݷŵ��ݴ�����
		UART4_ResiveStringFlag = 1;           //���ô���4�ַ������ڽ��ձ�־
		UART4_BuffIndex ++;                   //����4���ջ�������ǰλ������
	}
}