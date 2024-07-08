#include "stm32f10x.h"                  // Device header
#include "Buzzer.h"
#include "Delay.h"

// 只需在此更改引脚即可
#define BUZZER_GPIO		GPIOB
#define BUZZER_PIN		GPIO_Pin_10

void Buzzer_Init(void) {
	if (BUZZER_GPIO == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if (BUZZER_GPIO == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = BUZZER_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BUZZER_GPIO, &GPIO_InitStruct);
	
	GPIO_WriteBit(BUZZER_GPIO, BUZZER_PIN, Bit_SET);
}

void Buzzer_Reminder(void) {
	// 提醒
	GPIO_WriteBit(BUZZER_GPIO, BUZZER_PIN, Bit_RESET);
	Delay_ms(100);
	GPIO_WriteBit(BUZZER_GPIO, BUZZER_PIN, Bit_SET);
	Delay_ms(900);
}

void Buzzer_Warning(void) {
	// 警告
	GPIO_WriteBit(BUZZER_GPIO, BUZZER_PIN, Bit_RESET);
	Delay_ms(100);
	GPIO_WriteBit(BUZZER_GPIO, BUZZER_PIN, Bit_SET);
	Delay_ms(100);
	GPIO_WriteBit(BUZZER_GPIO, BUZZER_PIN, Bit_RESET);
	Delay_ms(100);
	GPIO_WriteBit(BUZZER_GPIO, BUZZER_PIN, Bit_SET);
	Delay_ms(700);
}

void Buzzer_Severe(void) {
	// 错误
	GPIO_WriteBit(BUZZER_GPIO, BUZZER_PIN, Bit_RESET);
	Delay_ms(500);
	GPIO_WriteBit(BUZZER_GPIO, BUZZER_PIN, Bit_SET);
	Delay_ms(500);
}
