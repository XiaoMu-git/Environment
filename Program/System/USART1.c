#include "stm32f10x.h"                  // Device header
#include "USART1.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "Delay.h"

USART_DataStruct usart1_data;

/**
  * @brief  初始化引脚、串口1、配置
  * @param  无
  * @retval 无
  */
void USART1_Init(void) {
	// 1.配置GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// 2.配置USART
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);
	
	// 3.配置中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	// 4.USART使能
	USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  发送一个字节
  * @param  byte 需要发送的字节
  * @retval 无
  */
void USART1_SendByte(uint8_t byte) {
	USART_SendData(USART1, byte);	// 数据写入发送寄存器
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/**
  * @brief  发送字符串
  * @param  str 需要发送的字符串
  * @retval 无
  */
void USART1_SendString(char* str) {
	for (uint16_t i = 0; str[i] != '\0'; i++) USART1_SendByte(str[i]);
}

/**
  * @brief  使用字符串格式化发送数据
  * @param  format 需要发送的格式串
  * @param  ... 格式串中填充的变量
  * @retval 无
  */
void USART1_Printf(char* format, ...) {
	char str[256];
	va_list arg;
	va_start(arg, format);
	vsprintf(str, format, arg);
	va_end(arg);
	USART1_SendString(str);
}

/**
  * @brief  清空缓存区数据
  * @param  无
  * @retval 无
  */
void USART1_Clear(void) {
	memset(usart1_data.data, 0, sizeof(usart1_data.data));
	usart1_data.size = 0;
}

/**
  * @brief  检查是否现在可以读取
  * @param  无
  * @retval 可以返回1 不可以返回0
  */
uint8_t USART1_DataFlag(void) {
	uint8_t size = usart1_data.size;
	Delay_ms(10);
	return usart1_data.size == size && size > 0;
}

/**
  * @brief  USART1的中断函数
  * @param  无
  * @retval 无
  */
void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
		uint8_t recv_byte = USART_ReceiveData(USART1);
		if (usart1_data.size >=  1024) USART1_Clear();
		usart1_data.data[usart1_data.size++] = recv_byte;
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	// 清除中断
    }
}
