#include "stm32f10x_it.h"
#include <stdio.h>
#include "usart.h"
#include "systick.h"
#include "motor.h"
#include "pid.h"
#include "mpu6050.h"

void PID_Control(PID_Val *para,PID *pid,int currentAngle);

//---------���ڽ��գģͣ��ж�---------------
extern uint8_t parameter[48];
union dataConvert PAValue;
union dataConvert IAValue;
union dataConvert DAValue;
union dataConvert PBValue;
union dataConvert IBValue;
union dataConvert DBValue;
Acceleration  AccelerationMsg;
AngularSpeed  AngularSpeedMsg;
Angle AngleMsg;
PID_Val para;

void DMA1_Channel5_IRQHandler(void)  //
{
		uint8_t i;
		//�����־λ  
    DMA_ClearFlag(DMA1_FLAG_TC5); 	
		DMA_ClearITPendingBit(DMA1_IT_TC5);
		for(i=0;i<8;i++)
		{
				PAValue.table[i] = parameter[i];
		}
		for(i=0;i<8;i++)
		{
				IAValue.table[i] = parameter[8+i];
		}
		for(i=0;i<8;i++)
		{
				DAValue.table[i] = parameter[i+16];
		}
		for(i=0;i<8;i++)
		{
				PBValue.table[i] = parameter[i+24];
		}
		for(i=0;i<8;i++)
		{
				IBValue.table[i] = parameter[32+i];
		}
		for(i=0;i<8;i++)
		{
				DBValue.table[i] = parameter[i+40];
		}
		para.kpA = PAValue.num;para.kiA = IAValue.num*0.01;para.kdA = DAValue.num;
		para.kpB = PBValue.num;para.kiB = IBValue.num;para.kdB = DBValue.num;
}



//-----------------------TIM3�ж�---------------------------
extern PID pid;
extern uint16_t angleAve;
void TIM3_IRQHandler(void)
{
		if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update) == SET)
		{
				PID_Control(&para,&pid,angleAve);
		}
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);//���TIMx�Ĵ������־λ
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//���TIMx���жϴ�����λ
}

//------------------PID����-----------------------
void PID_Control(PID_Val *para,PID *pid,int currentAngle)
{
		int pid_out,error;
		if(currentAngle>pid->int_mubiao-300 && currentAngle<pid->int_mubiao+300)
		{
				error = currentAngle - pid->int_mubiao;
				if(error>-8 && error<8)
				{
						MOTOR_Set1DutyRatio(0);
						MOTOR_Set2DutyRatio(0);
						return;
				}
				else
				{
						pid_out = PIDCalculate(para,pid,currentAngle);
						if(pid_out>0)
						{
								if(pid_out>700)
									pid_out=700;
								MOTOR_ForWard(pid_out);
						}
						else
						{
								if(pid_out<-700)
									pid_out = -700;
								pid_out = pid_out*-1;
								MOTOR_BackWard(pid_out);
						}
			}
		}
		else
		{
				MOTOR_Set1DutyRatio(25);
				MOTOR_Set2DutyRatio(25);
		}
}

/***************************************************************************************
*�������ܣ�����2�жϣ����ڶ�ȡmpu6050�����ݣ�����mpu6050�����ݴ��
***************************************************************************************/
uint8_t RxBuffer[11];
uint8_t count = 0;
uint8_t Temp;
uint8_t flag = 0;
void USART2_IRQHandler(void)
{
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж���Ч,���������ݼĴ�����
		{
				Temp = USART_ReceiveData(USART2);//����USARTx������յ�������
				if((Temp == 0x55) && (count == 0))//�ж��ǲ��ǰ�ͷ
					flag = 1;
				if(flag == 1)//�����־λ����Ϊ�ͽ���
				{
						RxBuffer[count++] = Temp;
						if(count == 11)
						{
								flag = 0;
								count = 0;
								switch(RxBuffer[1])//���ݷ�������
								{
										case 0x51: //��ʶ������Ǽ��ٶȰ�
										{
												AccelerationMsg.X_AcceValue = ((short)(RxBuffer[3]<<8 | RxBuffer[2]))/32768.0*16;      //X����ٶ�
												AccelerationMsg.Y_AcceValue = ((short)(RxBuffer[5]<<8 | RxBuffer[4]))/32768.0*16;      //Y����ٶ�
												AccelerationMsg.Z_AcceValue = ((short)(RxBuffer[7]<<8 | RxBuffer[6]))/32768.0*16; 		 //Z����ٶ�												AccelerationMsg.Z_AcceValue -= 1.080566;
										}
										break;
										case 0x52: //��ʶ������ǽ��ٶȰ�
										{
												AngularSpeedMsg.X_AngSpeed = ((short)(RxBuffer[3]<<8| RxBuffer[2]))/32768.0*2000;      //X����ٶ�
												AngularSpeedMsg.Y_AngSpeed = ((short)(RxBuffer[5]<<8| RxBuffer[4]))/32768.0*2000;      //Y����ٶ�
												AngularSpeedMsg.Z_AngSpeed = ((short)(RxBuffer[7]<<8| RxBuffer[6]))/32768.0*2000;      //Z����ٶ�
												AngularSpeedMsg.Z_AngSpeed += 0.976563;//����
										}
										break;
										case 0x53: //��ʶ������ǽǶȰ�
										{
												AngleMsg.X_Angle = ((short)(RxBuffer[3]<<8| RxBuffer[2]))/32768.0*180;   //X���ת�ǣ�x �ᣩ
												AngleMsg.Y_Angle = ((short)(RxBuffer[5]<<8| RxBuffer[4]))/32768.0*180;   //Y�ḩ���ǣ�y �ᣩ
												AngleMsg.Z_Angle = ((short)(RxBuffer[7]<<8| RxBuffer[6]))/32768.0*180;   //Z��ƫ���ǣ�z �ᣩ
										}
										break;
										default: 
										break;
								}
								
						}
				}
				
		}
		USART_ClearFlag(USART2, USART_FLAG_RXNE);//���USARTx�Ĵ������־λ
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);//���USARTx���жϴ�����λ
}
/***************************************************************************************
*�������ܣ���ʱ��2���жϳ����������
***************************************************************************************/
typedef enum {BackWard = 0, Forward = 1} DirectionStatus;
DirectionStatus state=Forward;
void TIM2_IRQHandler(void)
{
		if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update) == SET)
		{
				if(state)
				{
						MOTOR_ForWard(176);
						state = !state;
				}
				else
				{
						MOTOR_BackWard(176);
						state = !state;
				}
				
		}
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIMx�Ĵ������־λ
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//���TIMx���жϴ�����λ
}
/***************************************************************************************
*�������ܣ��ⲿ�жϵ��жϷ������
***************************************************************************************/
extern uint16_t DealDate(uint16_t* table);
extern uint16_t ADCCnovertedValue[30];//ADCת����ֵ
void EXTI15_10_IRQHandler(void)
{
		if(EXTI_GetITStatus(EXTI_Line12) == SET)
		{
				delay_ms(10);
				if(EXTI_GetITStatus(EXTI_Line12) == SET)
				{
						EXTI_ClearITPendingBit(EXTI_Line12);//���EXTI��·����λ
						TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);//ʹ�����ݸ����ж�
				}
				
		}
		
		if( EXTI_GetITStatus(EXTI_Line13) == SET)
		{
				EXTI_ClearITPendingBit(EXTI_Line13);//���EXTI��·����λ
		}
		
		if( EXTI_GetITStatus(EXTI_Line14) == SET)
		{
				EXTI_ClearITPendingBit(EXTI_Line14);//���EXTI��·����λ
		}
}
