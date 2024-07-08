#include "stm32f10x.h"                  // Device header
#include "ESP8266.h"
#include "USART1.h"
#include "Delay.h"
#include "string.h"
#include "OLED.h"

uint8_t ESP8266_SendCmd(char *cmd, char *resp) {
	uint8_t times = 20;
	USART1_SendString(cmd);
	
	while (times--) {
		if (strstr((const char *)usart1_data.data, resp) != NULL) {
			USART1_Clear();
			return 1;
		}
		Delay_ms(100);
	}
	return 0;
}

void ESP8266_Init(void) {
	USART1_Init();

	while (!ESP8266_SendCmd("AT\r\n", "OK"));
	OLED_ShowString(1, 1, "<1>");
	
	while (!ESP8266_SendCmd("AT+GMR\r\n", "OK"));
	OLED_ShowString(1, 1, "<2>");
	
	while (!ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK"));
	OLED_ShowString(1, 1, "<3>");
	
	while (!ESP8266_SendCmd("AT+CWJAP=\"XiaoMi_WiFi\",\"123/liumx\"\r\n", "OK"));
//	while (!ESP8266_SendCmd("AT+CWJAP=\"bofanlegao\",\"88888888a\"\r\n", "OK"));
//	while (!ESP8266_SendCmd("AT+CWJAP=\"QDCU-Campus\",\"qdcu@123\"\r\n", "OK"));
	OLED_ShowString(1, 1, "<4>");
	
	while (!ESP8266_SendCmd("AT+MQTTUSERCFG=0,1,\"stm32|securemode=2\\,signmethod=hmacsha1\\,timestamp=1719628902694|\",\"stm32&k1icvBUirL7\",\"8798B47397B064A59E3237708E88EC1E6988E861\",0,0,\"\"\r\n", "OK"));
//	while (!ESP8266_SendCmd("AT+MQTTUSERCFG=0,1,\"stm32|MyServer\",\"emqx\",\"emqx_101\",0,0,\"\"\r\n", "OK"));
	OLED_ShowString(1, 1, "<5>");
	
	while (!ESP8266_SendCmd("AT+MQTTCONN=0,\"k1icvBUirL7.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,0\r\n", "OK"));
//	while (!ESP8266_SendCmd("AT+MQTTCONN=0,\"192.168.205.176\",1883,0\r\n", "OK"));
	OLED_ShowString(1, 1, "<6>");
	
	while (!ESP8266_SendCmd("AT+MQTTSUB=0,\"/sys/k1icvBUirL7/stm32/thing/service/property/set\",0\r\n", "OK"));
	OLED_ShowString(1, 1, "<7>");
	
	while (!ESP8266_SendCmd("AT+MQTTPUB=0,\"/sys/k1icvBUirL7/stm32/thing/event/property/post\",\"{\\\"params\\\":{\\\"wd\\\":35.2\\,\\\"sd\\\":55}\\,\\\"version\\\":\\\"1.0.0\\\"}\",0,0\r\n", "OK"));
	OLED_ShowString(1, 1, "<8>");
}

// wd：温度
// sd：湿度
uint8_t ESP8266_SendData(float wd, uint16_t sd, uint16_t gz, uint16_t kq) {
	uint8_t times = 20;
	char *format = "AT+MQTTPUB=0,\"/sys/k1icvBUirL7/stm32/thing/event/property/post\",\"{\\\"params\\\":{\\\"wd\\\":%f\\,\\\"sd\\\":%d\\,\\\"gz\\\":%d\\,\\\"kq\\\":%d}\\,\\\"version\\\":\\\"1.0.0\\\"}\",0,0\r\n";
	USART1_Printf(format, wd, sd, gz, kq);
	
	while (times--) {
		if (strstr((const char *)usart1_data.data, "OK") != NULL) {
			USART1_Clear();
			return 1;
		}
		Delay_ms(100);
	}
	return 0;
}
