#include "mpu6050.h"
/*************************************************************************************
*�������ƣ�void USART2_GPIOConfig(void)
*�������ܣ�USART1ʱ�����ú�IO������
*�����������
*�����������
*************************************************************************************/
static void USART2_GPIOConfig(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ�ܻ���ʧ��APB2����ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//ʹ�ܻ���ʧ��APB2����ʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ�ܻ���ʧ��APB1����ʱ��
		//����USART1��Tx
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����������50MHz
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���
		//����USART1��Rx
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//�����������
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���	
}

/*************************************************************************************
*�������ƣ�void USART2_ModeConfig(void)
*�������ܣ�USART1ģʽ����
*�����������
*�����������
*************************************************************************************/
static void USART2_ModeConfig(void)
{
		USART_InitTypeDef USART_InitStructure;
		USART_InitStructure.USART_BaudRate = 115200;//������
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//��֡��β����1��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//��У��
		USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//���ͺͽ��ն�ʹ��
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ��������ʧ��
		USART_Init(USART2,&USART_InitStructure);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//ʹ�ܻ���ʧ��ָ����USART�ж�
		USART_Cmd(USART2, ENABLE);//ʹ�ܻ���ʧ��USART����
}

/*************************************************************************************
*�������ƣ�void UASRT2_NVIC_Configuration(void)
*�������ܣ�����2�����ж�����
*�����������
*�����������
*************************************************************************************/
static void UASRT2_NVIC_Configuration(void)
{
		NVIC_InitTypeDef  NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}

/*************************************************************************************
*�������ƣ�void MPU6050_Init(void)
*�������ܣ�MPU6050�ĳ�ʼ��
*�����������
*�����������
*************************************************************************************/
void MPU6050_Init(void)
{
		USART2_GPIOConfig();
		USART2_ModeConfig();
		UASRT2_NVIC_Configuration();
}

/*************************************************************************************
*�������ƣ�void MPU6050_RetToZero(void)
*�������ܣ�mpu6050��Z�����
*�����������
*�����������
*************************************************************************************/
void MPU6050_RetToZero(void)
{
		USART_SendData(USART2,0XFF);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);  //�ȴ�����2�������
		USART_SendData(USART2,0XAA);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);  //�ȴ�����2�������
		USART_SendData(USART2,0X52);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);  //�ȴ�����2�������
}

