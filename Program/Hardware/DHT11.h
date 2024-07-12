#ifndef _DHT11_H_
#define _DHT11_H_

extern float DHT11_humi, DHT11_temp;

void DHT11_Init(void);
void DHT11_OutputMode(void);
void DHT11_InputMode(void);
void DHT11_Collect(void);
uint8_t DHT11_RecvByte(void);
void DHT11_RecvData(void);

#endif
