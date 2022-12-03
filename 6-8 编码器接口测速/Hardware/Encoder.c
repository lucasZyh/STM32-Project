#include "stm32f10x.h"                  // Device header

void Encoder_Init(void){
	//开启时钟，TIM3是APB1的时钟外设
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	//配置GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//编码器接口是一个带方向控制的外部时钟，所以内部时钟不需要
	
	//选择时基单元的时钟，可以不选，默认上电后选择内部时钟
	//TIM_InternalClockConfig(TIM3);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//指定时钟分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数器模式
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;//ARR自动重装器的值，满量程，容易转换成负数
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;//PSC预分频器的值，不分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重复计数器的值，高级定时器使用
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	//配置捕获单元
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	//结构体配置不完整，所以需要默认初始化
	//通道1
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;//选择通道
	TIM_ICInitStructure.TIM_ICFilter = 0xF;//选择输入捕获滤波器
	
	//后面仍会配置极性，后面的会覆盖前面的，可以删去
	//TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//极性，上升沿，不反向
	//这两项编码器用不到，可以删去，但是删去后结构体配置不完整，需要初始化一下
	//TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//选择分频器
	//TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	//配置通道2
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;//选择通道
	TIM_ICInitStructure.TIM_ICFilter = 0xF;//选择输入捕获滤波器
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	//配置编码器接口
    TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);	
	
	TIM_Cmd(TIM3,ENABLE);
}

int16_t Encoder_Get(void){
	int16_t temp;
	temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3,0);
	return temp;
}
