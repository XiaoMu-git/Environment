#ifndef _MYI2C_H_
#define _MYI2C_H_

void MyI2C_Init(void);
void MyI2C_SCL_W(uint8_t value);
void MyI2C_SDA_W(uint8_t value);
uint8_t MyI2C_SDA_R(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t byte);
uint8_t MyI2C_RecvByte(void);
void MyI2C_SendAck(uint8_t ack_bit);
uint8_t MyI2C_RecvAck(void);

#endif
