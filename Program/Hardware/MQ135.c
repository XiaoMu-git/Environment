#include "stm32f10x.h"                  // Device header
#include "MQ135.h"
#include "MyADC.h"

uint16_t MQ_data;

void MQ135_Init(void) {
	MyADC_Init();
}

void MQ135_RecvData(void) {
	MQ_data = MyADC_data[0];
}
