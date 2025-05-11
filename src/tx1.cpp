#include <SoftwareSerial.h>

// Define the software serial pins for the sender
#define SENDER_RX_PIN D1 // D6
#define SENDER_TX_PIN D2 // D5
int count = 0;
const int hsen01 = D5;
const int lsen01 = D6;
int temp_count1=0;

// Create a SoftwareSerial object for the sender
SoftwareSerial senderSerial(SENDER_RX_PIN, SENDER_TX_PIN);

void setup() {
  Serial.begin(4800); // Initialize hardware serial communication
  //senderSerial.begin(9600); // Initialize software serial communication
  //Serial.println("Sender is ready.");
  pinMode(hsen01, INPUT_PULLUP); 
  pinMode(lsen01, INPUT_PULLUP);
}

void loop() {
  // Read input from serial monitor
  count = count+1;
  String a="0010001111";
  /*
  if (Serial.available() > 0) {
    char data = Serial.read();
    if (data == '0' || data == '1') { // Check if received data is '0' or '1'
      Serial.println("Sending command: " + String(data));
      senderSerial.write(data); // Send data to receiver
    }
  }*/
  if(digitalRead(hsen01)==0){
    senderSerial.write(11);
  }else{
    senderSerial.write(10);
  }

  Serial.print("TEXT");
  
  //int bytesSent = senderSerial.write(“hello”);
  delay(2000); // Small delay
}

