#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "PWM.h" 
#include "OLED.h" 
#include "IC.h"

uint8_t i;

int main(){
	OLED_Init();
	PWM_Init();
	IC_Init();
	
	OLED_ShowString(1,1,"Freq:00000HZ");
	PWM_SetCompare1(50);
	PWM_SetPrescaler(720-1); 
	
	while(1){
		OLED_ShowNum(1,6,IC_GetFreq(),5);
	}
}
