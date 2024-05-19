#include <Servo.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

#define FAN_PIN 15
#define SERVO_PIN 5
#define BUTTON_MODE_PIN 13
#define BUTTON_FAN_PIN 12
#define BUTTON_SWING_PIN 14
#define BUTTON_STOP_PIN 16

Servo servo;
DHT dht(DHTPIN, DHTTYPE);

enum FanState
{
    STOPPED,
    SPEEDLOW,
    SPEEDHIGH
};
enum Mode
{
    MANUAL,
    AUTO
};

Mode currentMode = MANUAL;
FanState fanState = STOPPED;
bool isSwinging = false;
bool isStopped = false;

// Function pointer type for state functions
typedef void (*StateFunc)();

// State function declarations
void stateManualStopped();
void stateManualLow();
void stateManualHigh();
void stateManualSwing();
void stateAutoRunning();
void stateAutoStopped();

// Current state function pointer
StateFunc currentState;

// Button debounce variables
unsigned long lastDebounceTimeMode = 0;
unsigned long lastDebounceTimeFan = 0;
unsigned long lastDebounceTimeSwing = 0;
unsigned long lastDebounceTimeStop = 0;
const unsigned long debounceDelay = 50; // 50ms debounce delay

void setup()
{
    // Serial.print("开始进入");
    Serial.begin(115200);
    pinMode(FAN_PIN, OUTPUT);
    pinMode(BUTTON_MODE_PIN, INPUT);
    pinMode(BUTTON_FAN_PIN, INPUT);
    pinMode(BUTTON_SWING_PIN, INPUT);
    pinMode(BUTTON_STOP_PIN, INPUT);

    servo.attach(SERVO_PIN, 500, 2500);
    servo.write(90); // 初始化舵机位置
    dht.begin();

    // Set initial state
    currentState = stateManualStopped;
    // Serial.print("开始结束");
}

void loop()
{
    Serial.print("舵机标志位:");
    Serial.println(isSwinging);
    // Call the current state function
    currentState();

    // Handle button presses
    handleButtons();
    // Serial.print("按钮读取结束");
}

void stateManualStopped()
{
    Serial.println("State: Manual Stopped");
    if (!isStopped)
    {
        analogWrite(FAN_PIN, 0);
        servo.write(90); // Stop swinging
    }
}

void stateManualLow()
{
    Serial.println("State: Manual Low Speed");
    if (!isStopped)
    {
        analogWrite(FAN_PIN, 128); // Low speed
        Serial.print("舵机标志位:");
        Serial.println(isSwinging);
        if (isSwinging)
        {
            swingServo();
        }
    }
}

void stateManualHigh()
{
    Serial.println("State: Manual High Speed");
    if (!isStopped)
    {
        analogWrite(FAN_PIN, 255); // High speed
        if (isSwinging)
        {
            swingServo();
        }
    }
}

void stateManualSwing()
{
    Serial.println("State: Manual Swinging");
    if (!isStopped)
    {
        if (isSwinging)
        {
            swingServo();
        }
    }
}

void stateAutoRunning()
{
    Serial.println("State: Auto Running");
    if (!isStopped)
    {
        float temperature = dht.readTemperature();
        if (isnan(temperature))
        {
            Serial.println("Failed to read from DHT sensor!");
            return;
        }

        Serial.print("Current temperature: ");
        Serial.print(temperature);
        Serial.println(" *C");

        if (temperature > 30)
        {
            fanState = SPEEDHIGH;
            analogWrite(FAN_PIN, 255); // High speed
        }
        else if (temperature > 26)
        {
            fanState = SPEEDLOW;
            analogWrite(FAN_PIN, 128); // Low speed
        }
        else
        {
            fanState = STOPPED;
            analogWrite(FAN_PIN, 0); // Stop
        }

        isSwinging = true;
        Serial.println("自动模式舵机开始运行");
        swingServo();
    }
}

void stateAutoStopped()
{
    Serial.println("State: Auto Stopped");
    analogWrite(FAN_PIN, 0);
    servo.write(90); // Stop swinging
}

void toggleMode()
{
    if (currentMode == MANUAL)
    {
        currentMode = AUTO;
        currentState = stateAutoRunning;
    }
    else
    {
        currentMode = MANUAL;
        currentState = stateManualStopped;
    }
}

void toggleFanState()
{
    if (currentMode == MANUAL && !isStopped)
    {
        if (fanState == STOPPED)
        {
            fanState = SPEEDLOW;
            currentState = stateManualLow;
        }
        else if (fanState == SPEEDLOW)
        {
            fanState = SPEEDHIGH;
            currentState = stateManualHigh;
        }
        else if (fanState == SPEEDHIGH)
        {
            fanState = STOPPED;
            currentState = stateManualStopped;
        }
    }
}

void toggleSwing()
{
    if (currentMode == MANUAL && !isStopped)
    {
        isSwinging = !isSwinging;
        if (isSwinging)
        {
            currentState = stateManualSwing;
        }
        else
        {
            servo.write(90); // Stop swinging
            currentState = (fanState == SPEEDLOW) ? stateManualLow : (fanState == SPEEDHIGH) ? stateManualHigh
                                                                                             : stateManualStopped;
        }
    }
}

void toggleStop()
{
    isStopped = !isStopped;
    if (isStopped)
    {
        analogWrite(FAN_PIN, 0);
        servo.write(90); // Stop swinging
    }
    else
    {
        if (currentMode == AUTO)
        {
            currentState = stateAutoRunning;
        }
        else
        {
            currentState = stateManualStopped;
        }
    }
}

void handleButtons()
{
    static int lastReadingMode = LOW;
    static int lastReadingFan = LOW;
    static int lastReadingSwing = LOW;
    static int lastReadingStop = LOW;

    int readingMode = digitalRead(BUTTON_MODE_PIN);
    int readingFan = digitalRead(BUTTON_FAN_PIN);
    int readingSwing = digitalRead(BUTTON_SWING_PIN);
    int readingStop = digitalRead(BUTTON_STOP_PIN);

    // Handle mode button
    if (readingMode != lastReadingMode)
    {
        delay(50); // Debounce delay
        readingMode = digitalRead(BUTTON_MODE_PIN);
        if (readingMode == HIGH)
        {
            toggleMode();
        }
    }
    lastReadingMode = readingMode;

    // Handle fan state button
    if (readingFan != lastReadingFan)
    {
        delay(50); // Debounce delay
        readingFan = digitalRead(BUTTON_FAN_PIN);
        if (readingFan == HIGH)
        {
            toggleFanState();
        }
    }
    lastReadingFan = readingFan;

    // Handle swing button
    if (readingSwing != lastReadingSwing)
    {
        delay(50); // Debounce delay
        readingSwing = digitalRead(BUTTON_SWING_PIN);
        if (readingSwing == HIGH)
        {
            toggleSwing();
        }
    }
    lastReadingSwing = readingSwing;

    // Handle stop button
    if (readingStop != lastReadingStop)
    {
        delay(50); // Debounce delay
        readingStop = digitalRead(BUTTON_STOP_PIN);
        if (readingStop == HIGH)
        {
            toggleStop();
        }
    }
    lastReadingStop = readingStop;
}

void swingServo()
{
    static uint32_t times1, times2;
    Serial.println("舵机开始运行");

    if (millis() - times1 > 500)
    {
        if (times2)
        {
            servo.write(180);
            times2++;
            times2 = times2 % 2;
        }
        else
        {
            servo.write(0);
            times2++;
            times2 = times2 % 2;
        }
        times1 = millis();
    }
}
