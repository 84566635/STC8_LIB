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

#ifndef UART1_BUFF_MAX
#define UART1_BUFF_MAX 64
#endif // ���û�ж���BUFFMAX����Ĭ��Ϊ64

#ifndef XTAL
#define XTAL 11.059200
#endif //���û�ж��徧��Ƶ�ʣ���Ĭ��Ϊ11.0592M����

extern void UART1_Action(unsigned char *dat, unsigned int len);
//�˺��������б�д�����������һ���ַ�����������Զ�����

unsigned char xdata UART1_Buff[UART1_BUFF_MAX];     //����1���ջ�����
unsigned int UART1_BuffIndex = 0;            //����1���ջ�������ǰλ��

bit UART1_SendFlag;                          //����1������ɱ�־
bit UART1_ResiveFlag;                        //����1������ɱ�־
bit UART1_ResiveStringEndFlag;               //����1�ַ�������ȫ����ɱ�־
bit UART1_ResiveStringFlag;                  //����1�ַ������ڽ��ձ�־

/*///////////////////////////////////////////////////////////////////////////////////
*��������UART1_Conf
*�������ܣ�����STC8��Ƭ������1
*�����б�
*   baud
*       �������ͣ�unsigned long������
*       ����������Ҫ���õĲ�����
*   timer
*       �������ͣ�unsigned char������
*       ����������Ҫʹ�õ����ڲ��������ʵĶ�ʱ������ѡ��1��2��
*		ע�⣺��Ҫ�������������ѡ�õĶ�ʱ��������ͻ�����ҽ�����ͬ���ڵĲ�����һ��ʱ�ɹ���һ����ʱ����
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��1��
*////////////////////////////////////////////////////////////////////////////////////
void UART1_Conf(unsigned long baud, unsigned char timer) 
{
	unsigned long temp;
	SCON = 0X50;    //���ô��ڹ���ģʽ
	temp = 65536 - XTAL * 1000000 / 4 / baud;    //���㶨ʱ����ֵ
	EA = 1;         //ʹ�����ж�
	ES = 1;         //ʹ�ܴ����ж�
	
	if(timer == 1)
	{
WrongTimerChoosed:
		AUXR |= 0x40;	//��ʱ��1ʱ��ΪFosc,��1T
		AUXR &= 0xFE;	//����1ѡ��ʱ��1Ϊ�����ʷ�����
		TMOD &= 0x0F;	//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ	
		TL1 = temp & 0xFF;
		TH1 = temp >> 8;
		TR1 = 1;        //ʹ�ܶ�ʱ��1
	}
	else if(timer == 2)
	{
		AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
		AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
		T2L = temp & 0xFF;
		T2H = temp >> 8;
		AUXR |= 0x10;		//������ʱ��2
	}
	else goto WrongTimerChoosed;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART1_SendString
*�������ܣ��򴮿�1����һ���ַ���
*�����б�
*   *dat
*       �������ͣ�unsigned char��ָ��
*       ����������Ҫ���͵��ַ������׵�ַ
*   len
*       �������ͣ�unsigned int������
*       ����������Ҫ���͵��ַ����ĳ���
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2014��12��9��
*////////////////////////////////////////////////////////////////////////////////////
void UART1_SendString(unsigned char *dat, unsigned int len)
{
	while(len)
	{
		len --;                     //ÿ����һλ�����ȼ�1
		SBUF = *dat;                //����һλ����
		dat ++;                     //����ָ��������һλ
		while(! UART1_SendFlag);    //�ȴ����ڷ�����ɱ�־
		UART1_SendFlag = 0;         //��մ��ڷ�����ɱ�־
	}
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART1_Read
*�������ܣ�������1�ݴ������е����ݶ�ȡ������
*�����б�
*   *to
*       �������ͣ�unsigned char��ָ��
*       �����������洢���յ����ַ���λ��
*   len
*       �������ͣ�unsigned int������
*       ����������Ҫ��ȡ���ַ����ĳ���
*����ֵ��unsigned int�����ݣ��ַ�����ʵ�ʳ���
*�汾��1.0
*���ߣ�������
*���ڣ�2014��12��9��
*////////////////////////////////////////////////////////////////////////////////////
unsigned int UART1_Read(unsigned char *to, unsigned int len)
{
	unsigned int i;
	if(UART1_BuffIndex < len)len = UART1_BuffIndex;   //��ȡ����1��ǰ�������ݵ�λ��
	for(i = 0;i < len;i ++)                           //�������ݵ�Ŀ������
		{
			*to = UART1_Buff[i];
			to ++;
		}
	UART1_BuffIndex = 0;                              //��մ���1���ջ�������ǰλ��
	return len;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART1_Driver
*�������ܣ�����1ͨ�ż�غ���������ѭ���е��á�
*         ������յ��ַ��������Զ��������б�д��UART1_Action(unsigned char *dat,unsigned int len)
*�����б�
*   ��
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2014��12��9��
*////////////////////////////////////////////////////////////////////////////////////
void UART1_Driver()
{
	unsigned char xdata dat[UART1_BUFF_MAX];       //���������ݴ�����
	unsigned int len;                        //���ݵĳ���
	if(UART1_ResiveStringEndFlag)            //�������1���յ�һ���������ַ���
		{
			UART1_ResiveStringEndFlag = 0;   //��ս�����ɱ�־
			len = UART1_Read(dat, UART1_BUFF_MAX);  //�����ݴ�ԭ������������õ����ݵĳ���
			UART1_Action(dat, len);          //�����û���д��UART_Action�����������յ������ݼ����ݳ�����Ϊ����
		}
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART1_RxMonitor
*�������ܣ�����1�ַ������ս����жϣ��ڶ�ʱ��0�жϺ����е���
*�����б�
*   ms
*       �������ͣ�unsigned char������
*       ������������ʱ����ʱʱ������λ��ms��
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2014��12��9��
*////////////////////////////////////////////////////////////////////////////////////
void UART1_RxMonitor(unsigned char ms)
{
	static unsigned char ms30 = 0;                   //30�����ʱ
	static unsigned int UART1_BuffIndex_Backup;     //����1�����ݴ�����λ�ñ���
	if(! UART1_ResiveStringFlag)return ;             //�����ǰû���ڽ������ݣ�ֱ���˳�����
    ms30 += ms;                                      //ÿһ�ζ�ʱ���жϣ���ʾʱ���ȥ�����ɺ���
	if(UART1_BuffIndex_Backup != UART1_BuffIndex)    //�������1�����ݴ�����λ�ñ��ݲ����ڴ���1���ջ�������ǰλ�ã����յ���������λ��
	{
		UART1_BuffIndex_Backup = UART1_BuffIndex;    //��¼����1��ǰ�Ľ��ջ�����λ��
		ms30 = 0;                                    //��λ30�����ʱ
	}
	if(ms30 > 30)                                    //30���뵽��
		{
			ms30 = 0;                                //��λ30�����ʱ
			UART1_ResiveStringEndFlag = 1;           //���ô���1�ַ�������ȫ����ɱ�־
			UART1_ResiveStringFlag = 0;              //��մ���1�ַ������ڽ��ձ�־
		}
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART1_IOPortSwitch
*�������ܣ�����1���ܽ�ѡ��
*�����б�
*   choose
*       �������ͣ�unsigned char������
*       ����������Ҫѡ��Ĺ��ܽ�λ��(��ע�ⲻҪ���������ܵ����ŷ�����ͻ��
					ȡֵ	RxDλ��		TxDλ��
					0		P3.0		P3.1
					1		P3.6		P3.7
					2		P1.6		P1.7
					3		P4.3		P4.4
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��1��
*////////////////////////////////////////////////////////////////////////////////////
void UART1_IOPortSwitch(unsigned char choose)
{
	P_SW1 &= 0x3F;
	P_SW1 |= (choose << 6);
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������interrupt_UART1
*�������ܣ�����1�жϺ���
*�����б�
*   ��
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2014��12��9��
*////////////////////////////////////////////////////////////////////////////////////
void interrupt_UART1() interrupt 4
{
	if(TI)                                    //�������1�������
	{
		TI = 0;                               //���ϵͳ��־λ
		UART1_SendFlag = 1;                   //���ô���1������ɱ�־
	}
	if(RI)                                    //�������1�������
	{
		RI = 0;                               //���ϵͳ��־λ
		UART1_ResiveFlag = 1;                 //���ô���1������ɱ�־
		UART1_Buff[UART1_BuffIndex] = SBUF;   //�����յ������ݷŵ��ݴ�����
		UART1_ResiveStringFlag = 1;           //���ô���1�ַ������ڽ��ձ�־
		UART1_BuffIndex ++;                   //����1���ջ�������ǰλ������
	}
}