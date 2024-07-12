#include "stm32f10x.h"                  // Device header
#include "LED.h"

/**
  * @brief  引脚初始化
  * @param  无
  * @retval 无
  */
void LED_Init(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_14|GPIO_Pin_15, Bit_SET);
}

/**
  * @brief  转换LED状态
  * @param  color LED颜色，1红色，2黄色，3绿色
  * @retval 无
  */
void LED_Switch(uint8_t color) {
	if (color == 1) {
		uint8_t state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)(state ^ 1));
	}
	else if (color == 2) {
		uint8_t state = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14);
		GPIO_WriteBit(GPIOC, GPIO_Pin_15, (BitAction)(state ^ 1));
	}
	else if (color == 3) {
		uint8_t state = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15);
		GPIO_WriteBit(GPIOC, GPIO_Pin_14, (BitAction)(state ^ 1));
	}
}

/**
  * @brief  打开LED
  * @param  color LED颜色，1红色，2黄色，3绿色
  * @retval 无
  */
void LED_Open(uint8_t color) {
	if (color == 1) GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
	else if (color == 2) GPIO_WriteBit(GPIOC, GPIO_Pin_15, Bit_RESET);
	else if (color == 3) GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_RESET);
}

/**
  * @brief  关闭LED
  * @param  color LED颜色，1红色，2黄色，3绿色
  * @retval 无
  */
void LED_Close(uint8_t color) {
	if (color == 1) GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
	else if (color == 2) GPIO_WriteBit(GPIOC, GPIO_Pin_15, Bit_SET);
	else if (color == 3) GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_SET);
}
