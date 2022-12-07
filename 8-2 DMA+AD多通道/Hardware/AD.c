#include "stm32f10x.h"                  // Device header

uint16_t AD_Value[4];

void AD_Init(void){
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
	
	
	//初始化ADC
	ADC_InitTypeDef ADC_InitTypeDefstructure;
	ADC_InitTypeDefstructure.ADC_DataAlign = ADC_DataAlign_Right;//对齐方式
	ADC_InitTypeDefstructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//触发源选择
	ADC_InitTypeDefstructure.ADC_Mode = ADC_Mode_Independent;//配置ADC工作模式
	ADC_InitTypeDefstructure.ADC_ContinuousConvMode = ENABLE;//连续转换
	ADC_InitTypeDefstructure.ADC_ScanConvMode = ENABLE;//扫描转换
	ADC_InitTypeDefstructure.ADC_NbrOfChannel = 4;//通道数目
	
	ADC_Init(ADC1,&ADC_InitTypeDefstructure);
	
	  
	//初始化
	DMA_InitTypeDef DMA_InitStructure;
	//外设
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//起始地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//数据宽度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//是否自增
	//存储器
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;//起始地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//数据宽度
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//是否自增
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向
	DMA_InitStructure.DMA_BufferSize = 4;//缓存区大小，赋值给传输计数器
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//传输模式，是否自动重装
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//选择硬件触发还是软件触发
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//优先级
	
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
	//开启ADC到DMA输出
	ADC_DMACmd(ADC1, ENABLE);
	//开启ADC
	ADC_Cmd(ADC1,ENABLE);
	//校准ADC
	ADC_ResetCalibration(ADC1);//复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);//等待校准完成
	ADC_StartCalibration(ADC1);//复位校准
	while(ADC_GetCalibrationStatus(ADC1) == SET);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件触发转换
}

//void AD_GetValue(void){
//	
//	DMA_Cmd(DMA1_Channel1, DISABLE);
//	DMA_SetCurrDataCounter(DMA1_Channel1, 4);
//	DMA_Cmd(DMA1_Channel1, ENABLE);
//	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件触发转换
//	
//	//等待转运完成
//	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
//	//清理标志位
//	DMA_ClearFlag(DMA1_FLAG_TC1);
//}
