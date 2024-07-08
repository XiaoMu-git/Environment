#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "Delay.h"

#define MyI2C_GPIO 		GPIOB
#define MyI2C_SCL_PIN 	GPIO_Pin_5
#define MyI2C_SDA_PIN 	GPIO_Pin_6

void MyI2C_Init(void) {
	// 配置GPIO
	if (MyI2C_GPIO == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if (MyI2C_GPIO == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = MyI2C_SCL_PIN | MyI2C_SDA_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MyI2C_GPIO, &GPIO_InitStruct);
	
	// 释放总线
	GPIO_SetBits(MyI2C_GPIO, MyI2C_SCL_PIN | MyI2C_SDA_PIN);
}

void MyI2C_SCL_W(uint8_t value) {
	GPIO_WriteBit(MyI2C_GPIO, MyI2C_SCL_PIN, (BitAction)value);
	Delay_us(10);
}

void MyI2C_SDA_W(uint8_t value) {
	GPIO_WriteBit(MyI2C_GPIO, MyI2C_SDA_PIN, (BitAction)value);
	Delay_us(10);
}

uint8_t MyI2C_SDA_R(void) {
	uint8_t value = GPIO_ReadInputDataBit(MyI2C_GPIO, MyI2C_SDA_PIN);
	Delay_us(10);
	return value;
}

void MyI2C_Start(void) {
	MyI2C_SDA_W(1);
	MyI2C_SCL_W(1);
	MyI2C_SDA_W(0);
	MyI2C_SCL_W(0);
}

void MyI2C_Stop(void) {
	MyI2C_SDA_W(0);
	MyI2C_SCL_W(1);
	MyI2C_SDA_W(1);
}

void MyI2C_SendByte(uint8_t byte) {
	for (int i = 7; i >= 0; i--) {
		uint8_t value = (byte >> i) & 1;
		MyI2C_SDA_W(value);
		MyI2C_SCL_W(1);
		MyI2C_SCL_W(0);
	}
}

uint8_t MyI2C_RecvByte(void) {
	uint8_t byte = 0x00;
	MyI2C_SDA_W(1);
	for (int i = 7; i >= 0; i--) {
		MyI2C_SCL_W(1);
		if (MyI2C_SDA_R() == 1) byte += (1 << i);
		MyI2C_SCL_W(0);
	}
	return byte;
}

void MyI2C_SendAck(uint8_t ack_bit) {
	MyI2C_SDA_W(ack_bit);
	MyI2C_SCL_W(1);
	MyI2C_SCL_W(0);
}

uint8_t MyI2C_RecvAck(void) {
	uint8_t ack_bit;
	MyI2C_SDA_W(1);
	MyI2C_SCL_W(1);
	ack_bit = MyI2C_SDA_R();
	MyI2C_SCL_W(0);
	return ack_bit;
}
