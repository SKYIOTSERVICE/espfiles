#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  String d4;
  String d5;
} struct_message;

// Create a struct_message called myData
struct_message myData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

int count = 0;
const int hsen1 = D1;
const int lsen1 = D2;
const int hsen2 = D6;
const int lsen2 = D7;
const int hpin1 = D8;
const int hpin2 = D5;
const int led1 = LED_BUILTIN;
int temp_count1=0;
int temp_count2=0;
int temp_count3=0;
int temp_count4=0;
int temp_count5=0;
int temp_count6=0;
int temp_status=0;

void setup() {
  Serial.begin(115200); // Initialize hardware serial communication
  Serial.println("Sender is ready.");
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    WiFi.setOutputPower(20);
      // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  myData.d4 = "10230600";
  myData.d5 = "10230700";

  pinMode(hsen1, INPUT_PULLUP); 
  pinMode(lsen1, INPUT_PULLUP);
  pinMode(hsen2, INPUT_PULLUP); 
  pinMode(lsen2, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(hpin1, OUTPUT);
  pinMode(hpin2, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(hpin1, LOW);
  digitalWrite(hpin2, LOW);
}

void loop() {
  count = count+1;
  Serial.println(count);
  // Device6 =================================================================

  if(digitalRead(hsen1)==0 && digitalRead(hpin1)==1){
    temp_count1=temp_count1+1;
    Serial.println("temp count1");
    Serial.println(temp_count1);
    if(temp_count1>=3){
      myData.d4 = "10230600";
    }
    if(temp_count1>=30){
      Serial.println("LOW....");
      digitalWrite(hpin1, LOW);
      temp_count1=0;
    }
  }else{
    temp_count1=0;
  }

  if(digitalRead(lsen1)==0 && digitalRead(hpin1)==0){
    temp_count2=temp_count2+1;
    Serial.println("temp count2");
    Serial.println(temp_count2);
    if(temp_count2>=3){
      Serial.println("HIGH...");
      digitalWrite(hpin1, HIGH);
      myData.d4 = "10230611";
      temp_count2=0;
    }
  }else{
    temp_count2=0;
  }
  
  //=======================

  if(digitalRead(hpin1)==1 && digitalRead(lsen1)!=0){
    temp_count3=temp_count3+1;
    if(temp_count3>=3){
      myData.d4 = "10230600";
    }
    if(temp_count3>=30){
      digitalWrite(hpin1, LOW);
      temp_count3=0;
    }
  }else{
    temp_count3=0;
  }

  // Device7 =================================================================
  if(digitalRead(hsen2)==0 && digitalRead(hpin2)==1){
    temp_count4=temp_count4+1;
    Serial.println("temp count");
    Serial.println(temp_count4);
    if(temp_count4>=3){
      myData.d5 = "10230700";
    }
    if(temp_count4>=30){
      Serial.println("LOW....");
      digitalWrite(hpin2, LOW);
      temp_count4=0;
    }
  }else{
    temp_count4=0;
  }

  if(digitalRead(lsen2)==0 && digitalRead(hpin2)==0){
    temp_count5=temp_count5+1;
    Serial.println("temp count");
    Serial.println(temp_count5);
    if(temp_count5>=3){
      Serial.println("HIGH...");
      digitalWrite(hpin2, HIGH);
      myData.d5 = "10230711";
      temp_count5=0;
    }
  }else{
    temp_count5=0;
  }

  //=======================

  if(digitalRead(hpin2)==1 && digitalRead(lsen2)!=0){
    temp_count6=temp_count6+1;
    if(temp_count6>=3){
      myData.d5 = "10230700";
    }
    if(temp_count6>=30){
      digitalWrite(hpin2, LOW);
      temp_count6=0;
    }
  }else{
    temp_count6=0;
  }

  if(count>=500){
    count=0;
  }


  // Send message via ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  delay(100);
  digitalWrite(led1, HIGH);
  delay(2000);
  digitalWrite(led1, LOW);
}