#include <LoRa.h>
#include <SPI.h>
#include <EEPROM.h>

#define ss 15
#define rst 16
#define dio0 2
#define networkid "1023"

const int hsen = D2;
const int lsen = D5;
const int hpin = D1;
const int sled = LED_BUILTIN;
const int input1 = D9;

int deviceNum = 1;
String deviceid = "01";
int counter = 10;
int vstate1 = 2;
int vstate2 = 2;
int temp_count1 = 0;
int temp_count2 = 0;
int temp_count3 = 0;


void ReadEEPROM() {
  EEPROM.begin(512);
  deviceNum = EEPROM.read(0);
  if (deviceNum < 1 || deviceNum > 6) {
    deviceNum = 1;
  }
  deviceid = (deviceNum < 10) ? "0" + String(deviceNum) : String(deviceNum);
  Serial.print("Device ID loaded from EEPROM: ");
  Serial.println(deviceid);
}


void SaveEEPROM() {
  int temp_count = 0;
  while (digitalRead(input1) == 0) {
    temp_count++;
    Serial.print("Button hold count: ");
    Serial.println(temp_count);
    if (temp_count >= 3) {
      Serial.println("Device ID Change Triggered!");
      deviceNum = (deviceNum < 6) ? deviceNum + 1 : 1;

      EEPROM.write(0, deviceNum);
      if (EEPROM.commit()) {
        Serial.println("EEPROM successfully committed");
      } else {
        Serial.println("ERROR! EEPROM commit failed");
      }

      deviceid = (deviceNum < 10) ? "0" + String(deviceNum) : String(deviceNum);
      Serial.print("New Device ID: ");
      Serial.println(deviceid);
      temp_count = 0;
    }
    delay(1000);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(input1, INPUT_PULLUP);
  pinMode(D0, WAKEUP_PULLUP);
  pinMode(hsen, INPUT_PULLUP);
  pinMode(lsen, INPUT_PULLUP);
  pinMode(hpin, OUTPUT);
  pinMode(sled, OUTPUT);
  digitalWrite(hpin, LOW);
  digitalWrite(sled, HIGH);

  ReadEEPROM();

  Serial.println("LoRa Sender");
  LoRa.setPins(ss, rst, dio0);
  LoRa.setSyncWord(0xA2);
  LoRa.setTxPower(20);
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(62.5E3);

  while (!LoRa.begin(433920000)) {
    Serial.println("LoRa Init Failed");
    delay(500);
  }
  Serial.println("LoRa Initialized Successfully!");
}


void send_data() {
  for (int i = 0; i < 10; i++) {
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
  Serial.println();
  SaveEEPROM();
}

void loop() {
  Serial.print("Sending Packet: ");
  Serial.println(counter);

  
  if (digitalRead(hsen) == 0 && digitalRead(hpin) == 1) {
    temp_count1++;
    if (temp_count1 >= 3) {
      vstate1 = 0;
      vstate2 = 0;
    }
    if (temp_count1 >= 15) {
      digitalWrite(hpin, LOW);
      Serial.println("Motor OFF");
      temp_count1 = 0;
    }
  } else {
    temp_count1 = 0;
  }


  if (digitalRead(lsen) == 0 && digitalRead(hpin) == 0) {
    temp_count2++;
    if (temp_count2 >= 3) {
      digitalWrite(hpin, HIGH);
      vstate1 = 1;
      vstate2 = 1;
      Serial.println("Motor ON");
      temp_count2 = 0;
    }
  } else {
    temp_count2 = 0;
  }


  if (digitalRead(hpin) == 1 && digitalRead(lsen) != 0) {
    temp_count3++;
    if (temp_count3 >= 3) {
      vstate1 = 0;
      vstate2 = 0;
    }
    if (temp_count3 >= 10) {
      digitalWrite(hpin, LOW);
      Serial.println("Motor OFF");
      temp_count3 = 0;
    }
  } else {
    temp_count3 = 0;
  }

 
  counter++;
  if (counter >= 200) counter = 10;

  send_data();


  digitalWrite(sled, LOW);
  delay(50);
  digitalWrite(sled, HIGH);
  delay(50);

  delay(2000);  
}
