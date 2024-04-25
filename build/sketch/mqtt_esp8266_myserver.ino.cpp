#include <Arduino.h>
#line 1 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_esp8266_myserver\\mqtt_esp8266_myserver.ino"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define KEY 0 // 令变量KEY为0，0对应ESP8266开发板上端口0

const char *ssid = "kenbio";           // 替换自己的WIFI
const char *password = "123456xuy";    // 替换自己的WIFI
const char *mqtt_server = "127.0.0.1"; // 替换自己的MQTT服务器IP地址

// 下面是一系列变量定义及初始化
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0; // 用来保存上一次接收TOPIC的时间
char msg[50];     // 用来存放ESP8266将要PUBLISH到服务器的消息
int value = 0;    // 这个变量假装自己是温度值，在后面的程序中每秒钟累计加1
int key_flag = 1; // 这个变量用来和KEY端口读过来的值做比较

// 让ESP8266开发板能够接入WiFi网络
#line 18 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_esp8266_myserver\\mqtt_esp8266_myserver.ino"
void setup_wifi();
#line 43 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_esp8266_myserver\\mqtt_esp8266_myserver.ino"
void callback(char *topic, byte *payload, unsigned int length);
#line 65 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_esp8266_myserver\\mqtt_esp8266_myserver.ino"
void reconnect();
#line 95 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_esp8266_myserver\\mqtt_esp8266_myserver.ino"
void setup();
#line 117 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_esp8266_myserver\\mqtt_esp8266_myserver.ino"
void loop();
#line 18 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_esp8266_myserver\\mqtt_esp8266_myserver.ino"
void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid); // 串口显示提示文字Connecting to ssid

  WiFi.begin(ssid, password); // 开始连接ssid网络

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  } // 如果未连接上WiFi，串口每半分钟打印一个.

  randomSeed(micros()); // 根据当前时间产生随机数种子

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // 如果连接上WiFi，串口显示WiFi connected以及ESP8266开发板的IP地址
}

// 处理MQTT服务器虚拟客户端PUBLISH的TOPIC
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] "); // 串口显示Message arrived [topic]，主题（topic）是led或者inTopic
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  } // payload是具体的消息，比如Hello World；length是消息长度；payload是个字符串，因此这里将字符串的每个字符依次打印出来
  Serial.println();
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1')
  {
    digitalWrite(BUILTIN_LED, LOW);
  } // 读取payload的第1个字符，如果这个字符是1，就开BUILTIN_LED灯
  if ((char)payload[0] == '0')
  {
    digitalWrite(BUILTIN_LED, HIGH);
  } // 读取payload的第1个字符，如果这个字符是0，就关BUILTIN_LED灯
}

// 让ESP8266开发板能够接入MQTT服务器
void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection..."); // 如果ESP8266开发板还没有连上MQTT服务器，串口输出Attempting MQTT connection...
    // Create a random client ID
    String clientId = "ESP8266Client-"; // 给ESP8266开发板随机生成一个客户端ID号，在它连上MQTT服务器后，我们在MQTT服务器网页上能够查看到
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected"); // 如果ESP8266开发板连上了MQTT服务器，串口输出connected
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world"); // 同时ESP8266开发板向MQTT服务器PUBLISH一个主题为outTopic内容为hello world的消息
      // ... and resubscribe
      client.subscribe("inTopic"); // 同时ESP8266开发板从MQTT服务器SUBSCRIBE一个主题inTopic
    }
    else
    {
      Serial.print("failed, rc=");  // 如果ESP8266开发板还是没有连上了MQTT服务器，串口输出failed, rc=
      Serial.print(client.state()); // 串口输出错误代码
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Arduino初始化命令，写在setup函数中的命令只执行1次
void setup()
{
  pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
  pinMode(KEY, INPUT);          // 初始化KEY端口为输入。我们KEY定义为0端口，所以0端口接个按钮作为输入信号
  Serial.begin(115200);         // 初始化串口波特率为115200
  // 以下依次调用连接WiFi、设置MQTT服务器、回调（接收并处理来自MQTT服务器的消息）、连接MQTT服务器等函数
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  reconnect();
  // 如果没有成功订阅主题为led内容为0的消息，串口输出订阅失败;尝试重新订阅！，并在0.3秒后重新订阅
  while (!client.subscribe("led", 0))
  {
    Serial.println("订阅失败;尝试重新订阅！");
    client.subscribe("led", 0);
    delay(300);
  }
  // 如果成功订阅主题led，串口输出订阅成功~~~
  Serial.println("订阅成功~~~");
}

// Arduino循环执行命令，写在loop函数中的命令循环执行
void loop()
{
  // 如果ESP8266开发板没有连接上MQTT服务器，重新建立连接
  if (!client.connected())
  {
    reconnect();
  }

  client.loop();

  // 如果读取的KEY值不等于key_flag，串口输出：开关状态改变；Publish message: door:读取的KEY值
  if (digitalRead(KEY) != key_flag)
  {
    Serial.println("开关状态改变");
    Serial.print("Publish message: door:");
    Serial.println(digitalRead(KEY));
    // 准备将msg作为消息体PUBLISH，msg必须是字符串格式
    // 下面将读取的KEY值（digitalRead(KEY)）按照整型值（%d）的格式格式化为字符串并将其拷贝到msg中，字符串长度10
    snprintf(msg, 10, "%d", digitalRead(KEY));
    client.publish("door", msg); // ESP8266开发板PUBLISH一个主题为door内容为msg的消息
  }
  key_flag = digitalRead(KEY);

  long now = millis(); // 读取当前时间
  if (now - lastMsg > 2000)
  {
    lastMsg = now;
    ++value;
    snprintf(msg, 50, "%d", value); // 如果已经过了2秒，更新lastMsg，value值加1。准备将value值发布到MQTT服务器，像上面的KEY值一样处理成字符串
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("temp", msg); // ESP8266开发板PUBLISH一个主题为temp内容为msg的消息
  }
}

