#include <Arduino.h>
#line 1 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_5_4\\mqtt_8266_5_4.ino"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "kenbio";        // Enter your WiFi name
const char *password = "123456xuy"; // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "192.168.43.75";
const char *topic = "xuy_8266/demo001";
const char *mqtt_username = "xuy8266";
const char *mqtt_password = "ex123456";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

#line 18 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_5_4\\mqtt_8266_5_4.ino"
void setup();
#line 54 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_5_4\\mqtt_8266_5_4.ino"
void callback(char *topic, byte *payload, unsigned int length);
#line 67 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_5_4\\mqtt_8266_5_4.ino"
void loop();
#line 18 "E:\\win_code_git\\8266_task_xuy_kenbio\\mqtt_8266_5_4\\mqtt_8266_5_4.ino"
void setup()
{
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
    // publish and subscribe
    client.publish(topic, "hello emqx");
    client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void loop()
{
    client.loop();
}

