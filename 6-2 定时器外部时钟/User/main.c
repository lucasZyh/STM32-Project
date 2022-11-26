#include "stm32f10x.h"                  // Device header
#include "OLED.h" 
#include "Timer.h" 

uint16_t num = 0;

int main(){
	OLED_Init();
	Timer_Init();
	OLED_ShowString(1,1,"Num:");
	OLED_ShowString(2,1,"CNT:");
	while(1){ 
		OLED_ShowNum(1,5,num,5);
		OLED_ShowNum(2,5,Timer_GetCount(),5);
	}
}

void TIM2_IRQHandler(void){
	//判断中断
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET){
		num++;
		//清除中断标志位
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
