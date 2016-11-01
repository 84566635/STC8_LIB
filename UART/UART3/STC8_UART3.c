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
	����������STC8��Ƭ������3�ַ���ͨ��ģ��
	��ע������ʹ�÷�װ�õĺ������в���������Ҫʹ��ֱ�ӶԴ��ڽ��в�����
        ʹ�ø�ģ�飬����config.h�ж�UART3_RESIVE_BUFF_MAX����Ϊ���ݽ��ջ���������󳤶ȡ�
            �� #define UART3_RESIVE_BUFF_MAX 64
		ʹ�ø�ģ�飬����config.h�ж�UART3_SEND_BUFF_MAX����Ϊ���ݷ��ͻ���������󳤶ȡ�
            �� #define UART3_SEND_BUFF_MAX 64
        ʹ�ø�ģ�飬����config.h�ж���XTAL����Ϊ����Ƶ�ʣ���λ���׺��ȣ���
            �� #define XTAL 11.059200
*////////////////////////////////////////////////////////////////////////////////////////
#include<STC8.h>

#ifndef UART3_RESIVE_BUFF_MAX
#define UART3_RESIVE_BUFF_MAX 64
#endif // ���û�ж���BUFFMAX����Ĭ��Ϊ64

#ifndef UART3_SEND_BUFF_MAX
#define UART3_SEND_BUFF_MAX 64
#endif // ���û�ж���BUFFMAX����Ĭ��Ϊ64

#ifndef XTAL
#define XTAL 11.059200
#endif //���û�ж��徧��Ƶ�ʣ���Ĭ��Ϊ11.0592M����

extern void UART3_Action(unsigned char *dat, unsigned int len);
//�˺��������б�д�����������һ���ַ�����������Զ�����

unsigned char xdata UART3_ResiveBuff[UART3_RESIVE_BUFF_MAX];     	//����3���ջ�����
unsigned int UART3_ResiveBuffIndex = 0;      						//����3���ջ�������ǰλ��

unsigned char xdata UART3_SendBuffQueue[UART3_SEND_BUFF_MAX];     	//����3���ն���
unsigned int UART3_SendBuffQueue_IndexIn = 0;						//����3���ն��ж�βָ��
unsigned int UART3_SendBuffQueue_IndexOut = 0;      				//����3���ն��ж���ָ��

bit UART3_ResiveFlag;                        //����3������ɱ�־
bit UART3_ResiveStringEndFlag;               //����3�ַ�������ȫ����ɱ�־
bit UART3_ResiveStringFlag;                  //����3�ַ������ڽ��ձ�־
unsigned int UART3SendBuffer_GetStatu();
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART3_AddStringToSendBuffer
*�������ܣ���һ���ַ������봮��3���ͻ��������첽��
*�����б�
*   *dat
*       �������ͣ�unsigned char��ָ��
*       ����������Ҫ���͵��ַ������׵�ַ
*   len
*       �������ͣ�unsigned int������
*       ����������Ҫ���͵��ַ����ĳ���
*����ֵ��һ��bit�ͱ�����1Ϊʧ�ܡ�
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��2��
*////////////////////////////////////////////////////////////////////////////////////
bit UART3_AddStringToSendBuffer(unsigned char *dat, unsigned int len)
{
    unsigned int i = 0;	
	if(UART3SendBuffer_GetStatu() == 0)
	{
		i = 1;
		S3BUF = dat[0];
	}
    for(; i < len; i++)
    {
        UART3_SendBuffQueue[UART3_SendBuffQueue_IndexIn] = dat[i];
        UART3_SendBuffQueue_IndexIn++;
        if(UART3_SendBuffQueue_IndexIn >= UART3_SEND_BUFF_MAX)
            UART3_SendBuffQueue_IndexIn = 0;
        if(UART3_SendBuffQueue_IndexIn == UART3_SendBuffQueue_IndexOut)
            return 1;
    }
	return 0;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART3SendBuffer_Out
*�������ܣ��Ӵ���3���ͻ���������һ���ַ�
*�����б�
*   ��
*����ֵ��һ��unsigned char�ͱ������������ַ�
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��2��
*////////////////////////////////////////////////////////////////////////////////////
unsigned char UART3SendBuffer_Out()
{
    unsigned char temp;
	if(UART3SendBuffer_GetStatu() == 0)
		return 0;
	temp = UART3_SendBuffQueue[UART3_SendBuffQueue_IndexOut];
	UART3_SendBuffQueue_IndexOut++;
    if(UART3_SendBuffQueue_IndexOut >= UART3_SEND_BUFF_MAX)
            UART3_SendBuffQueue_IndexOut = 0;
	
	return temp;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART3SendBuffer_GetStatu
*�������ܣ���ȡ����3���ͻ��������пռ�
*�����б�
*   ��
*����ֵ��һ��unsigned int�ͱ���������3���ͻ��������пռ�
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��2��
*////////////////////////////////////////////////////////////////////////////////////
unsigned int UART3SendBuffer_GetStatu()
{
	if(UART3_SendBuffQueue_IndexIn > UART3_SendBuffQueue_IndexOut)
		return UART3_SendBuffQueue_IndexIn - UART3_SendBuffQueue_IndexOut;
	else if(UART3_SendBuffQueue_IndexIn < UART3_SendBuffQueue_IndexOut)
		return UART3_SendBuffQueue_IndexIn + UART3_SEND_BUFF_MAX - UART3_SendBuffQueue_IndexOut;
	else return 0;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART3_Conf
*�������ܣ�����STC8��Ƭ������3
*�����б�
*   baud
*       �������ͣ�unsigned long������
*       ����������Ҫ���õĲ�����
*   timer
*       �������ͣ�unsigned char������
*       ����������Ҫʹ�õ����ڲ��������ʵĶ�ʱ������ѡ��ʱ��2��3��
*		ע�⣺��Ҫ�������������ѡ�õĶ�ʱ��������ͻ�����ҽ�����ͬ���ڵĲ�����һ��ʱ�ɹ���һ����ʱ����
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��1��
*////////////////////////////////////////////////////////////////////////////////////
void UART3_Conf(unsigned long baud, unsigned char timer) 
{
	unsigned long temp;
	
	S3CON = 0x10;		//8λ����,�ɱ䲨����
	IE2 |= 0x08;		//ʹ�ܴ���3�ж�
	EA = 1;         	//ʹ�����ж�
	
	temp = 65536 - XTAL * 1000000 / 4 / baud;    //���㶨ʱ����ֵ
	if(timer == 3)
	{
WrongTimerChoosed:
		S3CON |= 0x40;		//����3ѡ��ʱ��3Ϊ�����ʷ�����
		T4T3M |= 0x02;		//��ʱ��3ʱ��ΪFosc,��1T
		T3L = temp & 0xFF;
		T3H = temp >> 8;
		T4T3M |= 0x08;		//������ʱ��3
	}
	else if(timer == 2)
	{
		S3CON &= 0xBF;		//����3ѡ��ʱ��2Ϊ�����ʷ�����
		AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
		T2L = temp & 0xFF;	//�趨��ʱ��ֵ
		T2H = temp >> 8;	//�趨��ʱ��ֵ
		AUXR |= 0x10;		//������ʱ��2
	}
	else goto WrongTimerChoosed; 
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART3_SendStringNow
*�������ܣ������򴮿�3����һ���ַ�����ͬ�������жϣ�
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
*���ڣ�2016��11��2��
*////////////////////////////////////////////////////////////////////////////////////
void UART3_SendStringNow(unsigned char *dat, unsigned int len)
{
	IE2 &= 0xF7;
	while(len)
	{
		len --;                     //ÿ����һλ�����ȼ�1
		S3BUF = *dat;               //����һλ����
		dat ++;                     //����ָ��������һλ
		while(!(S3CON & 0x02));    	//�ȴ����ڷ�����ɱ�־
		S3CON &= 0xFD;         		//��մ��ڷ�����ɱ�־
	}
	IE2 |= 0x08;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART3_Read
*�������ܣ�������3�ݴ������е����ݶ�ȡ������
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
unsigned int UART3_Read(unsigned char *to, unsigned int len)
{
	unsigned int i;
	if(UART3_ResiveBuffIndex < len)len = UART3_ResiveBuffIndex;   	//��ȡ����3��ǰ�������ݵ�λ��
	for(i = 0;i < len;i ++)                           				//�������ݵ�Ŀ������
	{
		*to = UART3_ResiveBuff[i];
		to ++;
	}
	UART3_ResiveBuffIndex = 0;                              		//��մ���3���ջ�������ǰλ��
	return len;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART3_Driver
*�������ܣ�����3ͨ�ż�غ���������ѭ���е��á�
*         ������յ��ַ��������Զ��������б�д��UART3_Action(unsigned char *dat,unsigned int len)
*�����б�
*   ��
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2014��12��9��
*////////////////////////////////////////////////////////////////////////////////////
void UART3_Driver()
{
	unsigned char xdata dat[UART3_RESIVE_BUFF_MAX];     //���������ݴ�����
	unsigned int len;                        			//���ݵĳ���
	if(UART3_ResiveStringEndFlag)            			//�������3���յ�һ���������ַ���
	{
		UART3_ResiveStringEndFlag = 0;   				//��ս�����ɱ�־
		len = UART3_Read(dat, UART3_RESIVE_BUFF_MAX);  	//�����ݴ�ԭ������������õ����ݵĳ���
		UART3_Action(dat, len);          				//�����û���д��UART_Action�����������յ������ݼ����ݳ�����Ϊ����
	}
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART3_RxMonitor
*�������ܣ�����3�ַ������ս����жϣ��ڶ�ʱ��0�жϺ����е���
*�����б�
*   ms
*       �������ͣ�unsigned char������
*       ������������ʱ����ʱʱ������λ��ms��
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2014��12��9��
*////////////////////////////////////////////////////////////////////////////////////
void UART3_RxMonitor(unsigned char ms)
{
	static unsigned char ms30 = 0;                   	//30�����ʱ
	static unsigned int UART3_BuffIndex_Backup;     	//����3�����ݴ�����λ�ñ���
	if(! UART3_ResiveStringFlag)return ;             	//�����ǰû���ڽ������ݣ�ֱ���˳�����
    ms30 += ms;                                      	//ÿһ�ζ�ʱ���жϣ���ʾʱ���ȥ�����ɺ���
	if(UART3_BuffIndex_Backup != UART3_ResiveBuffIndex) //�������3�����ݴ�����λ�ñ��ݲ����ڴ���3���ջ�������ǰλ�ã����յ���������λ��
	{
		UART3_BuffIndex_Backup = UART3_ResiveBuffIndex; //��¼����3��ǰ�Ľ��ջ�����λ��
		ms30 = 0;                                    	//��λ30�����ʱ
	}
	if(ms30 > 30)                                    	//30���뵽��
	{
		ms30 = 0;                                		//��λ30�����ʱ
		UART3_ResiveStringEndFlag = 1;           		//���ô���3�ַ�������ȫ����ɱ�־
		UART3_ResiveStringFlag = 0;              		//��մ���3�ַ������ڽ��ձ�־
	}
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART3_IOPortSwitch
*�������ܣ�����3���ܽ�ѡ��
*�����б�
*   choose
*       �������ͣ�unsigned char������
*       ����������Ҫѡ��Ĺ��ܽ�λ��(��ע�ⲻҪ���������ܵ����ŷ�����ͻ��
					ȡֵ	RxDλ��		TxDλ��
					0		P0.0		P0.1
					1		P5.0		P5.1
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��1��
*////////////////////////////////////////////////////////////////////////////////////
void UART3_IOPortSwitch(unsigned char choose)
{
	P_SW2 &= 0xFD;
	P_SW2 |= (choose << 1);
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������interrupt_UART3
*�������ܣ�����3�жϺ���
*�����б�
*   ��
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2014��12��9��
*////////////////////////////////////////////////////////////////////////////////////
void interrupt_UART3() interrupt 17
{
	if(S3CON & 0x02)                          				//�������3�������
	{
		S3CON &= 0xFD;                        				//���ϵͳ��־λ
		if(UART3SendBuffer_GetStatu() > 0)
			S3BUF = UART3SendBuffer_Out();
	}
	else if(S3CON & 0x01)                          			//�������3�������
	{
		S3CON &= 0xFE;                        				//���ϵͳ��־λ
		UART3_ResiveFlag = 1;                 				//���ô���3������ɱ�־
		UART3_ResiveBuff[UART3_ResiveBuffIndex] = S3BUF;   	//�����յ������ݷŵ��ݴ�����
		UART3_ResiveStringFlag = 1;           				//���ô���3�ַ������ڽ��ձ�־
		UART3_ResiveBuffIndex ++;                   		//����3���ջ�������ǰλ������
	}
}