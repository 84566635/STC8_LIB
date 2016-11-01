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
        ʹ�ø�ģ�飬����config.h�ж�UART2_RESIVE_BUFF_MAX����Ϊ���ݽ��ջ���������󳤶ȡ�
            �� #define UART2_RESIVE_BUFF_MAX 64
		ʹ�ø�ģ�飬����config.h�ж�UART2_SEND_BUFF_MAX����Ϊ���ݷ��ͻ���������󳤶ȡ�
            �� #define UART2_SEND_BUFF_MAX 64
        ʹ�ø�ģ�飬����config.h�ж���XTAL����Ϊ����Ƶ�ʣ���λ���׺��ȣ���
            �� #define XTAL 11.059200
*////////////////////////////////////////////////////////////////////////////////////////
#include<STC8.h>

#ifndef UART2_RESIVE_BUFF_MAX
#define UART2_RESIVE_BUFF_MAX 64
#endif // ���û�ж���BUFFMAX����Ĭ��Ϊ64

#ifndef UART2_SEND_BUFF_MAX
#define UART2_SEND_BUFF_MAX 64
#endif // ���û�ж���BUFFMAX����Ĭ��Ϊ64

#ifndef XTAL
#define XTAL 11.059200
#endif //���û�ж��徧��Ƶ�ʣ���Ĭ��Ϊ11.0592M����

extern void UART2_Action(unsigned char *dat, unsigned int len);
//�˺��������б�д�����������һ���ַ�����������Զ�����

unsigned char xdata UART2_ResiveBuff[UART2_RESIVE_BUFF_MAX];     	//����2���ջ�����
unsigned int UART2_ResiveBuffIndex = 0;      						//����2���ջ�������ǰλ��

unsigned char xdata UART2_SendBuffQueue[UART2_SEND_BUFF_MAX];     	//����2���ն���
unsigned int UART2_SendBuffQueue_IndexIn = 0;						//����2���ն��ж�βָ��
unsigned int UART2_SendBuffQueue_IndexOut = 0;      				//����2���ն��ж���ָ��

bit UART2_ResiveFlag;                        //����2������ɱ�־
bit UART2_ResiveStringEndFlag;               //����2�ַ�������ȫ����ɱ�־
bit UART2_ResiveStringFlag;                  //����2�ַ������ڽ��ձ�־
unsigned int UART2SendBuffer_GetStatu();
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART2_AddStringToSendBuffer
*�������ܣ���һ���ַ������봮��2���ͻ��������첽��
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
bit UART2_AddStringToSendBuffer(unsigned char *dat, unsigned int len)
{
    unsigned int i = 0;	
	if(UART2SendBuffer_GetStatu() == 0)
	{
		i = 1;
		S2BUF = dat[0];
	}
    for(; i < len; i++)
    {
        UART2_SendBuffQueue[UART2_SendBuffQueue_IndexIn] = dat[i];
        UART2_SendBuffQueue_IndexIn++;
        if(UART2_SendBuffQueue_IndexIn >= UART2_SEND_BUFF_MAX)
            UART2_SendBuffQueue_IndexIn = 0;
        if(UART2_SendBuffQueue_IndexIn == UART2_SendBuffQueue_IndexOut)
            return 1;
    }
	return 0;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART2SendBuffer_Out
*�������ܣ��Ӵ���2���ͻ���������һ���ַ�
*�����б�
*   ��
*����ֵ��һ��unsigned char�ͱ������������ַ�
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��2��
*////////////////////////////////////////////////////////////////////////////////////
unsigned char UART2SendBuffer_Out()
{
    unsigned char temp;
	if(UART2SendBuffer_GetStatu() == 0)
		return 0;
	temp = UART2_SendBuffQueue[UART2_SendBuffQueue_IndexOut];
	UART2_SendBuffQueue_IndexOut++;
    if(UART2_SendBuffQueue_IndexOut >= UART2_SEND_BUFF_MAX)
            UART2_SendBuffQueue_IndexOut = 0;
	
	return temp;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART2SendBuffer_GetStatu
*�������ܣ���ȡ����2���ͻ��������пռ�
*�����б�
*   ��
*����ֵ��һ��unsigned int�ͱ���������2���ͻ��������пռ�
*�汾��1.0
*���ߣ�������
*���ڣ�2016��11��2��
*////////////////////////////////////////////////////////////////////////////////////
unsigned int UART2SendBuffer_GetStatu()
{
	if(UART2_SendBuffQueue_IndexIn > UART2_SendBuffQueue_IndexOut)
		return UART2_SendBuffQueue_IndexIn - UART2_SendBuffQueue_IndexOut;
	else if(UART2_SendBuffQueue_IndexIn < UART2_SendBuffQueue_IndexOut)
		return UART2_SendBuffQueue_IndexIn + UART2_SEND_BUFF_MAX - UART2_SendBuffQueue_IndexOut;
	else return 0;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART2_Conf
*�������ܣ�����STC8��Ƭ������2
*�����б�
*   baud
*       �������ͣ�unsigned long������
*       ����������Ҫ���õĲ�����
*   timer
*       �������ͣ�unsigned char������
*       ����������Ҫʹ�õ����ڲ��������ʵĶ�ʱ����ʵ����Ч������ѡ��ʱ��2���˴����˲�������һ���Կ��ǣ�
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
	AUXR |= 0x10;		//������ʱ��2
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART2_SendStringNow
*�������ܣ������򴮿�2����һ���ַ�����ͬ�������жϣ�
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
void UART2_SendStringNow(unsigned char *dat, unsigned int len)
{
	IE2 &= 0xFE;
	while(len)
	{
		len --;                     //ÿ����һλ�����ȼ�1
		S2BUF = *dat;               //����һλ����
		dat ++;                     //����ָ��������һλ
		while(!(S2CON & 0x02));    	//�ȴ����ڷ�����ɱ�־
		S2CON &= 0xFD;         		//��մ��ڷ�����ɱ�־
	}
	IE2 |= 0x01;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART2_Read
*�������ܣ�������2�ݴ������е����ݶ�ȡ������
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
unsigned int UART2_Read(unsigned char *to, unsigned int len)
{
	unsigned int i;
	if(UART2_ResiveBuffIndex < len)len = UART2_ResiveBuffIndex;   	//��ȡ����2��ǰ�������ݵ�λ��
	for(i = 0;i < len;i ++)                           				//�������ݵ�Ŀ������
	{
		*to = UART2_ResiveBuff[i];
		to ++;
	}
	UART2_ResiveBuffIndex = 0;                              		//��մ���2���ջ�������ǰλ��
	return len;
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART2_Driver
*�������ܣ�����2ͨ�ż�غ���������ѭ���е��á�
*         ������յ��ַ��������Զ��������б�д��UART2_Action(unsigned char *dat,unsigned int len)
*�����б�
*   ��
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2014��12��9��
*////////////////////////////////////////////////////////////////////////////////////
void UART2_Driver()
{
	unsigned char xdata dat[UART2_RESIVE_BUFF_MAX];     //���������ݴ�����
	unsigned int len;                        			//���ݵĳ���
	if(UART2_ResiveStringEndFlag)            			//�������2���յ�һ���������ַ���
	{
		UART2_ResiveStringEndFlag = 0;   				//��ս�����ɱ�־
		len = UART2_Read(dat, UART2_RESIVE_BUFF_MAX);  	//�����ݴ�ԭ������������õ����ݵĳ���
		UART2_Action(dat, len);          				//�����û���д��UART_Action�����������յ������ݼ����ݳ�����Ϊ����
	}
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART2_RxMonitor
*�������ܣ�����2�ַ������ս����жϣ��ڶ�ʱ��0�жϺ����е���
*�����б�
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
	static unsigned char ms30 = 0;                   	//30�����ʱ
	static unsigned int UART2_BuffIndex_Backup;     	//����2�����ݴ�����λ�ñ���
	if(! UART2_ResiveStringFlag)return ;             	//�����ǰû���ڽ������ݣ�ֱ���˳�����
    ms30 += ms;                                      	//ÿһ�ζ�ʱ���жϣ���ʾʱ���ȥ�����ɺ���
	if(UART2_BuffIndex_Backup != UART2_ResiveBuffIndex) //�������2�����ݴ�����λ�ñ��ݲ����ڴ���2���ջ�������ǰλ�ã����յ���������λ��
	{
		UART2_BuffIndex_Backup = UART2_ResiveBuffIndex; //��¼����2��ǰ�Ľ��ջ�����λ��
		ms30 = 0;                                    	//��λ30�����ʱ
	}
	if(ms30 > 30)                                    	//30���뵽��
	{
		ms30 = 0;                                		//��λ30�����ʱ
		UART2_ResiveStringEndFlag = 1;           		//���ô���2�ַ�������ȫ����ɱ�־
		UART2_ResiveStringFlag = 0;              		//��մ���2�ַ������ڽ��ձ�־
	}
}
/*///////////////////////////////////////////////////////////////////////////////////
*��������UART2_IOPortSwitch
*�������ܣ�����2���ܽ�ѡ��
*�����б�
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
*�����б�
*   ��
*����ֵ����
*�汾��1.0
*���ߣ�������
*���ڣ�2014��12��9��
*////////////////////////////////////////////////////////////////////////////////////
void interrupt_UART2() interrupt 8
{
	if(S2CON & 0x02)                          				//�������2�������
	{
		S2CON &= 0xFD;                        				//���ϵͳ��־λ
		if(UART2SendBuffer_GetStatu() > 0)
			S2BUF = UART2SendBuffer_Out();
	}
	else if(S2CON & 0x01)                          			//�������2�������
	{
		S2CON &= 0xFE;                        				//���ϵͳ��־λ
		UART2_ResiveFlag = 1;                 				//���ô���2������ɱ�־
		UART2_ResiveBuff[UART2_ResiveBuffIndex] = S2BUF;   	//�����յ������ݷŵ��ݴ�����
		UART2_ResiveStringFlag = 1;           				//���ô���2�ַ������ڽ��ձ�־
		UART2_ResiveBuffIndex ++;                   		//����2���ջ�������ǰλ������
	}
}