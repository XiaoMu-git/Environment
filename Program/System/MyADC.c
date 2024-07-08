#include "stm32f10x.h"                  // Device header
#include "MyADC.h"

uint16_t MyADC_data[16];

void MyADC_Init(void) {
	// 1.开启RCC时钟，ADC，GPIO，DMA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	// 设置ADC分频
	RCC_ADCCLKConfig(RCC_PCLK2_Div4);
	
	// 2.配置GPIO，模拟输入模式
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;	// 模拟输入模式，ADC专属模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// 3.配置多路开关，规则组
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	
	// 4.配置ADC数模转换器
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;	// 非连续扫描
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;	// 右对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	// 不从内部软件源触发
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;	// 独立转换，不使用ADC双模式
	ADC_InitStruct.ADC_NbrOfChannel = 1;	// 需要扫描的通道数量
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;	// 扫描模式
	ADC_Init(ADC1, &ADC_InitStruct);
	
	// 5.配置MDA
	// 外设站点
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	// 存储器站点
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)(MyADC_data);
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_BufferSize = 1;	// DMA自增的大小
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);
	
	// 6.DMA使能
	DMA_Cmd(DMA1_Channel1, ENABLE);
	// 开启ADC到DMA的输出
	ADC_DMACmd(ADC1, ENABLE);
	// ADC使能
	ADC_Cmd(ADC1, ENABLE);
	
	// 7.ADC校准
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
	
	// 8.触发开始
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
