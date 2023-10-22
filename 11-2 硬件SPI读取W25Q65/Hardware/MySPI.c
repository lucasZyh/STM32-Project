#include "MySPI.h"

/**
  * @brief  引脚封装 SS引脚使用软件模拟
  * @param  
  * @retval 
  */
void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

/**
  * @brief  SPI初始化
  * @param  
  * @retval 
  */
void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	//初始化SPI外设
	SPI_InitTypeDef SPI_InitStruct;
	
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master; // 模式
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//全双工
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;// 8位
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//高位先行
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;//分频
	SPI_InitStruct.SPI_CPHA = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPOL = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;//软件NSS
	
	SPI_Init(SPI1,&SPI_InitStruct);
	SPI_Cmd(SPI1,ENABLE);
	
	MySPI_W_SS(1);//默认输出高电平，不做从机
}

/**
  * @brief  起始信号
  * @param  SS置低电平 
  * @retval 
  */
void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

/**
  * @brief  终止信号
  * @param  SS置高电平
  * @retval 
  */
void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}

/**
  * @brief  交互数据     模式0   SS下降沿->移出数据->SCK上升沿->移入数据->SCK下降沿->移出数据
  * @param  ByteSend     需要发送出去的数据
  * @retval ByteReceive  需要接收到的数据
  */
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
	
	//软件写入数据
	SPI_I2S_SendData(SPI1,ByteSend);
	
	//等待接收标志位RXNE
	while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET);
	
	//读取数据
	return SPI_I2S_ReceiveData(SPI1);
}
