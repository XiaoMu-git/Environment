# 基于STM32的环境监测系统



## 一、功能设计

- 上传环境信息
  - 温度
    - temperature
    - 范围：-20 ~ 60

  - 湿度
    - humidity
    - 范围：5 ~ 95

  - 分贝
    - decibels

  - 空气质量
    - air_quality

  - 二氧化碳
    - carbon_dioxide

  - 光照强度
    - light_intensity

- 接收服务器命令
  - 开关蜂鸣器
    - set_buzzer

  - 开关LED灯
    - set_led

  - 设置参数补偿值
    - comp_temperature
    - comp_humidity
    - comp_decibels
    - comp_air_quality
    - comp_carbon_dioxide
    - comp_light_intensity

  - 设置信息上传间隔
    - upload_interval




## 二、外设选择

1. 主单片机：STM32F103C8T6
2. WIFI模块：ESP8266-01S
3. 温/湿度检测：DHT11
4. 空气质量检测：MQ-135
5. 光照强度检测：GY-30
6. 蜂鸣器：S8550蜂鸣器



## 三、连接报文

- 阿里云

  - 连接信息

    ```
    {
      "ProductKey": "k1icvBUirL7",
      "DeviceName": "stm32",
      "DeviceSecret": "b94fe303fa8210d9a51dd88b76a21ad8"
    }
    
    username: stm32&k1icvBUirL7
    password: 8798B47397B064A59E3237708E88EC1E6988E861
    ```

  - 配置ESP8266的MQTT客户端信息

    ```
    AT+MQTTUSERCFG=0,1,"stm32|securemode=2,signmethod=hmacsha1,timestamp=1687594902069|","stm32&k1icvBUirL7","b94fe303fa8210d9a51dd88b76a21ad8",0,0,""
    ```

  - 连接到阿里云平台

    ```
    AT+MQTTCONN=0,"k1icvBUirL7.iot-as-mqtt.cn-shanghai.aliyuncs.com",1883,0
    ```

  - 订阅主题，用来接收

    ```
    AT+MQTTSUB=0,"/sys/k1icvBUirL7/stm32/thing/service/property/set",0
    ```

  - 发布消息

    ```
    AT+MQTTPUB=0,"/sys/k1icvBUirL7/stm32/thing/event/property/post","{\"params\":{\"data1\":30,\"data2\":20},\"version\":\"1.0.0\"}",0,0
    ```

- EMQX代理

  - 连接信息

    ```
    username: emqx
    password: emqx_101
    ```

  - 配置ESP8266的MQTT客户端信息

    ```
    AT+MQTTUSERCFG=0,1,"stm32|SchoolServer","emqx","emqx_101",0,0,""
    ```

  - 连接到服务器

    ```
    AT+MQTTCONN=0,"192.168.205.176",1883,0
    ```

  - 订阅主题，用来接收

    ```
    AT+MQTTSUB=0,"/thing/stm32",0
    ```

  - 发布消息

    ```
    AT+MQTTPUB=0,"/thing/stm32","{\"params\":{\"data1\":30,\"data2\":20},\"version\":\"1.0.0\"}",0,0
    ```

    