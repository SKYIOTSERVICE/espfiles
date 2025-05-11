#include <Arduino.h>
#include <SPI.h>
#include <SoftwareSerial.h>

// RO to pin 8 & DI to pin 9 when using AltSoftSerial
#define RE 6
#define DE 7

const byte nitro[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};


byte values[11];
SoftwareSerial mod(D7,D8);

void setup() {
  Serial.begin(9600);
  mod.begin(9600);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);

  // put RS-485 into receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  delay(3000);
}

void loop() {
  byte val1, val2, val3;
  Serial.print("Nitrogen: ");
  val1 = nitrogen();
  Serial.print(" = ");
  Serial.print(val1);
  Serial.println(" mg/kg");
  delay(1000);
}

byte nitrogen() {
  // clear the receive buffer
  mod.flush();

  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);

  // write out the message
  for (uint8_t i = 0; i < sizeof(nitro); i++ ) mod.write( nitro[i] );

  // wait for the transmission to complete
  mod.write
  
  // switching RS485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // delay to allow response bytes to be received!
  delay(200);

  // read in the received bytes
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    Serial.print(values[i], HEX);
    Serial.print(' ');
  }
  return values[4];
}