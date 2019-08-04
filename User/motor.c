#include "motor.h"

uint16_t ARR_Val;
/***********************************************************************************************
*�������ƣ�void TIM4_GPIO_configuration(void)
*�������ܣ���ʱ��4��ʱ�ӻ�׼�����Լ�ʱ������
*�����������
*�����������
***********************************************************************************************/
static void TIM4_GPIO_configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ�ܻ���ʧ��APB1����ʱ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����������50MHz
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���

}
/***********************************************************************************************
*�������ƣ�void TIM4_Mode_configuration(void)
*�������ܣ���ʱ��4��ʱ�ӻ�׼�����Լ�ʱ������
*�����������
*�����������
***********************************************************************************************/
static void TIM4_Mode_configuration(void)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//ʹ�ܻ���ʧ��APB1����ʱ��
		
		TIM_TimeBaseStructure.TIM_Prescaler = 89;//Ԥ��Ƶֵ
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_Period = 800;//����һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x01;//�ظ�����ֵ
		TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//��ʼ��
		TIM_ARRPreloadConfig(TIM4, ENABLE);//ʹ�ܻ���ʧ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
		TIM_Cmd(TIM4, ENABLE);//ʹ�ܻ���ʧ��TIMx����
}

/***********************************************************************************************
*�������ƣ�void TIM4_PWM_configuration(void)
*�������ܣ���ʱ��4��PWM�������
*�����������
*�����������
***********************************************************************************************/
static void TIM4_PWM_configuration(void)
{
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 400;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC1Init(TIM4, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
		
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 400;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC2Init(TIM4, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
}


/***********************************************************************************************
*�������ƣ�void MOTOR_configuration(void)
*�������ܣ������ʼ������
*�����������
*�����������
***********************************************************************************************/
void MOTOR_configuration(void)
{
		TIM4_GPIO_configuration();
		TIM4_Mode_configuration();
		TIM4_PWM_configuration();
}


/***********************************************************************************************
*�������ƣ�void MOTOR_SetFrenquency(void)
*�������ܣ�PWMƵ�ʵ���
*���������Fren:��Ҫ��Ƶ��
*�����������
***********************************************************************************************/
void MOTOR_SetFrenquency(uint32_t Fren)
{		
		ARR_Val = 800000/Fren;
		TIM_SetAutoreload(TIM4, ARR_Val);	
}

/***********************************************************************************************
*�������ƣ�void MOTOR_Set1DutyRatio(int PWM1_Val)
*�������ܣ�����PWM1ռ�ձ�
*���������PWM1_Val:ռ�ձ�ֵ
*�����������
***********************************************************************************************/
void MOTOR_Set1DutyRatio(int PWM1_Val)
{
		TIM_SetCompare1(TIM4, PWM1_Val);//����TIM4����Ƚ�1�Ĵ���ֵ
}

/***********************************************************************************************
*�������ƣ�void MOTOR_Set2DutyRatio(int PWM2_Val)
*�������ܣ�����PWM2ռ�ձ�
*���������PWM2_Val:ռ�ձ�ֵ
*�����������
***********************************************************************************************/
void MOTOR_Set2DutyRatio(int PWM2_Val)
{
		TIM_SetCompare2(TIM4, PWM2_Val);//����TIM4����Ƚ�2�Ĵ���ֵ
}

/*******************************************************************************************
*�������ƣ�void MOTOR_ForWard(int PWM_Val)
*�������ܣ�ʵ�ֵ������ת(˳ʱ��)
*���������PWM_Val:�����ת�Ŀ���
*�����������
*******************************************************************************************/
void MOTOR_ForWard(int PWM_Val)
{
		MOTOR_Set1DutyRatio(0);
		MOTOR_Set2DutyRatio(PWM_Val);
}

/*******************************************************************************************
*�������ƣ�void MOTOR_BackWard(int PWM_Val)
*�������ܣ�ʵ�ֵ���ķ�ת(��ʱ��)
*���������PWM_Val:�����ת�Ŀ���
*�����������
*******************************************************************************************/
void MOTOR_BackWard(int PWM_Val)
{		
		MOTOR_Set1DutyRatio(PWM_Val);
		MOTOR_Set2DutyRatio(0);
}
