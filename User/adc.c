#include "adc.h"

uint16_t ADCCnovertedValue[30];//ADCת����ֵ
/***********************************************************************************************
*�������ƣ�void ADC_GPIO_Configuration(void)
*�������ܣ�ADC��GPIO��ʱ������
*�����������
*�����������
***********************************************************************************************/
static void ADC_GPIO_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//ʹ��ADC1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��GPIOC
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//ʹ�ܻ���ʧ��AHB����ʱ��
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);//����ADCʱ�ӣ�ADCCLK��
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ADCѡ��ģ������ģʽ
		GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	
}

/***********************************************************************************************
*�������ƣ�void ADC_Mode_Configuration(void)
*�������ܣ�ADC��ģʽ����
*�����������
*�����������
***********************************************************************************************/
static void ADC_Mode_Configuration(void)
{
		ADC_InitTypeDef ADC_InitStructure;
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;//����ģʽ(��ͨ��)
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ģʽ
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ת��������������ⲿ��������
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC�����Ҷ���
		ADC_InitStructure.ADC_NbrOfChannel = 1;//����ת����ADC��ͨ����Ŀ,ȡֵ��ΧΪ1-16
		ADC_Init(ADC1,&ADC_InitStructure);//��ʼ������
		/*����ָ��ADC�Ĺ�����ͨ��,�������ǵ�ת��˳��Ͳ���ʱ��*/
		ADC_RegularChannelConfig(ADC1,ADC_Channel_10, 1, ADC_SampleTime_71Cycles5);//����ADC��������	
	
}

/***********************************************************************************************
*�������ƣ�void ADC_DMA_Configuration(void)
*�������ܣ�ADC�ģģͣ�����
*�����������
*�����������
***********************************************************************************************/
static void ADC_DMA_Configuration(void)
{
		DMA_InitTypeDef DMA_InitStructure;
		DMA_DeInit(DMA1_Channel1);//��DMA��ͨ��x�Ĵ�������Ϊȱʡֵ(DMA1 ͨ��1)
		DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_ADDRESS;//�������ַ
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCCnovertedValue;//�ڴ����ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//������Ϊ���ݴ������Դ
		DMA_InitStructure.DMA_BufferSize = 30;//ָ��DMAͨ����DMA����Ĵ�С,��λΪ���ݵ�λ
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�Ĵ�������
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ�Ĵ�������
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���ݿ��Ϊ16λ
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���ݿ��Ϊ16λ
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//������ѭ������ģʽ
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//�ǳ������ȼ�
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//���ڴ浽�ڴ洫��
		DMA_Init(DMA1_Channel1, &DMA_InitStructure);
//		DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
}

/*************************************************************
*�������ƣ�ADC_NVIC_Configuration(void)
*�������ܣ�ȷ��USART�жϵ����ȼ�
*���룺��
*�������
*��ע����
*************************************************************/
static void ADC_NVIC_Configuration(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}

/***********************************************************************************************
*�������ƣ�void ADC_STARTUP_Configuration(void)
*�������ܣ�ADC����������
*�����������
*�����������
***********************************************************************************************/
static void ADC_STARTUP_Configuration(void)
{
		ADC_DMACmd(ADC1,ENABLE);//ʹ�ܻ���ʧ��ָ����ADC��DMA����
		DMA_Cmd(DMA1_Channel1,ENABLE);//ʹ�ܻ���ʧ��ָ����ͨ��x(DMA1 ͨ��1)
		ADC_Cmd(ADC1,ENABLE);//ʹ��ָ����ADC
		ADC_ResetCalibration(ADC1);//��λУ��
		while(ADC_GetResetCalibrationStatus(ADC1));
		ADC_StartCalibration(ADC1);//��ʼУ��
		while(ADC_GetCalibrationStatus(ADC1));
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);//���ʹ��ADCת��

}
/***********************************************************************************************
*�������ƣ�void ADC_Config(void)
*�������ܣ�ADC����������
*�����������
*�����������
***********************************************************************************************/
void ADC_Config(void)
{
		ADC_GPIO_Configuration();
		ADC_Mode_Configuration();
		ADC_DMA_Configuration();
//		ADC_NVIC_Configuration();
		ADC_STARTUP_Configuration();
}

/********************************************************************************************
*�������ƣ�void ADC_WaitForConvertEnd(void)
*�������ܣ��ȴ�ADCת������
*�����������
*�����������
********************************************************************************************/
void ADC_WaitForConvertEnd(void)
{
		while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
		ADC_ClearFlag(ADC1,ADC_FLAG_EOC);//���ADCx�Ĵ������־λ
		while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));
		DMA_ClearFlag(DMA1_FLAG_TC1 );//���DMAͨ��x�������־λ(DMA1ͨ��1������ɱ�־λ)	
}



