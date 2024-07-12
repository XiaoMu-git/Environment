#include "stm32f10x.h"                  // Device header
#include  "DHT11.h"
#include  "Delay.h"

float DHT11_humi, DHT11_temp;

/**
  * @brief  引脚初始化，暂无实际作用，是为了与其他外设保持一致
  * @param  无
  * @retval 无
  */
void DHT11_Init(void) {
	
}

/**
  * @brief  GPIO输出模式
  * @param  无
  * @retval 无
  */
void DHT11_OutputMode(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * @brief  GPIO输出模式
  * @param  无
  * @retval 无
  */
void DHT11_InputMode(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * @brief  发送采集信息命令
  * @param  无
  * @retval 无
  */
void DHT11_Collect(void) {
	DHT11_OutputMode();	// 切换到输出模式
	
	// 发送采集命令
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
	Delay_us(30);
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	Delay_ms(20);
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
	Delay_us(30);
	
	DHT11_InputMode();	// 切换到输入模式
}

/**
  * @brief  以字节为单位接收
  * @param  无
  * @retval 无
  */
uint8_t DHT11_RecvByte(void) {
	unsigned char data;
	
	// 读取8位数据
	for(uint8_t i = 0; i < 8; i++) {
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);	// 等待高电平的数据
		Delay_us(30);	// 延迟30us是为了区别数据0和数据1，0只有26~28us
		
		data = (data << 1) + GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);	// 保存当前位
		
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 1 );	// 等待低电平的等待时间
	}
	
	return data;
}

/**
  * @brief  接收外设发送的数据
  * @param  无
  * @retval 无
  */
void DHT11_RecvData(void) {
	uint8_t humi_high, humi_low, temp_high, temp_low, check;
	
	DHT11_Collect();	// 发送采集信息命令
	
	// 外设回应下拉电平
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) {
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);	// 等待回应信号结束
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 1);	// 等待拉高延时结束
		
		// 接收数据
		humi_high = DHT11_RecvByte();
		humi_low = DHT11_RecvByte();
		temp_high = DHT11_RecvByte();
		temp_low = DHT11_RecvByte();
		check = DHT11_RecvByte();
		
		// 校验收到的数据
		if(humi_high + humi_low + temp_high + temp_low == check){
			DHT11_humi = humi_high + 0.1 * humi_low;
			DHT11_temp = temp_high + 0.1 * temp_low;
		}
	}
}
