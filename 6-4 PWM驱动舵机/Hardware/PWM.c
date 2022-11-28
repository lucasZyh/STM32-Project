#include "stm32f10x.h"                  // Device header

void PWM_Init(void){
	
	//开启时钟，TIM2是APB1的时钟外设
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//配置GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//选择时基单元的时钟，可以不选，默认上电后选择内部时钟
	TIM_InternalClockConfig(TIM2);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//指定时钟分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数器模式
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;//ARR自动重装器的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;//PSC预分频器的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重复计数器的值，高级定时器使用
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	//初始化输出比较单元
	TIM_OCInitTypeDef TIM_OCInitStructure;
	//不论是否使用，都先赋一个初值，避免出现奇奇怪怪的错误。赋值后再更改需要的参数即可
	TIM_OCStructInit(&TIM_OCInitStructure);//结构体赋初始值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//设置输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//设置输出比较极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//设置输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;		//设置CCR
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	
	//启动定时器
	TIM_Cmd(TIM2,ENABLE);
	
}
void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2, Compare);
}
