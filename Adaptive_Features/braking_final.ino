
#include <Wire.h>                               
#include <HCSR04.h>

const int hall = 2; // Pin where the Hall effect sensor is connected
unsigned long lastTime;
unsigned long lastPulseTime;
unsigned long pulseCount = 0;
float rpm;

int seatbelt;
int speed;
// Define the pins for the ultrasonic sensors
byte adas_breaking_pin = 5;
byte trigPin = 33;

int ir = 6;
byte numSensors = 6;
byte brake_pin = 7;
byte* echoPins = new byte[numSensors] {3, 4, 27, 29, 31, 35};

const int brake_treshold_distance = 200;
double* distances;

int data = 0;

String doubleArrayToString(double doubleArray[], int length);

// Array to store distance values
float front_distances[2] = {0, 0};

void setup()
{
    lastTime = millis();
    lastPulseTime = millis();
    Wire.begin(9);
    Wire.onRequest(requestEvent);
    Serial.begin(9600);
    HCSR04.begin(trigPin, echoPins, numSensors);
    pinMode(brake_pin, OUTPUT);
    pinMode(adas_breaking_pin, INPUT_PULLUP);
    pinMode(hall, INPUT);
    pinMode(ir, INPUT_PULLUP);
}

void requestEvent()
{
    Wire.write(data);
}

void loop()
{
    // Check for a pulse
    if (digitalRead(ir) == LOW)
    {
        pulseCount++;
        lastPulseTime = millis();
    }

    // Calculate RPM every second
    if (millis() - lastTime >= 1000)
    {
        // Calculate time between pulses in seconds
        float elapsedTime = (millis() - lastPulseTime) / 1000.0;

        // Calculate RPM
        rpm = ((float)pulseCount / (elapsedTime / 60.0))/8;

        pulseCount = 0;
        lastTime = millis();
    }

    seatbelt = digitalRead(hall);

    distances = HCSR04.measureDistanceCm();
    for (int i = 0; i < numSensors; i++)
    {
        if (i < 2)
            front_distances[i] = distances[i];
        data = front_distances[0] <= front_distances[1] ? front_distances[0] : front_distances[1];
        if (data < 0)
            data = data = front_distances[0] > front_distances[1] ? front_distances[0] : front_distances[1];

        if (front_distances[0] == -1 && front_distances[1] == -1)
            data = 255;
    }

    String myString = doubleArrayToString(distances, 6);

        Serial.println(myString);
        Serial.println(seatbelt);
        Serial.println(rpm);

    if (data < brake_treshold_distance && digitalRead(adas_breaking_pin) == LOW)
    {
        digitalWrite(brake_pin, LOW);
    }
    else
        digitalWrite(brake_pin, HIGH);

    delay(100);
}

String doubleArrayToString(double doubleArray[], int length)
{
    String result = "";

    for (int i = 0; i < length; i++)
    {
        result += String(doubleArray[i], 0);

        if (i < length - 1)
        {
            result += ",";
        }
    }

    return result;
}
