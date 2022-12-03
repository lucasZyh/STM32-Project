#include "stm32f10x.h"                  // Device header
#include "OLED.h" 
#include "Timer.h" 
#include "Encoder.h" 

int16_t speed;

int main(){
	OLED_Init();
	Timer_Init();
	Encoder_Init();
	
	OLED_ShowString(1,1,"SPEED:");
	while(1){ 
		//OLED_ShowNum(1,5,num,5);
		OLED_ShowSignedNum(2,1,speed,5);
	}
}

void TIM2_IRQHandler(void){
	//判断中断
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET){
		speed = Encoder_Get();
		//清除中断标志位
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
