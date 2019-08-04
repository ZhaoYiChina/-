#include "tim2.h"
/******************************************************************
*�������ƣ�void TIM2_Mode_Configuration(void)
*�������ܣ�����TIM2ʱ�ӣ����ҳ�ʼ��TIM2
*���룺��
*�������
*******************************************************************/
static void TIM2_Mode_Configuration(void)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//ʹ�ܶ�ʱ������ʱ��		
		TIM_TimeBaseStructure.TIM_Prescaler = 3599;//����Ԥ��Ƶֵ���м�Ҫ��1����Ϊ�����Զ���1
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;//���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_Period = 10000;//Ԥװ��Ϊ20000,1s���һ��
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//������ʱ�ӷָ����ʱ�ӷָ��ʱ�������
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//�������ظ�����
		TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//�Խṹ����г�ʼ��
		/*��Ҫ����һ����ϰ�ߣ���ʹ��һ���жϵ�ʱ���Ƚ������*/
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);//��ո��±�־λ
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//������ݸ����ж�
		TIM_ITConfig(TIM2, TIM_IT_Update,DISABLE);//ʹ�����ݸ����ж�
		TIM_Cmd(TIM2, ENABLE);//ʹ�ܶ�ʱ�����裬���õ�ÿһ������ʱ����Ҫʹ��
}

/******************************************************************
*�������ƣ�void TIM2_NVIC_Configuration(void)
*�������ܣ����ö�ʱ��2���ж����ȼ�
*���룺��
*�������
*******************************************************************/
static void TIM2_NVIC_Configuration(void)
{
		NVIC_InitTypeDef  NVIC_InitStructure;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

}
/******************************************************************
*�������ƣ�TIM_CompositeInit(void)
*�������ܣ���TIM2�������еĳ�ʼ������
*���룺��
*�������
*******************************************************************/
void TIM2_CompositeInit(void)
{
		TIM2_Mode_Configuration();
		TIM2_NVIC_Configuration();
}
