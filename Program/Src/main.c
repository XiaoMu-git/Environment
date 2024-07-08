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

int main(void) {
	// 外设初始化
	LED_Init();
	LED_Open(2);
	OLED_Init();
	DHT11_Init();
	GY30_Init();
	Buzzer_Init();
	MQ135_Init();
 	ESP8266_Init();

	Buzzer_Reminder();
	LED_Close(2);
	LED_Open(1);
	
	OLED_ShowString(1, 1, "Humi:  00.00");
	OLED_ShowString(2, 1, "Temp:  00.00");
	OLED_ShowString(3, 1, "Light: 00.00");
	OLED_ShowString(4, 1, "Air:   00.00");
	
	while(1) {
		Delay_ms(1000);
		DHT11_RecvData();
		GY30_RecvData();
		MQ135_RecvData();
		
		OLED_ShowNum(1, 8, DHT11_data[0], 2);
		OLED_ShowNum(1, 11, DHT11_data[1], 1);
		
	    OLED_ShowNum(2, 8, DHT11_data[2] - 2, 2);
		OLED_ShowNum(2, 11, DHT11_data[3], 1);
		
	    OLED_ShowNum(3, 8, GY30_data, 5);
		
		float MQ135_voltage = 5.0 * MQ_data / 4096;
		OLED_ShowNum(4, 8, MQ135_voltage, 2);
		OLED_ShowNum(4, 11, (int)(MQ135_voltage * 100) % 100, 2);
		
		while (!ESP8266_SendData(DHT11_data[2] + 0.1 * DHT11_data[3] - 2, DHT11_data[0], GY30_data, MQ_data));
	}
}
