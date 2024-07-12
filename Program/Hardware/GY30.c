#include "stm32f10x.h"                  // Device header
#include "GY30.h"
#include "Delay.h"

#define GY30_ADDRESS	0x46	// 外设地址
#define GY30_REGDATA	0x47	// 数据地址
#define GY30_POWERON	0x01	// 打开电源
#define GY30_HIGHMODE1	0x10	// 高分辨率模式1
#define GY30_HIGHMODE2	0x11	// 高分辨率模式2 
#define GY30_LOWMODE	0x13	// 低分辨率模式
#define GY30_PowDown	0x00	// 关闭模块

uint16_t GY30_data;

/**
  * @brief  设置SCL引脚电平
  * @param  value 需要设置的电平
  * @retval 无
  */
void GY30_SCL_W(uint8_t value) {
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)value);
	Delay_us(10);
}

/**
  * @brief  设置SDA引脚电平
  * @param  value 需要设置的电平
  * @retval 无
  */
void GY30_SDA_W(uint8_t value) {
	GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)value);
	Delay_us(10);
}

/**
  * @brief  获取SDA引脚的电平
  * @param  无
  * @retval value SDA引脚电平
  */
uint8_t GY30_SDA_R(void) {
	uint8_t value = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
	Delay_us(10);
	return value;
}

/**
  * @brief  发送开始信号
  * @param  无
  * @retval 无
  */
void GY30_Start(void) {
	GY30_SDA_W(1);
	GY30_SCL_W(1);
	GY30_SDA_W(0);
	GY30_SCL_W(0);
}

/**
  * @brief  发送停止信号
  * @param  无
  * @retval 无
  */
void GY30_Stop(void) {
	GY30_SDA_W(0);
	GY30_SCL_W(1);
	GY30_SDA_W(1);
}

/**
  * @brief  发送一个字节的数据
  * @param  byte 发送的数据
  * @retval 无
  */
void GY30_SendByte(uint8_t byte) {
	for (int i = 7; i >= 0; i--) {
		uint8_t value = (byte >> i) & 1;
		GY30_SDA_W(value);
		GY30_SCL_W(1);
		GY30_SCL_W(0);
	}
}

/**
  * @brief  接收一个字节的数据
  * @param  无
  * @retval byte 接收到的数据
  */
uint8_t GY30_RecvByte(void) {
	uint8_t byte = 0x00;
	GY30_SDA_W(1);
	for (int i = 7; i >= 0; i--) {
		GY30_SCL_W(1);
		if (GY30_SDA_R() == 1) byte += (1 << i);
		GY30_SCL_W(0);
	}
	return byte;
}

/**
  * @brief  发送应答位
  * @param  ack_bit 需要发送的应答
  * @retval 无
  */
void GY30_SendAck(uint8_t ack_bit) {
	GY30_SDA_W(ack_bit);
	GY30_SCL_W(1);
	GY30_SCL_W(0);
}

/**
  * @brief  接收应答位
  * @param  无
  * @retval ack_bit GY30的应答
  */
uint8_t GY30_RecvAck(void) {
	uint8_t ack_bit;
	GY30_SDA_W(1);
	GY30_SCL_W(1);
	ack_bit = GY30_SDA_R();
	GY30_SCL_W(0);
	return ack_bit;
}

/**
  * @brief  初始化引脚和配置GY30通信
  * @param  无
  * @retval 无
  */
void GY30_Init(void) {
	// 配置GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// 释放总线
	GPIO_SetBits(GPIOB, GPIO_Pin_5 | GPIO_Pin_6);
	
	// 开启设备
	GY30_Start();
	GY30_SendByte(GY30_ADDRESS);
	GY30_RecvAck();
	GY30_SendByte(GY30_POWERON);
	GY30_RecvAck();
	GY30_Stop();
	
	// 请求使用高精度模式1，检测数据
	GY30_Start();
	GY30_SendByte(GY30_ADDRESS);
	GY30_RecvAck();
	GY30_SendByte(GY30_HIGHMODE1);
	GY30_RecvAck();
	GY30_Stop();
}

/**
  * @brief  读取GY30中的光照强度
  * @param  无
  * @retval 无
  */
void GY30_RecvData(void) {
	GY30_Start();
	GY30_SendByte(GY30_REGDATA);
	GY30_RecvAck();
	GY30_data = (GY30_data << 8) + GY30_RecvByte();
	GY30_SendAck(0);
	GY30_data = (GY30_data << 8) + GY30_RecvByte();
	GY30_SendAck(0);
	GY30_Stop();
}
