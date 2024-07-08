#include "stm32f10x.h"                  // Device header
#include "LED.h"

#define LED_GPIO		GPIOC
#define LED_PIN			GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15
#define LED_GREEN		GPIO_Pin_13
#define LED_YELLOW		GPIO_Pin_14
#define LED_RED			GPIO_Pin_15

void LED_Init(void) {
	if (LED_GPIO == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if (LED_GPIO == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = LED_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_GPIO, &GPIO_InitStruct);
	
	GPIO_WriteBit(LED_GPIO, LED_PIN, Bit_SET);
}

void LED_Switch(uint8_t color) {
	if (color == 1) {
		uint8_t state = GPIO_ReadInputDataBit(LED_GPIO, LED_GREEN);
		GPIO_WriteBit(LED_GPIO, LED_GREEN, (BitAction)(state ^ 1));
	}
	else if (color == 2) {
		uint8_t state = GPIO_ReadInputDataBit(LED_GPIO, LED_YELLOW);
		GPIO_WriteBit(LED_GPIO, LED_YELLOW, (BitAction)(state ^ 1));
	}
	else if (color == 3) {
		uint8_t state = GPIO_ReadInputDataBit(LED_GPIO, LED_RED);
		GPIO_WriteBit(LED_GPIO, LED_RED, (BitAction)(state ^ 1));
	}
}

void LED_Open(uint8_t color) {
	if (color == 1) GPIO_WriteBit(LED_GPIO, LED_GREEN, Bit_RESET);
	else if (color == 2) GPIO_WriteBit(LED_GPIO, LED_YELLOW, Bit_RESET);
	else if (color == 3) GPIO_WriteBit(LED_GPIO, LED_RED, Bit_RESET);
}

void LED_Close(uint8_t color) {
	if (color == 1) GPIO_WriteBit(LED_GPIO, LED_GREEN, Bit_SET);
	else if (color == 2) GPIO_WriteBit(LED_GPIO, LED_YELLOW, Bit_SET);
	else if (color == 3) GPIO_WriteBit(LED_GPIO, LED_RED, Bit_SET);
}
