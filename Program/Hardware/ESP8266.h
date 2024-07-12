#ifndef _ESP8266_H_
#define _ESP8266_H_

uint8_t ESP8266_SendCmd(char *cmd, char *resp);
void ESP8266_Init(void);
uint8_t ESP8266_SendData(char *attribute, float data);

#endif
