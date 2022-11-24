#include "stm32f10x.h"                  // Device header
#include "OLED.h" 
#include "Encoder.h"

int16_t Num;

int main(){
	OLED_Init();
	Encoder_Init();
	OLED_ShowString(1,1,"Num:");
	while(1){
		Num = Num + CountSensor_Get();
		OLED_ShowSignedNum(1,5,Num,5);
	}
}
