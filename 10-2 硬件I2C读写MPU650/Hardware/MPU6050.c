#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"
#include "Delay.h"

#define MPU6050_ADDRESS			0xD0

/**
  * @brief  超时退出等待
  * @param  
  * @retval 
  */
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
} 

/**
  * @brief  指定地址写
  * @param  
  * @retval 
  */
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	I2C_GenerateSTART(I2C2,ENABLE);//只发送数据，不管波形是否发送完毕 
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);//判断
	
	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//判断
	
	I2C_SendData(I2C2,RegAddress);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);//判断
	
	I2C_SendData(I2C2,Data);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);//判断
	
	I2C_GenerateSTOP(I2C2,ENABLE);
}

/**
  * @brief  指定地址读
  * @param  
  * @retval 
  */
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	I2C_GenerateSTART(I2C2,ENABLE);//只发送数据，不管波形是否发送完毕 
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);//判断
	
	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//判断
	
	I2C_SendData(I2C2,RegAddress);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);//判断
	
	//=================================================
	I2C_GenerateSTART(I2C2,ENABLE);//只发送数据，不管波形是否发送完毕 
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);//判断
	
	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Receiver);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);//判断
	
	//在接受1个字节的时候，要先置标志位，停止接收（停止不代表立即停止，当前波形结束后停止）
	//ACK = 0;STOP = 1
	I2C_AcknowledgeConfig(I2C2,DISABLE);
	I2C_GenerateSTOP(I2C2,ENABLE);
	
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);//判断
	Data = I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2,ENABLE);
	return Data;
}

/**
  * @brief  初始化
  * @param  
  * @retval 
  */
void MPU6050_Init(void)
{
	//硬件初始化I2C
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//复用开漏（硬件使用）
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStruct;
	
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;//模式
	
	I2C_InitStruct.I2C_ClockSpeed = 50000;//速度
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;//接受字节后给应答位
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//作为从机响应几位地址
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;//作为从机，自身地址
	I2C_Init(I2C2,&I2C_InitStruct);
	
	I2C_Cmd(I2C2,ENABLE);
	
	//解除睡眠,选择陀螺仪时钟,6个轴不待机,采样分频为10,滤波参数最大,陀螺仪和加速度计选最大量程
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);//电源管理寄存器1,选择陀螺仪时钟
	//Delay_ms(100);
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);//电源管理寄存器2
	//Delay_ms(100);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);//采样率分频,10分频
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);//配置寄存器,0000 0110
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);//陀螺仪配置寄存器, 0001 1000 
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);//加速度计配置寄存器
}

/**
  * @brief  
  * @param  
  * @retval 
  */
uint8_t MPU6050_ReadID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

/**
  * @brief  获取数据寄存器数据
  * @param  
  * @retval 
  */
void MPU6050_ReadData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
					  int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	//加速度
	*AccX = (MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H) << 8)| MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	
	*AccY = (MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H) << 8)
			| MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	
	*AccZ = (MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H) << 8)
			| MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	
	*GyroX = (MPU6050_ReadReg(MPU6050_GYRO_XOUT_H) << 8)
			| MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	
	*GyroY = (MPU6050_ReadReg(MPU6050_GYRO_YOUT_H) << 8)
			| MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	
	*GyroZ = (MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H) << 8)
			| MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
}



