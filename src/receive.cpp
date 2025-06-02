// RECEIVER V1.2 WITH MESSAGE
#include <ESP8266WiFi.h>
#include <espnow.h>

// MAC Address of the SENDER
uint8_t broadcastAddress[] = {0xA4, 0xCF, 0x12, 0xF3, 0x8F, 0xB0};

const long interval = 10000;
unsigned long previousMillis = 0;

#define BUTTON D3
#define led_state D4

typedef struct struct_message {
  int button;
  char message[32];
} struct_message;

int IN_button_state;
int OUT_button_state;

struct_message outgoingmsg;
struct_message incomingmsg;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
 // Serial.print("Last Packet Send Status: ");
//   if (sendStatus == 0) {
//     Serial.println("Delivery success");
//   } else {
//     Serial.println("Delivery fail");
//   }
}

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&incomingmsg, incomingData, sizeof(incomingmsg));
  Serial.print("Bytes received: ");
  Serial.println(len);

  IN_button_state = incomingmsg.button;
  Serial.print("Received button: ");
  Serial.println(IN_button_state);
  Serial.print("Received message: ");
  Serial.println(incomingmsg.message);

  
  
 if(IN_button_state==1){
    digitalWrite(led_state,HIGH);
  }
  else{
    digitalWrite(led_state,LOW);}
}

 void getReadings() {
   OUT_button_state = digitalRead(BUTTON);
//   Serial.println("OUTGOING MESSAGE");
//   Serial.println(OUT_button_state);
 }

 void printIncomingMessage() {
//   Serial.println("INCOMING MESSAGE");
//   Serial.println(IN_button_state);
//   Serial.print("Message: ");
//   Serial.println(incomingmsg.message);
 }

void setup() {
  Serial.begin(115200);
 // pinMode(BUTTON, INPUT_PULLUP);
  pinMode(led_state, OUTPUT);
  digitalWrite(led_state, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    getReadings();

    outgoingmsg.button = OUT_button_state;
    if (IN_button_state==0)
    {
         strcpy(outgoingmsg.message, "LED ON");
    }
    else
    {
         strcpy(outgoingmsg.message, "LED OFF");
    }
    
    
   

    esp_now_send(broadcastAddress, (uint8_t *) &outgoingmsg, sizeof(outgoingmsg));

    printIncomingMessage();
  }
}
