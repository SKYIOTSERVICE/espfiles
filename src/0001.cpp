#include <LoRa.h>
#include <SPI.h>
#include <EEPROM.h>

#define ss 15
#define rst 16
#define dio0 2

#define networkid "1023"

#define hsen D2
#define lsen D5
#define hpin D1
#define sled LED_BUILTIN
#define input D9

int counter = 10;
int value = 11;
int state = 0;
int vstate1 = 2;
int vstate2 = 2;
int volt_state = 1;
int temp_count1 = 0;
int temp_count2 = 0;
int temp_count3 = 0;

int buttonState = HIGH;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

int deviceNum = 1;  
String deviceid = "01";

void readDeviceIdFromEEPROM() {
  EEPROM.begin(512);  
  deviceNum = EEPROM.read(0);
  if (deviceNum < 1 || deviceNum > 2) {
    deviceNum = 1;
  }
  deviceid = (deviceNum < 10) ? "0" + String(deviceNum) : String(deviceNum);
  Serial.print("Device ID loaded from EEPROM: ");
  Serial.println(deviceid);
}

void saveDeviceIdToEEPROM() {
  EEPROM.write(0, deviceNum);
  EEPROM.commit();  
  Serial.print("Device ID saved to EEPROM: ");
  Serial.println(deviceid);
}

void setup() {
  Serial.begin(115200);
  pinMode(D0, WAKEUP_PULLUP);
  pinMode(hsen, INPUT_PULLUP);
  pinMode(lsen, INPUT_PULLUP);
  pinMode(hpin, OUTPUT);
  pinMode(sled, OUTPUT);
  pinMode(input, INPUT_PULLUP);

  digitalWrite(hpin, LOW);
  digitalWrite(sled, HIGH);

  readDeviceIdFromEEPROM();

  while (!Serial);
  Serial.println("LoRa Sender");

  LoRa.setPins(ss, rst, dio0);
  LoRa.setSyncWord(0xA2);
  LoRa.setTxPower(20);
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(62.5E3);

  while (!LoRa.begin(433920000)) {
    Serial.println(".");
    delay(500);
  }
  Serial.println("LoRa Initializing OK!");
}

void send_data() {
  for (int i = 0; i <= 10; i++) {
    LoRa.beginPacket();
    LoRa.print(networkid);
    LoRa.print(deviceid);
    LoRa.print(vstate1);
    LoRa.print(vstate2);
    LoRa.endPacket();
    Serial.print(networkid);
    Serial.print(deviceid);
    Serial.print(vstate1);
    Serial.print(vstate2);
    Serial.print(".");
    delay(100);
  }
  Serial.println("Hii");
}

void checkButton() {
  int reading = digitalRead(input);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        deviceNum++;
        if (deviceNum > 2) deviceNum = 1;
        deviceid = (deviceNum < 10) ? "0" + String(deviceNum) : String(deviceNum);
        saveDeviceIdToEEPROM();
      }
    }
  }
  lastButtonState = reading;
}

void loop() {
  checkButton();

  Serial.print("Sending packet: ");
  Serial.println(counter);

  if (digitalRead(hsen) == 0 && digitalRead(hpin) == 1) {
    temp_count1++;
    Serial.println("temp count1: " + String(temp_count1));
    if (temp_count1 >= 3) {
      state = 2;
      vstate1 = 0;
      vstate2 = 0;
    }
    if (temp_count1 >= 15) {
      Serial.println("LOW....");
      digitalWrite(hpin, LOW);
      temp_count1 = 0;
    }
  } else {
    temp_count1 = 0;
  }

  if (digitalRead(lsen) == 0 && digitalRead(hpin) == 0) {
    temp_count2++;
    Serial.println("temp count2: " + String(temp_count2));
    if (temp_count2 >= 3) {
      Serial.println("HIGH...");
      digitalWrite(hpin, HIGH);
      state = 1;
      vstate1 = 1;
      vstate2 = 1;
      temp_count2 = 0;
    }
  } else {
    temp_count2 = 0;
  }

  if (digitalRead(hpin) == 1 && digitalRead(lsen) != 0) {
    temp_count3++;
    if (temp_count3 >= 3) {
      state = 2;
      vstate1 = 0;
      vstate2 = 0;
    }
    if (temp_count3 >= 10) {
      digitalWrite(hpin, LOW);
      temp_count3 = 0;
    }
  } else {
    temp_count3 = 0;
  }

  counter++;
  if (counter >= 200) {
    counter = 10;
  }

  send_data();

  digitalWrite(sled, LOW);
  delay(50);
  digitalWrite(sled, HIGH);
  delay(50);
  delay(2000);
}
