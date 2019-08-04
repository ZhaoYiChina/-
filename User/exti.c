#include "exti.h"

/************************************************************************************************
*�������ƣ�void EXTI_GPIO_Configuration(void)
*�������ܣ��԰���IO�ں�ʱ�ӵ�����
*�����������
*�����������
************************************************************************************************/
static void EXTI_GPIO_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ�ܻ���ʧ��APB2����ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//ʹ�ܻ���ʧ��APB2����ʱ��
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);//ѡ��GPIO�ܽ������ⲿ�ж���·
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);//ѡ��GPIO�ܽ������ⲿ�ж���·
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);//ѡ��GPIO�ܽ������ⲿ�ж���·
		
		EXTI_InitStructure.EXTI_Line = EXTI_Line12|EXTI_Line13|EXTI_Line14;//�ж���
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�����
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½���Ϊ�ж�����
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��
		EXTI_Init(&EXTI_InitStructure);//���ָ����EXTI��·�������������

}

/********************************************************************************************
*�������ƣ�void EXTI_NVIC_Configuration(void)
*�������ܣ����ⲿ�жϵ����ȼ���������
*�����������
*�����������
********************************************************************************************/
static void EXTI_NVIC_Configuration(void)
{
		NVIC_InitTypeDef   NVIC_InitStructure;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}

/********************************************************************************************
*�������ƣ�void EXTI_Config(void)
*�������ܣ����ⲿ�жϽ�������
*�����������
*�����������
********************************************************************************************/
void EXTI_Config(void)
{
		EXTI_GPIO_Configuration();
		EXTI_NVIC_Configuration();
}
