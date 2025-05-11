/*
#include <ESP8266WiFi.h>

void setup()
{
  Serial.begin(115200);
  Serial.println();

  WiFi.begin("Airtel_9764005401", "air46403_");

  Serial.print("Connecting");
  int temp_count=0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    temp_count++;
    Serial.println(temp_count);
    if(temp_count>=20){
        //break;
        ESP.deepSleep(0);
    }
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {}

*/

#include "WiFi.h"
#include "espnow.h"

void setup()
{
  Serial.begin(115200);
  Serial.println();
  WiFi.mode(WIFI_MODE_STA);
  esp_wifi_set_protocol( WIFI_IF_STA , WIFI_PROTOCOL_LR);

  //WiFi.mode(WIFI_STA);
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  WiFi.disconnect();
  delay(100);
}

void loop()
{
  Serial.print("Scan start ... ");
  int n = WiFi.scanNetworks();
  Serial.print(n);
  Serial.println(" network(s) found");
  for (int i = 0; i < n; i++)
  {
    Serial.println(WiFi.SSID(i));
  }
  Serial.println();

  delay(5000);
}