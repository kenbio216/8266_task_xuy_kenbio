/*
 * @Author: xuyang
 * @Date: 2024-04-25 09:39:14
 * @LastEditors: xuyang
 * @LastEditTime: 2024-05-09 09:50:46
 * @FilePath: \8266_task_xuy_kenbio\mqtt_8266_5_4\mqtt_8266_5_4.ino
 * @Description:
 *
 * Copyright (c) 2024 by xuyang, All Rights Reserved
 */
/* -------------------------------------------------------------------------- */
/*                               wifi以及mqtt服务器配置                              */
/* -------------------------------------------------------------------------- */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// WiFi
const char *ssid = "kenbio";        // Enter your WiFi name
const char *password = "123456xuy"; // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "192.168.43.75";
const char *mqtt_username = "xuy8266";
const char *mqtt_password = "ex123456";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
/* -------------------------------------------------------------------------- */
/*                                    硬件配置                                    */
/* -------------------------------------------------------------------------- */
const int led_red_pin = 0;
const int led_blue_pin = 2;
char msg[50]; // 用来存放ESP8266将要PUBLISH到服务器的消息

/* -------------------------------------------------------------------------- */
/*                                  任务调度器相关配置                                 */
/* -------------------------------------------------------------------------- */
uint8_t task_num;
typedef struct
{
    void (*task_func)(void);
    uint16_t rate_ms;
    uint32_t last_run;
} scheduler_task_t;

/* -------------------------------------------------------------------------- */
/*                                   任务函数定义                                   */
/* -------------------------------------------------------------------------- */
static void led_blink(void)
{
    // Serial.println("led task!");
}
static void mqtt_test(void)
{
    snprintf(msg, 10, "%d", analogRead(A0)); // 把光敏电阻的数值发送过去
    client.publish("xy_light_number", msg);  // 服务器能获取亮度传感器数值
}

/* -------------------------------------------------------------------------- */
/*                                  任务调度器函数定义                                 */
/* -------------------------------------------------------------------------- */

static scheduler_task_t scheduler_task[] =
    {
        {led_blink, 500, 0},
        {mqtt_test, 2000, 0}};

void Scheduler_run(void)
{
    for (int i = 0; i < task_num; i++)
    {
        uint32_t now_time = millis();
        if (now_time - scheduler_task[i].last_run >= scheduler_task[i].rate_ms)
        {
            scheduler_task[i].task_func();
            scheduler_task[i].last_run = now_time;
        }
    }
}

void Scheduler_init(void)
{
    task_num = sizeof(scheduler_task) / sizeof(scheduler_task_t);
}

/* -------------------------------------------------------------------------- */
/*                                  setup初始入口                                 */
/* -------------------------------------------------------------------------- */
void setup()
{
    /* --------------------------------- lcd初始化 --------------------------------- */
    lcd.init();
    lcd.backlight();
    lcd.clear();
    /* --------------------------------- 设置引脚模式 --------------------------------- */
    pinMode(led_red_pin, OUTPUT);
    pinMode(led_blue_pin, OUTPUT);
    pinMode(BUILTIN_LED, OUTPUT);
    pinMode(A0, INPUT); // 设置光敏电阻引脚为输入模式
    Scheduler_init();
    /* ----------------------------- 连接WiFi以及MQTT服务器 ---------------------------- */
    // Set software serial baud to 115200;
    Serial.begin(115200);
    // connecting to a WiFi network
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
    // connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected())
    {
        String client_id = "esp8266-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password))
        {
            Serial.println("Public emqx mqtt broker connected");
        }
        else
        {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
    /* ------------------------------ 配置MQTT服务器的订阅 ------------------------------ */

    client.subscribe("xy2_led_ctrl");     // 订阅灯的话题，服务器能控制板载led灯亮灭
    client.subscribe("xy2_remote_sound"); // 订阅超声波的话题，使其能够获取超声波的数值并显示在lcd上
}

/* -------------------------------------------------------------------------- */
/*                                   loop循环                                   */
/* -------------------------------------------------------------------------- */

void loop()
{
    client.loop();
    Scheduler_run();
}

/* ------------------------------ 定义MQTT订阅的回调函数 ----------------------------- */
void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }

    // 控制LED灯亮灭
    if (strcmp(topic, "xy2_led_ctrl") == 0)
    {
        if ((char)payload[0] == '1')
        {
            digitalWrite(BUILTIN_LED, LOW); // 打开LED灯
            Serial.println("成功读取到数值1，LED灯亮");
        }
        else if ((char)payload[0] == '0')
        {
            digitalWrite(BUILTIN_LED, HIGH); // 关闭LED灯
            Serial.println("成功读取到数值0，LED灯灭");
        }
    }

    // 显示超声波数值在LCD上
    if (strcmp(topic, "xy2_remote_sound") == 0)
    {
        payload[length] = '\0'; // 确保以空字符结尾
        String ultrasonicValue = String((char *)payload);
        lcd.clear();                   // 清空LCD屏幕
        lcd.setCursor(0, 0);           // 设置光标在第一行的开头
        lcd.print("The current dist"); // 打印消息的第一部分
        lcd.setCursor(0, 1);           // 设置光标在第二行的开头
        lcd.print("is ");
        lcd.print(ultrasonicValue);
        lcd.print(" cm       "); // 清理多余字符
    }

    Serial.println();
    Serial.println("-----------------------");
}
