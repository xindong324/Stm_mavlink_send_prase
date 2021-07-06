#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "string.h"
#include "common/mavlink.h"


//ALIENTEK ̽����STM32F407������ ʵ��4
//����ͨ��ʵ�� -�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK


// TODO: ����12���շ��߼�����˳

// ����2��mavlink��Ϣ������1��mavlink������print����1����
mavlink_message_t msg_send;

/*
void praseBuf(u8* buf)
{
	double  px,py,pz;
	double roll,pitch,yaw;
	memcpy(&px,buf+3,sizeof(double));
	memcpy(&py,buf+3+8,sizeof(double));
	memcpy(&pz,buf+3+2*8,sizeof(double));
	memcpy(&roll,buf+3+24,sizeof(double));
	memcpy(&pitch,buf+3+32,sizeof(double));
	memcpy(&yaw,buf+3+40,sizeof(double));
	
	printf("data: x: %lf, y: %lf,z: %lf, roll: %lf,pitch: %lf, yaw: %lf\n",px,py,pz,roll,pitch,yaw);
}
*/


int main(void)
{ 
 
	u8 t;
	u8 len;	
	u8 buf[255];
	mavlink_vicon_position_estimate_t vicon_position_send;
	u16 times=0,i;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);		//��ʱ��ʼ�� 
	uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	uart2_init(115200);	//���ڳ�ʼ��������Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�  
	
	while(1)
	{
		u8 send_data = 0x09;		
		//USART_SendData(USART1,send_data);         //�򴮿�1��������
		
		if(USART_RX_STA&0x8000)
		{
			
		}
		else
		{
			times++;
			if (times%100==0)
			{
				// send mavlink vicon
				mavlink_vicon_position_estimate_t mocap_pos_send;
				mocap_pos_send.x = 0.2;
				mocap_pos_send.y = 0.3;
				mocap_pos_send.z = 0.4;
				mocap_pos_send.roll = 0.6;
				mocap_pos_send.pitch = 0.7;
				mocap_pos_send.yaw = 0.8;
				mavlink_msg_vicon_position_estimate_encode(9,201,&msg_send,&mocap_pos_send);
				len = mavlink_msg_to_send_buffer(buf, &msg_send);
				//send data
				for(t=0;t<len;t++)
				{
					USART_SendData(USART2, buf[t]);         //�򴮿�1��������
					while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
				}
			}
			if(times%50==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
			delay_ms(10);
		}
	}
}
