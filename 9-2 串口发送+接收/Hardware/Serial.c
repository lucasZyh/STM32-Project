#include "stm32f10x.h"        // Device header
#include <math.h>			  //数学函数库
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;

void Serial_Init(void)
{
	//开启时钟 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    	
	//初始化引脚
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//TX引脚，复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//RX引脚，浮空输入或上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//初始化USART
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = 9600;//设置比特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制（不适应）
	USART_InitStructure.USART_Mode = USART_Mode_Tx| USART_Mode_Rx;//串口模式（发送,接收）
	USART_InitStructure.USART_Parity = USART_Parity_No;//校验位（不校验）
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长
	
	USART_Init(USART1, &USART_InitStructure);
	
	//开启中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  发送一个字节
  * @param  
  * @retval 
  */
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//判断标志位
}

/**
  * @brief  发送一个数组
  * @param  
  * @retval 
  */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

/**
  * @brief  发送一个字符串
  * @param  
  * @retval 
  */
void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		Serial_SendByte(String[i]);
	}
}

/**
  * @brief  发送数字
  * @param  
  * @retval 
  */
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / (uint32_t)pow(10, Length - i - 1) % 10 + '0');//取出数字的每一位
	}
}

/**
  * @brief  重定向fputc
  * @param  
  * @retval 
  */
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

/**
  * @brief  封装sprintf
  * @param  
  * @retval 
  */
void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

/**
  * @brief  接收数据
  * @param  
  * @retval 
  */
uint8_t Serial_ReceiveData(void)
{	
	uint8_t RxData;
	if (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET)
	{
		RxData = USART_ReceiveData(USART1);
	}
	return RxData;
}

/**
  * @brief 获取标志位 
  * @param  
  * @retval 
  */
uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

/**
  * @brief  获取通过中断读取到的数据
  * @param  
  * @retval 
  */
uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}

/**
  * @brief  中断函数
  * @param  
  * @retval 
  */
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)//判断标志位
	{
		Serial_RxData = USART_ReceiveData(USART1);
		Serial_RxFlag = 1;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清楚标志位,如果读取DR会自动清除,这里也可以不清除
	}
}

