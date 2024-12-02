
#include <WiFi.h>
#include <ThingsBoard.h>
#include <Arduino_MQTT_Client.h>
#include <string.h>
#define LED_BUILTIN 15

#define WIFI_AP "MRVINA"
#define WIFI_PASS "phamcaoky@1234"

#define TB_SERVER "thingsboard.cloud"
#define TOKEN "EFgw0C30e91cnM89l7Ih"
constexpr uint16_t MAX_MESSAGE_SIZE = 128U;

WiFiClient espClient;
Arduino_MQTT_Client mqttClient(espClient);
ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE);

const int relayPins[] = {23, 22, 4, 5, 18, 19, 21};
int relayIndex = 3;

typedef enum {ON,OFF} state_t;
static state_t state=OFF;

char str[10];
/////////////////////////////////////////////////////////////////////////////
void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  int attempts = 0;
  
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    WiFi.begin(WIFI_AP, WIFI_PASS, 6);
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect to WiFi.");
  } else {
    Serial.println("\nConnected to WiFi");
  }
}

void connectToThingsBoard() {
  if (!tb.connected()) {
    Serial.println("Connecting to ThingsBoard server");
    
    if (!tb.connect(TB_SERVER, TOKEN)) {
      Serial.println("Failed to connect to ThingsBoard");
    } else {
      Serial.println("Connected to ThingsBoard");
    }
  }
}

void sendDataToThingsBoard(int relay, int state) {
  if(state==ON) strcpy(str, "ON");
  else if(state==OFF) strcpy(str, "OFF");
  String jsonData = "{\"Relay\":" + String(relay) + ", \"State\":" + String(state) + ", \"String\":" + String(str) +"}";
  tb.sendTelemetryJson(jsonData.c_str());
}
void setup() {
  Serial.begin(9600);
  connectToWiFi();
  connectToThingsBoard();

  pinMode(LED_BUILTIN, OUTPUT);
  for (int i = 0; i < 7; i++) {
    pinMode(relayPins[i], OUTPUT);
    delay(200);
  }
}

void loop() {
  if(state==OFF){
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(relayPins[relayIndex], LOW);
    state=ON;
    delay(3000);
  }else{
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(relayPins[relayIndex], HIGH);
    state=OFF;
    delay(3000);
  }
  sendDataToThingsBoard(relayIndex, state);
}
