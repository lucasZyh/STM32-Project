#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "OLED.h" 
#include "Serial.h"
#include <stdio.h>

int main(){
	OLED_Init();
	Serial_Init();
	Serial_SendByte('f');
	
	uint8_t MyArray[] = {0x42, 0x43, 0x44, 0x45};
	Serial_SendArray(MyArray, 4);
	Serial_SendString("\r\n");
	Serial_SendString("Hello World!\r\n");
	
	Serial_SendNumber(2344,4);
	
	printf("Num=%d\r\n", 666);
	
	char String[100];
	sprintf(String, "Num=%d\r\n", 333);
	Serial_SendString(String);
	
	Serial_Printf("你好世界");
	while(1){
	}
}
