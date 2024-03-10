#include <HCSR04.h>
#include <Wire.h>
#include<Adafruit_MCP4725.h>
Adafruit_MCP4725 MCP4725;

#define manual_accl_pin A3
#define cruise_on_pin 4
#define brake_on_pin 2
#define toggle_pin 5
#define regen_pin 8
#define regen_on_pin 7


#define idle_throtle 252

int manual_accl = 163, accl_out = 0, cruise_on = HIGH, brake_on = HIGH, current_accl, delta_accl, i = 0, regen_on;
int distance = 0;

char* regen;



void setup () {
  pinMode(brake_on_pin, INPUT_PULLUP);
  pinMode(cruise_on_pin, INPUT_PULLUP);
  pinMode(regen_on_pin, INPUT_PULLUP);
  pinMode(toggle_pin, OUTPUT);
  pinMode(regen_pin, OUTPUT);
  pinMode(regen_pin, OUTPUT);
  Serial.begin(9600);

  Wire.begin(); 
  MCP4725.begin(0x60);
}

void loop () {
  Serial.print("hi");
  regen_on = digitalRead(regen_on_pin);
  brake_on = digitalRead(brake_on_pin);
  cruise_on = digitalRead(cruise_on_pin);
  current_accl = analogRead(manual_accl_pin);
  delta_accl = current_accl - idle_throtle;

  Wire.requestFrom(9,1);
  distance = Wire.read();
  

  if(current_accl<275){
    i = 1;
  }
  if(current_accl<275 && regen_on == LOW){
    digitalWrite(regen_pin, LOW);
    regen = "ON";
  }
  else{
    digitalWrite(regen_pin, HIGH);
    regen = "OFF";
  }
  if(brake_on == LOW)
  {
    while(cruise_on == LOW)
    {
        cruise_on = digitalRead(cruise_on_pin);
        current_accl = analogRead(manual_accl_pin);
        digitalWrite(toggle_pin, HIGH);
        if(current_accl<275 && regen_on == LOW){
          digitalWrite(regen_pin, LOW);
        }
        else
          digitalWrite(regen_pin, HIGH);
    }
  }
  if(i==1)
  {  
    if(delta_accl > 25)
    {
      while(cruise_on == LOW)
      {
          digitalWrite(13,HIGH);
          cruise_on = digitalRead(cruise_on_pin);
          current_accl = analogRead(manual_accl_pin);
          digitalWrite(toggle_pin, HIGH);
          if(current_accl<275 && regen_on == LOW){
            digitalWrite(regen_pin, LOW);
          }
          else
            digitalWrite(regen_pin, HIGH);
          Serial.println("Working");
      }
      i = 0;
      digitalWrite(13,LOW);
    }
  }
  
  if(cruise_on == LOW)
  {
    digitalWrite(regen_pin, HIGH);
    digitalWrite(toggle_pin,LOW);
    if(distance < 50)
      accl_out = 0;

    else if(distance < 100)
      accl_out = (int)manual_accl*0.75;

    else
      accl_out = manual_accl;
    

    MCP4725.setVoltage((accl_out)*4, false);

    digitalWrite(toggle_pin, LOW);
    Serial.print(distance);
    Serial.println(" cm");
    Serial.println(accl_out);
    Serial.println("---");
  }
  else
  {
    digitalWrite(toggle_pin,HIGH);
    manual_accl = analogRead(manual_accl_pin);
    accl_out = manual_accl;
    Serial.println(accl_out);
    Serial.println(distance);
    Serial.println(regen);
  }
  delay(100);
}