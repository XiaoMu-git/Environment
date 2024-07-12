#ifndef _GY30_H_
#define _GY30_H_

extern uint16_t GY30_data;

void GY30_SCL_W(uint8_t value);
void GY30_SDA_W(uint8_t value);
uint8_t GY30_SDA_R(void);
void GY30_Start(void);
void GY30_Stop(void);
void GY30_SendByte(uint8_t byte);
uint8_t GY30_RecvByte(void);
void GY30_SendAck(uint8_t ack_bit);
uint8_t GY30_RecvAck(void);
void GY30_Init(void);
void GY30_RecvData(void);

#endif
