#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "OLED.h"
#include "DHT11.h"
#include "Delay.h"
#include "GY30.h"
#include "Buzzer.h"
#include "LED.h"
#include "MQ135.h"
#include "ESP8266.h"

/**
  * @brief  调用所有外设的初始化
  * @param  无
  * @retval 无
  */
void doInit() {
	DHT11_Init();		// 温湿度传感器
	GY30_Init();		// 光照强度传感器
	OLED_Init();  		// OLED显示
//  MQ135_Init();		// 空气质量传感器
 	ESP8266_Init();		// WiFi模块
	LED_Init();			// LED灯
	Buzzer_Init();		// 蜂鸣器
}

int main(void) {
	doInit();							// 调用所有外设的初始化
	
	LED_Open(3);
	Buzzer_Reminder(1);
	
	OLED_ShowString(1, 1, "sd:xx.xx");
	OLED_ShowString(2, 1, "wd:xx.xx");
	OLED_ShowString(3, 1, "gz:xxxxx");
	
	while(1) {
		// 等待外设检测数据
		Delay_ms(1000);
		
		// 获取最新数据
		DHT11_RecvData();
		GY30_RecvData();
		
		// OLED显示
		OLED_ShowNum(1, 4, (int)DHT11_humi, 2);
		OLED_ShowNum(1, 7, (int)(DHT11_humi * 100) % 100, 2);
		OLED_ShowNum(2, 4, (int)DHT11_temp, 2);
		OLED_ShowNum(2, 7, (int)(DHT11_temp * 100) % 100, 2);
		OLED_ShowNum(2, 4, (int)DHT11_temp, 2);
		OLED_ShowNum(2, 7, (int)(DHT11_temp * 100) % 100, 2);
		OLED_ShowNum(3, 4, (int)GY30_data, 5);
		
		// 将读取到的数据发送
		ESP8266_SendData("temperature", DHT11_temp);
		ESP8266_SendData("humidity", DHT11_humi);
		ESP8266_SendData("light_intensity", GY30_data);
		ESP8266_SendData("decibels", 0);
		ESP8266_SendData("air_quality", 0);
		ESP8266_SendData("carbon_dioxide", 0);
	}
}
