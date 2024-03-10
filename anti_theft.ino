#include <SoftwareSerial.h>

SoftwareSerial SIM900A(10, 11);

int sens = 7;
int led = 13; //led is relay
int rst = 5; //reset pin
String msgrec;

bool messageSent = false; // Flag to track whether the message has been sent

void setup() {
  SIM900A.begin(9600);   // Setting the baud rate of GSM Module
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  Serial.println("SIM900A Ready");
  delay(100);
  pinMode(sens, INPUT);
  pinMode(led, OUTPUT);
  pinMode(rst, OUTPUT);
}

void loop() {
  
  digitalWrite(rst,HIGH);
  if (!messageSent && digitalRead(sens) == HIGH) {
    SendMessage();
    messageSent = true; // Set the flag to indicate that the message has been sent
  }

  if (messageSent) {
    msgrec = RecieveMessage();
    
    
    if (msgrec == "y") {
      digitalWrite(led, HIGH); // Set the 13th pin (led) to HIGH if "y" is received
    } else if (msgrec == "n") {
      digitalWrite(led, LOW);  // Set the 13th pin (led) to LOW if "n" is received
    }

    // Reset the flag after processing the message
    messageSent = false;
  }

  if (SIM900A.available() > 0) {
    Serial.write(SIM900A.read());
  }
}

void SendMessage() {
  Serial.println("Sending Message");
  SIM900A.println("AT+CMGF=1");    // Sets the GSM Module in Text Mode
  delay(2000);
  SIM900A.println("AT+CMGS=\"+917994941374\"\r"); // Mobile phone number to send message
  delay(2000);
  SIM900A.println("Warning: your current vehicle location : (http://maps.google.com/maps?q={latitude},{longitude}) Your vehichle detected motion. Do you want to on the ALERT system? y/n");// Message content
  delay(2000);
  SIM900A.println((char)26);  // ASCII code of CTRL+Z
  delay(1000);

  // Check for "OK" response
  if (SIM900A.find("OK")) {
    Serial.println("Message has been sent -> SMS From Mavericks");
  } else {
    Serial.println("Error sending message");
  }

  // Wait for a response after sending the message
  delay(5000); // Adjust this delay as needed
}

String RecieveMessage() {
  Serial.println("Reading SMS");
  delay(1000);
  SIM900A.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(1000);
  
  // Wait for a moment to ensure that the module is ready to receive SMS
  delay(2000);
  
  Serial.println("Unread Message done");

  // Read the incoming message content
  String incomingMessage = "";
  while (SIM900A.available()) {
    incomingMessage += (char)SIM900A.read();
  }

  return incomingMessage;
}