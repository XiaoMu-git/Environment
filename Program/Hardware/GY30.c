#include "stm32f10x.h"                  // Device header
#include "GY30.h"
#include "Delay.h"
#include "MyI2C.h"

#define GY30_ADDRESS	0x46	// 外设地址
#define GY30_REGDATA	0x47	// 数据地址
#define GY30_POWERON	0x01	// 打开电源
#define GY30_HIGHMODE1	0x10	// 高分辨率模式1
#define GY30_HIGHMODE2	0x11	// 高分辨率模式2 
#define GY30_LOWMODE	0x13	// 低分辨率模式
#define GY30_PowDown	0x00	// 关闭模块

uint16_t GY30_data;

void GY30_Init(void) {
	MyI2C_Init();
	
	// 开启设备
	MyI2C_Start();
	MyI2C_SendByte(GY30_ADDRESS);
	MyI2C_RecvAck();
	MyI2C_SendByte(GY30_POWERON);
	MyI2C_RecvAck();
	MyI2C_Stop();
	
	// 请求使用高精度模式1，检测数据
	MyI2C_Start();
	MyI2C_SendByte(GY30_ADDRESS);
	MyI2C_RecvAck();
	MyI2C_SendByte(GY30_HIGHMODE1);
	MyI2C_RecvAck();
	MyI2C_Stop();
}

void GY30_RecvData(void) {
	// 读取检测到的数据
	MyI2C_Start();
	MyI2C_SendByte(GY30_REGDATA);
	MyI2C_RecvAck();
	GY30_data = (GY30_data << 8) + MyI2C_RecvByte();
	MyI2C_SendAck(0);
	GY30_data = (GY30_data << 8) + MyI2C_RecvByte();
	MyI2C_SendAck(0);
	MyI2C_Stop();
}
