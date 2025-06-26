#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

// WiFi credentials
const char* ssid = "Anupam";
const char* password = "12345678";

// Firebase Realtime Database URL (your own node path)
const char* serverName = "https://esp-02.asia-southeast1.firebasedatabase.app/users/uid/10103.json";

// Data variables
int sdevice[8] = {0};
char motor_st = 'S';

// Function declarations
void sendDataToFirebase();
void checkWiFiConnection();

void setup() {
  Serial.begin(115200);
  Serial.println("\nESP8266 Receiver Starting...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    Serial.print(".");
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi.");
  }
}

void loop() {
  checkWiFiConnection(); // Ensure WiFi stays connected

  // Read serial input when available
  if (Serial.available()) {
    String received = Serial.readStringUntil('\n');
    received.trim();

    if (received.length() == 9) { // 8 digits + 1 char
      for (int i = 0; i < 8; i++) {
        char c = received.charAt(i);
        if (c >= '0' && c <= '9') {
          sdevice[i] = c - '0';
        } else {
          sdevice[i] = 0; // fallback for invalid character
        }
      }
      motor_st = received.charAt(8);

      // Debug print
      Serial.print("Received sdevice: ");
      for (int i = 0; i < 8; i++) {
        Serial.print(sdevice[i]);
        Serial.print(" ");
      }
      Serial.print(" motor_st: ");
      Serial.println(motor_st);

      // Send to Firebase
      sendDataToFirebase();
    } else {
      Serial.println("Invalid data length received.");
    }
  }
}

// Check WiFi connection and reconnect if needed
void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Attempting reconnection...");
    WiFi.begin(ssid, password);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10) {
      delay(500);
      Serial.print(".");
      attempts++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nReconnected to WiFi.");
    } else {
      Serial.println("\nReconnection failed.");
    }
  }
}

// Send JSON data to Firebase
void sendDataToFirebase() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure(); // Skip certificate verification (not recommended for production)

    HTTPClient https;
    https.begin(client, serverName);
    https.addHeader("Content-Type", "application/json");

    // Build JSON
    String jsonData = "{";
    jsonData += "\"sdevice\":[";
    for (int i = 0; i < 8; i++) {
      jsonData += String(sdevice[i]);
      if (i < 7) jsonData += ",";
    }
    jsonData += "],";
    jsonData += "\"motor_st\":\"" + String(motor_st) + "\"";
    jsonData += "}";

    Serial.println("Sending to Firebase: " + jsonData);

    int httpResponseCode = https.PUT(jsonData);

    if (httpResponseCode > 0) {
      Serial.print("Firebase response code: ");
      Serial.println(httpResponseCode);
      Serial.println(https.getString());
    } else {
      Serial.print("Error sending to Firebase: ");
      Serial.println(httpResponseCode);
    }

    https.end();
  } else {
    Serial.println("WiFi not connected. Cannot send data.");
  }
}
