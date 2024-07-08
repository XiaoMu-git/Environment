#include "stm32f10x.h"
#include "Key.h"
#include "Delay.h"

void Key_Init(void) {
	// 启动时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 初始化端口
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;	// 上拉输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

uint8_t Key_Get_Num(void) {
	uint8_t KeyNum = 0;
	// 端口按下
	if (0 == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)) {
		// 消除抖动的影响
		Delay_ms(20);
		// 如果没有松开按键则等待按键松开
		while (0 == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)) {};
		Delay_ms(20);
		KeyNum = 1;
	}
	
	if (0 == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)) {
		// 消除抖动的影响
		Delay_ms(20);
		// 如果没有松开按键则等待按键松开
		while (0 == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)) {};
		Delay_ms(20);
		KeyNum = 2;
	}
	
	return KeyNum;
}
