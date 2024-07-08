#include "stm32f10x.h"                  // Device header
#include  "DHT11.h"
#include  "Delay.h"

// 只需在此更改引脚即可
#define DHT11_GPIO	GPIOB
#define DHT11_PIN	GPIO_Pin_1

//数据
uint8_t DHT11_data[4];

void DHT11_Init(void) {
	
}

// GPIO输出模式
void DHT11_OutputMode(void) {
	if (DHT11_GPIO == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if (DHT11_GPIO == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = DHT11_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11_GPIO, &GPIO_InitStruct);
}

// GPIO输入模式
void DHT11_InputMode(void) {
	if (DHT11_GPIO == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if (DHT11_GPIO == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = DHT11_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11_GPIO, &GPIO_InitStruct);
}

// 发送采集信息命令
void DHT11_Collect(void) {
	DHT11_OutputMode();	// 切换到输出模式
	
	// 发送采集命令
	GPIO_SetBits(DHT11_GPIO, DHT11_PIN);
	Delay_us(30);
	GPIO_ResetBits(DHT11_GPIO, DHT11_PIN);
	Delay_ms(20);
	GPIO_SetBits(DHT11_GPIO, DHT11_PIN);
	Delay_us(30);
	
	DHT11_InputMode();	// 切换到输入模式
}

// 以字节为单位接收
uint8_t DHT11_RecvByte(void) {
	unsigned char data;
	
	// 读取8位数据
	for(uint8_t i = 0; i < 8; i++) {
		while(GPIO_ReadInputDataBit(DHT11_GPIO, DHT11_PIN) == 0);	// 等待高电平的数据
		Delay_us(30);	// 延迟30us是为了区别数据0和数据1，0只有26~28us
		
		data = (data << 1) + GPIO_ReadInputDataBit(DHT11_GPIO, DHT11_PIN);	// 保存当前位
		
		while(GPIO_ReadInputDataBit(DHT11_GPIO, DHT11_PIN) == 1 );	// 等待低电平的等待时间
	}
	
	return data;
}

// 接收外设发送的数据
void DHT11_RecvData(void) {
	uint8_t RH, RL, TH, TL, CHECK;
	
	DHT11_Collect();	// 发送采集信息命令
	
	// 外设回应下拉电平
	if(GPIO_ReadInputDataBit(DHT11_GPIO, DHT11_PIN) == 0) {
		while(GPIO_ReadInputDataBit(DHT11_GPIO, DHT11_PIN) == 0);	// 等待回应信号结束
		while(GPIO_ReadInputDataBit(DHT11_GPIO, DHT11_PIN) == 1);	// 等待拉高延时结束
		
		// 接收数据
		RH = DHT11_RecvByte();
		RL = DHT11_RecvByte();
		TH = DHT11_RecvByte();
		TL = DHT11_RecvByte();
		CHECK = DHT11_RecvByte();
		
		// 校验收到的数据
		if(RH + RL + TH + TL == CHECK){
			DHT11_data[0] = RH;
			DHT11_data[1] = RL;
			DHT11_data[2] = TH;
			DHT11_data[3] = TL;
		}
	}
}
