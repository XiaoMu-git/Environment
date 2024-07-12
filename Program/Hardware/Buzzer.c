#include "stm32f10x.h"                  // Device header
#include "Buzzer.h"
#include "Delay.h"

/**
  * @brief  引脚初始化
  * @param  无
  * @retval 无
  */
void Buzzer_Init(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET);
}

/**
  * @brief  提醒 x----,-----
  * @param  time 警报时长
  * @retval 无
  */
void Buzzer_Reminder(uint8_t time) {
	while (time--) {
		GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET);
		Delay_ms(100);
		GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET);
		Delay_ms(900);
	}
}

/**
  * @brief  警告 x-x--,-----
  * @param  time 警报时长
  * @retval 无
  */
void Buzzer_Warning(uint8_t time) {
	while (time--) {
		GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET);
		Delay_ms(100);
		GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET);
		Delay_ms(100);
		GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET);
		Delay_ms(100);
		GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET);
		Delay_ms(700);
	}
}

/**
  * @brief  错误 xxxxx,-----
  * @param  time 警报时长
  * @retval 无
  */
void Buzzer_Severe(uint8_t time) {
	while (time--) {
		GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET);
		Delay_ms(500);
		GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET);
		Delay_ms(500);
	}
}
