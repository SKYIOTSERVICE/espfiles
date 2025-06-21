#include <ESP8266WiFi.h>
#include <TM1637Display.h>
#include <EEPROM.h>
#include <ESP8266HTTPClient.h>

#define CLK D3
#define DIO D4
#define buzzer D2
#define input1 D9

const char* ssid = "Airel_8600577773";
const char* password = "air10162";

const char* commandUrl = "http://your-server.com/status.php";  
const char* expectedNetworkID = "2012";
const char* expectedDeviceID = "01";

TM1637Display display(CLK, DIO);

const uint8_t seg_empty[] = {
  0x00,
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,
  0x00
};

const uint8_t seg_full[] = {
  0x00,
  SEG_A | SEG_E | SEG_F | SEG_G,
  SEG_A | SEG_E | SEG_F | SEG_G,
  0x00
};

int motor_status = 0;
int motor_duration = 30;
int motor_time = 0;
int sensor_status = 2;
int tcount1 = 0, tcount2 = 0, tcount3 = 0;
int temp_count1 = 0;
unsigned long lastCheck = 0;

void handleStates(int vstate1, int vstate2) {
  String status = String(vstate1) + String(vstate2);

  if (status == "00") { 
    display.clear();
    display.setSegments(seg_full);
    tcount1++;
    if (tcount1 >= 3) {
      Serial.println("Tank Full Detected");
      digitalWrite(buzzer, LOW);
      motor_status = 0;
      motor_time = motor_duration * 60;
      sensor_status = 0;
      tcount1 = 0;
    }
  } else tcount1 = 0;

  if (status == "11") { 
    display.clear();
    display.setSegments(seg_empty);
    tcount2++;
    if (tcount2 >= 3) {
      Serial.println("Tank Empty Detected");
      digitalWrite(buzzer, HIGH);
      motor_status = 1;
      sensor_status = 1;
      tcount2 = 0;
    }
  } else tcount2 = 0;

  if (status == "22") { 
    tcount3++;
    if (tcount3 >= 3) {
      Serial.println("Water Level Normal");
      sensor_status = 2;
      tcount3 = 0;
    }
  } else tcount3 = 0;
}

void checkInternetCommands() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(commandUrl);
    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      Serial.println("HTTP GET Response: " + payload);

      int nidIndex = payload.indexOf("\"networkid\":\"");
      int didIndex = payload.indexOf("\"deviceid\":\"");
      int v1Index = payload.indexOf("\"vstate1\":");
      int v2Index = payload.indexOf("\"vstate2\":");

      if (nidIndex >= 0 && didIndex >= 0 && v1Index >= 0 && v2Index >= 0) {
        String netid = payload.substring(nidIndex + 13, nidIndex + 17);
        String devid = payload.substring(didIndex + 13, didIndex + 15);
        int vstate1 = payload.substring(v1Index + 10, v1Index + 11).toInt();
        int vstate2 = payload.substring(v2Index + 10, v2Index + 11).toInt();

        Serial.println("Parsed Network ID: " + netid);
        Serial.println("Parsed Device ID: " + devid);
        Serial.println("Parsed vstate1: " + String(vstate1));
        Serial.println("Parsed vstate2: " + String(vstate2));

        if (netid == expectedNetworkID && devid == expectedDeviceID) {
          handleStates(vstate1, vstate2);
        } else {
          Serial.println("Ignored: Network ID or Device ID mismatch");
        }
      }
    } else {
      Serial.print("HTTP error: ");
      Serial.println(httpCode);
    }

    http.end();
  } else {
    Serial.println("WiFi disconnected");
  }
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);

  pinMode(buzzer, OUTPUT);
  pinMode(input1, INPUT_PULLUP);
  digitalWrite(buzzer, LOW);

  display.setBrightness(0x0f);
  display.clear();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  motor_duration = EEPROM.read(0);
  if (motor_duration < 1 || motor_duration > 180) {
    motor_duration = 30;
  }
  motor_time = motor_duration * 60;

  Serial.println("Internet-based Receiver ready.");
}

void loop() {
  if (digitalRead(input1) == LOW) {
    Serial.println("Manual Button Pressed");
    if (digitalRead(buzzer) == LOW) {
      digitalWrite(buzzer, HIGH);
      motor_status = 1;
      motor_time = motor_duration * 60;
    } else {
      digitalWrite(buzzer, LOW);
      motor_status = 0;
      Serial.println("Motor stopped manually");
    }
    delay(1000);
  }

  if (motor_status == 1) {
    motor_time--;
    display.showNumberDec((motor_time / 60) + 1, false);
    if (motor_time <= 0 || sensor_status == 0) {
      temp_count1++;
      if (temp_count1 >= 5) {
        digitalWrite(buzzer, LOW);
        motor_status = 0;
        motor_time = motor_duration * 60;
        temp_count1 = 0;
        display.clear();
      }
    } else {
      temp_count1 = 0;
    }
    delay(1000);
  } else {
    delay(500);
  }

  if (millis() - lastCheck > 10000) {
    lastCheck = millis();
    checkInternetCommands();
  }
}
