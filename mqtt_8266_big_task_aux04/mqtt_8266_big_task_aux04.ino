void setup()
{
    pinMode(0, INPUT_PULLUP);
    pinMode(1, INPUT_PULLUP);
    pinMode(4,OUTPUT);
    pinMode(28,OUTPUT);
    digitalWrite(4,HIGH);
    digitalWrite(28,HIGH);
    delay(500);
    digitalWrite(4,LOW);
    digitalWrite(28,LOW);

    Serial.begin(115200);
}

void loop()
{

    Serial.print("GPIO0: ");
    Serial.print(digitalRead(0));
    Serial.print(" GPIO1: ");
    Serial.println(digitalRead(1));
    delay(500);
}
