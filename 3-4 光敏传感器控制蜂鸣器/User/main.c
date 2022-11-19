#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "LightSensor.h" 
#include "Buzzer.h" 

int main()
{
	lightSensor_Init();
	Buzzer_Init();
	
	while(1)
	{
		if(lightSensor_Get() == 1)
		{
			Buzzer_on();
		}
		else
		{
			Buzzer_off();
		}
//		Buzzer_on();
//		Delay_ms(500);
//		Buzzer_off();
//		Delay_ms(500);
//		Buzzer_Turn();
//		Delay_ms(500);
//		Buzzer_Turn();
//		Delay_ms(500);
	}
}
