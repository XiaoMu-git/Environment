#include "stm32f10x.h"                  // Device header
#include "ESP8266.h"
#include "USART1.h"
#include "Delay.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "stdbool.h"
#include "OLED.h"

/**
  * @brief  通过USART1向ESP8266发送AT命令
  * @param  cmd 需要发送的AT命令
  * @param  resp 需要需要接收的返回值
  * @retval 发送成功返回1 否则返回0
  */
uint8_t ESP8266_SendCmd(char *cmd, char *resp) {
	for (int i = 0; i < 5; i++) {
		USART1_SendString(cmd);
		for (int j = 0; j < 5; j++) {
			Delay_ms(200);
			if (strstr((const char *)usart1_data.data, resp) != NULL) {
				USART1_Clear();
				return 1;
			}
		}
		USART1_Clear();
	}
	return 0;
}

/**
  * @brief  调用USART1初始化、使用AT命令设置ESP8266的MQTT客户端
  * @param  ssid WiFi账号
  * @param  password WiFi密码
  * @retval 无
  */
void ESP8266_Init(void) {
	char cmd[256];
	USART1_Init();
	Delay_ms(1000);
	
	// 初始化端口
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;	// 上拉输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// 检测ESP8266是否连接
	OLED_ShowString(1, 1, "Init ESP8266... ");
	strcpy(cmd, "AT\r\n");
	if (ESP8266_SendCmd(cmd, "OK")) OLED_ShowString(1, 1, "Init OK         ");
	else OLED_ShowString(2, 1, "Init failed     ");
	
	// 重启ESP8266
	OLED_ShowString(1, 1, "Reset ESP8266...");
	strcpy(cmd, "AT+RST\r\n");
	if (ESP8266_SendCmd(cmd, "OK")) OLED_ShowString(1, 1, "Reset OK        ");
	else OLED_ShowString(2, 1, "Reset failed    ");
	
	// 设置WiFi模式
	OLED_ShowString(1, 1, "Set WiFi mode...");
	strcpy(cmd, "AT+CWMODE=1\r\n");
	if (ESP8266_SendCmd(cmd, "OK")) OLED_ShowString(1, 1, "WiFi mode OK    ");
	else OLED_ShowString(2, 1, "WiFi mode failed");
	
	// 连接WiFi
	OLED_ShowString(1, 1, "Connect WiFi... ");
	strcpy(cmd, "AT+CWJAP=\"XiaoMi_WiFi\",\"123/liumx\"\r\n");
	if (ESP8266_SendCmd(cmd, "OK")) OLED_ShowString(1, 1, "WiFi connected  ");
	else OLED_ShowString(2, 1, "WiFi conn failed");
	
	// 配置MQTT客户端信息
	OLED_ShowString(1, 1, "Config MQTT...  ");
	strcpy(cmd, "AT+MQTTUSERCFG=0,1,\"stm32|securemode=2\\,signmethod=hmacsha1\\,timestamp=1719628902694|\",\"stm32&k1icvBUirL7\",\"8798B47397B064A59E3237708E88EC1E6988E861\",0,0,\"\"\r\n");
	if (ESP8266_SendCmd(cmd, "OK")) OLED_ShowString(1, 1, "MQTT config OK  ");
	else OLED_ShowString(2, 1, "MQTT config fail");
	
	// 连接MQTT服务器
	OLED_ShowString(1, 1, "Connect MQTT... ");
	strcpy(cmd, "AT+MQTTCONN=0,\"k1icvBUirL7.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,0\r\n");
	if (ESP8266_SendCmd(cmd, "OK")) OLED_ShowString(1, 1, "MQTT connect OK ");
	else OLED_ShowString(2, 1, "MQTT conn failed");
	
	// 订阅MQTT主题
	OLED_ShowString(1, 1, "Subscribe...    ");
	strcpy(cmd, "AT+MQTTSUB=0,\"/sys/k1icvBUirL7/stm32/thing/service/property/set\",0\r\n");
	if (ESP8266_SendCmd(cmd, "OK")) OLED_ShowString(1, 1, "Subscribe OK    ");
	else OLED_ShowString(2, 1, "Subscribe failed");
	OLED_ShowString(1, 1, "                ");
	OLED_ShowString(2, 1, "                ");
}

/**
  * @brief  向服务器发送数据报文
  * @param  send_data 需要发送的数据结构体
  * @retval 成功返回1 失败返回0
  */
uint8_t ESP8266_SendData(char *attribute, float data) {
	for (int i = 0; i < 5; i++) {
		char *format = "AT+MQTTPUB=0,\"/sys/k1icvBUirL7/stm32/thing/event/property/post\",\"{\\\"params\\\":{\\\"%s\\\":%f}\\,\\\"version\\\":\\\"1.0.0\\\"}\",0,0\r\n";		
		USART1_Printf(format, attribute, data);
		for (int j = 0; j < 5; j++) {
			Delay_ms(200);
			if (strstr((const char *)usart1_data.data, "OK") != NULL) {
				USART1_Clear();
				return 1;
			}
		}
		USART1_Clear();
	}
	return 0;
}
