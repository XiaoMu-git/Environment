#ifndef _USART1_H_
#define _USART1_H_

typedef struct USART_DataStruct {
	char data[1024];
	int size;
} USART_DataStruct;

extern USART_DataStruct usart1_data;

void USART1_Init(void);
void USART1_SendByte(uint8_t byte);
void USART1_SendString(char* str);
void USART1_Printf(char* format, ...);
void USART1_Clear(void);
uint8_t USART1_DataFlag(void);

#endif
