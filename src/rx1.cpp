#include <SoftwareSerial.h>

// Define the software serial pins for the receiver
#define RECEIVER_RX_PIN D1 // D6
#define RECEIVER_TX_PIN D2 // D5

// Create a SoftwareSerial object for the receiver
SoftwareSerial receiverSerial(RECEIVER_RX_PIN, RECEIVER_TX_PIN);

const int ledPin = LED_BUILTIN; // Define the pin connected to the LED

void setup() {
  Serial.begin(115200); // Initialize hardware serial communication
  receiverSerial.begin(9600); // Initialize software serial communication
  pinMode(ledPin, OUTPUT); // Set the LED pin as output
}

void loop() {
  if (receiverSerial.available() > 0) {
    int data = receiverSerial.read(); // Read data from sender
    Serial.println(data);
    if (data == 48) {
      digitalWrite(ledPin, LOW); // Turn off LED
      Serial.println("LED OFF");
    } else if (data == 49 ) {
      digitalWrite(ledPin, HIGH); // Turn on LED
      Serial.println("LED ON");
    }
  }
}