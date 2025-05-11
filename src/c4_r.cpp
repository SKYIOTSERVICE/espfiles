
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <LoRa.h>
#include <SPI.h>

#define ss D8
#define rst D9
#define dio0 D10

int counter=1;
int timeout_count1=10;
int timeout_count2=10;
int rnum=10;
const int led1=BUILTIN_LED;
String tank_data1="10230422";
String tank_data2="10230522";
String tank_data3="10230622";
String tank_data4="10230722";

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    String d4;
    String d5;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  //Serial.println();
  //Serial.println(myData.d4);
  //Serial.println(myData.d5);
  if(myData.d4=="10230400" || myData.d4=="10230411"){
    tank_data1=myData.d4;
    timeout_count1=10;
  }
  if(myData.d5=="10230500" || myData.d5=="10230511"){
    tank_data2=myData.d5;
    timeout_count1=10;
  }
  if(myData.d4=="10230600" || myData.d4=="10230611"){
    tank_data3=myData.d4;
    timeout_count2=10;
  }
  if(myData.d5=="10230700" || myData.d5=="10230711"){
    tank_data4=myData.d5;
    timeout_count2=10;
  }
  //Serial.println(sizeof(myData));
  Serial.println(tank_data1);
  Serial.println(tank_data2);
  Serial.println(tank_data3);
  Serial.println(tank_data4);
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  pinMode(led1,OUTPUT);
  digitalWrite(led1,HIGH);
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("LoRa Sender");
  LoRa.setPins(ss, rst, dio0);    //setup LoRa transceiver module
  LoRa.setSyncWord(0xA2);
  LoRa.setTxPower(20);
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(62.5E3);
  while (!LoRa.begin(433920000))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println(".");
    delay(500);
  }
  Serial.println("LoRa Initializing OK!");

}

void send_data1(){
    int i;
    rnum=random(5,20);
    for(i=0;i<=rnum;i++){
      LoRa.beginPacket();   //Send LoRa packet to receiver
      LoRa.print(tank_data1);
      LoRa.endPacket(); 
      Serial.print(".");
      delay(100);
    }
    rnum=random(5,20);
    for(i=0;i<=rnum;i++){
      LoRa.beginPacket();   //Send LoRa packet to receiver
      LoRa.print(tank_data2);
      LoRa.endPacket(); 
      Serial.print(".");
      delay(100);
    }
    digitalWrite(led1,LOW);
    delay(100);
    digitalWrite(led1,HIGH);
    Serial.println("");
 }

 void send_data2(){
  int i;
  rnum=random(5,20);
  for(i=0;i<=rnum;i++){
    LoRa.beginPacket();   //Send LoRa packet to receiver
    LoRa.print(tank_data3);
    LoRa.endPacket(); 
    Serial.print(".");
    delay(100);
  }
  rnum=random(5,20);
  Serial.println("");
  for(i=0;i<=rnum;i++){
    LoRa.beginPacket();   //Send LoRa packet to receiver
    LoRa.print(tank_data4);
    LoRa.endPacket(); 
    Serial.print(".");
    delay(100);
  }
  digitalWrite(led1,LOW);
  delay(100);
  digitalWrite(led1,HIGH);
  Serial.println("");
}

void loop() {
    counter=counter+1;
    Serial.println("Data...");
    if(timeout_count1>4){
      send_data1();
    }
    if(timeout_count2>4){
      send_data2();
    }
    if(timeout_count1>0){
      timeout_count1=timeout_count1-1;
    }
    if(timeout_count2>0){
      timeout_count2=timeout_count2-1;
    }
    if(timeout_count1<=0){
      tank_data1="10230422";
      tank_data2="10230522";
    }
    if(timeout_count2<=0){
      tank_data1="10230622";
      tank_data2="10230722";
    }
    delay(4000);
    Serial.println(counter);
    if(counter>=1200){
      ESP.restart();
    }
}