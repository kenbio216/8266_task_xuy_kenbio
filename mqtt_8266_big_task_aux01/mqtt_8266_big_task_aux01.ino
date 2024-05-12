void setup()
{
    Serial.begin(115200);
}

void loop()
{
    for (int i = 0; i < 10; i++)
    {
        Serial.println(i);
    }
    delay(1000);
    // Serial.println("Hello, world!");
}