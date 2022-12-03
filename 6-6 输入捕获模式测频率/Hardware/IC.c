#include "stm32f10x.h"                  // Device header

void IC_Init(void)
{
	//开启时钟，TIM3是APB1的时钟外设
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	//配置GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//选择时基单元的时钟，可以不选，默认上电后选择内部时钟
	TIM_InternalClockConfig(TIM3);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//指定时钟分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数器模式
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;//ARR自动重装器的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;//PSC预分频器的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重复计数器的值，高级定时器使用
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	//配置捕获单元
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;//选择通道
	TIM_ICInitStructure.TIM_ICFilter = 0xF;//选择输入捕获滤波器
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//极性
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//选择分频器
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	//配置TRGI的触发源
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	
	//配置从模式
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	
	//启动定时器
	TIM_Cmd(TIM3,ENABLE);
}

uint32_t IC_GetFreq(void)
{
	return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}
