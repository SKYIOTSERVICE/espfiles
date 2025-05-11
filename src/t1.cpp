#include <Arduino.h>
#include <LoRa.h>
#include <SPI.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Display Module connection pins (Digital Pins)
#define CLK D3
#define DIO D4

#define ss D8
#define rst D0
#define dio0 D4

const int buzzer = D2;
const int epin = D1;
const int input1 = D9;

int t_count=0;
int count=0;
int cnt=0;
int sdevice[]={0,0,0,0,0,0,0,0};
int cdevice[]={0,0,0,0,0,0,0,0};
int cdstatus[]={0,0,0,0,0,0,0,0};
int st=2;
int minval=0;
int temp_count1=0;
char motor_st='S';
int timeout_val=70;

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  Serial.begin(9600); // Starts the serial communication
  Wire.begin(2,0);
  lcd.init();
  lcd.setCursor(0,0);
  lcd.print("SkyIoT Control");
  lcd.backlight(); 
  pinMode(buzzer, OUTPUT);
  pinMode(epin, OUTPUT);
  digitalWrite(epin, LOW);
  digitalWrite(buzzer, LOW); 
  delay(1000);

  LoRa.setPins(ss, rst, dio0);    //setup LoRa transceiver module
  LoRa.setSyncWord(0xA2);
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(62.5E3);
  //LoRa.begin(433E6)
  int temp_count1=30;

  while (!LoRa.begin(433920000))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    //Serial.println(".");
    temp_count1++;
    if(temp_count1>=60){
      break;
    }
    delay(500);
  }

}

void loop() {
  //Serial.print("Count:");
  //Serial.println(count);
  int packetSize = LoRa.parsePacket();    // try to parse packet
  if (packetSize) 
  {
    //Serial.print("Packet Size:");
    //Serial.println(packetSize);
    //Serial.print("Received packet '");
    while (LoRa.available())              // read packet
    {
      String LoRaData = LoRa.readString();
      //Serial.println(LoRaData); 
      String netid=LoRaData.substring(0,4);
      String deviceid=LoRaData.substring(0,6);
      String devicestatus=LoRaData.substring(6);
      //Serial.print(deviceid);
      //Serial.print("    ");
      //Serial.println(devicestatus);
      if(netid.equals("1023") && packetSize==8){
        if(deviceid=="102301" && devicestatus=="00"){
          sdevice[0]=0;
          cdstatus[0]=0;
        }
        if(deviceid=="102301" && devicestatus=="11"){
          sdevice[0]=1;
          cdstatus[0]=1;
          cdevice[0]=timeout_val;
        }
        if(deviceid=="102302" && devicestatus=="00"){
          sdevice[1]=0;
          cdstatus[1]=0;
        }
        if(deviceid=="102302" && devicestatus=="11"){
          sdevice[1]=1;
          cdstatus[1]=1;
          cdevice[1]=timeout_val;
        }
        if(deviceid=="102303" && devicestatus=="00"){
          sdevice[1]=0;
          cdstatus[2]=0;
        }
        if(deviceid=="102303" && devicestatus=="11"){
          sdevice[2]=1;
          cdstatus[2]=1;
          cdevice[2]=timeout_val;
        }
        if(deviceid=="102304" && devicestatus=="00"){
          sdevice[3]=0;
          cdstatus[3]=0;
        }
        if(deviceid=="102304" && devicestatus=="11"){
          sdevice[3]=1;
          cdstatus[3]=1;
          cdevice[3]=timeout_val;
        }
        if(deviceid=="102305" && devicestatus=="00"){
          sdevice[4]=0;
          cdstatus[4]=0;
        }
        if(deviceid=="102305" && devicestatus=="11"){
          sdevice[4]=1;
          cdstatus[4]=1;
          cdevice[4]=timeout_val;
        }
        if(deviceid=="102306" && devicestatus=="00"){
          sdevice[5]=0;
          cdstatus[5]=0;
        }
        if(deviceid=="102306" && devicestatus=="11"){
          sdevice[5]=1;
          cdstatus[5]=1;
          cdevice[5]=timeout_val;
        }
        if(deviceid=="102307" && devicestatus=="00"){
          sdevice[6]=0;
          cdstatus[6]=0;
        }
        if(deviceid=="102307" && devicestatus=="11"){
          sdevice[6]=1;
          cdstatus[6]=1;
          cdevice[6]=timeout_val;
        }
        if(deviceid=="102308" && devicestatus=="00"){
          sdevice[7]=0;
          cdstatus[7]=0;
        }
        if(deviceid=="102308" && devicestatus=="11"){
          sdevice[7]=1;
          cdstatus[7]=1;
          cdevice[7]=timeout_val;
        }
      }
    }
    //Serial.print("RSSI :");         // print RSSI of packet
    //Serial.println(LoRa.packetRssi());
  }

  if(cdevice[0]>minval){
    cdevice[0]=cdevice[0]-1;
  }
  if(cdevice[1]>minval){
    cdevice[1]=cdevice[1]-1;
  }
  if(cdevice[2]>minval){
    cdevice[2]=cdevice[2]-1;
  }
  if(cdevice[3]>minval){
    cdevice[3]=cdevice[3]-1;
  }
  if(cdevice[4]>minval){
    cdevice[4]=cdevice[4]-1;
  }
  if(cdevice[5]>minval){
    cdevice[5]=cdevice[5]-1;
  }
  if(cdevice[6]>minval){
    cdevice[6]=cdevice[6]-1;
  }
  if(cdevice[7]>minval){
    cdevice[7]=cdevice[7]-1;
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("C O M Bb Hh V MT");
  lcd.setCursor(0,1);
  lcd.print(sdevice[0]);
  lcd.print(" ");
  lcd.print(sdevice[1]);
  lcd.print(" ");
  lcd.print(sdevice[2]);
  lcd.print(" ");
  lcd.print(sdevice[3]);
  lcd.print(sdevice[4]);
  lcd.print(" ");
  lcd.print(sdevice[5]);
  lcd.print(sdevice[6]);
  lcd.print(" ");
  lcd.print(sdevice[7]);
  lcd.print(" ");
  lcd.print(motor_st);

  Serial.print(sdevice[0]);
  Serial.print(" ");
  Serial.print(sdevice[1]);
  Serial.print(" ");
  Serial.print(sdevice[2]);
  Serial.print(" ");
  Serial.print(sdevice[3]);
  Serial.print(" ");
  Serial.print(sdevice[4]);
  Serial.print(" ");
  Serial.print(sdevice[5]);
  Serial.print(" ");
  Serial.print(sdevice[6]);
  Serial.print(" ");
  Serial.print(sdevice[7]);
  Serial.print(" ");
  Serial.print(motor_st);
  Serial.println(" ");


  delay(500);
  count=count+1;
  t_count=t_count+1;
  cnt=cnt+1;

  if(count>=15){
    int current_cnt = 0;
    for (int i = 0; i < 8; i++) {
        if (cdevice[i] > 0) {
          current_cnt++;
        }else{
          sdevice[i]=0;
        }
    }
    
    if(current_cnt>=1  && digitalRead(buzzer)==0){
      digitalWrite(buzzer,HIGH);
      motor_st='R';
    }

    if(current_cnt<1  && digitalRead(buzzer)==1){
      digitalWrite(buzzer,LOW);
      motor_st='S';
      ESP.restart();
    }

    if(current_cnt==1){
      digitalWrite(epin,HIGH);
    }

    if(current_cnt>1){
      digitalWrite(epin,LOW);
    }

    if(current_cnt<1){
      for (int i = 0; i < 8; i++) {
        sdevice[i]=0;
      }
    }

    //Serial.println(" ");
    //Serial.print("Current Device:");
    //Serial.println(current_cnt);
    count=0;
  }

  //Serial.println(cnt);
  if(digitalRead(buzzer)==1 && cnt>=10000){
    digitalWrite(buzzer,LOW);
    delay(400*1000);
    ESP.restart();
  }

  if(cnt>=12000){
    cnt=0;
  }

 }