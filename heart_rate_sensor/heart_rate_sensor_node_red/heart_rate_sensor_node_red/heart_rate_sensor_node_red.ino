#include <PubSubClient.h>
#include <WiFi.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS 1000
#define Subscribe "sensor/receive"
#define Publish "sensor/send"

const char* ssid = "Ohm";
const char* password = "123456789";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* topic = "sensor/receive";
float BPM;
unsigned long initialMillis = millis();
const char* mqttClient = "ohm_arthakorn";
const char* mqttUsername = "";
const char* mqttPassword = "";

String DataString;
long lastMsg = 0;
int value = 0;
char msg[100];


WiFiClient espClient;
PubSubClient client(espClient);
PulseOximeter pox;

uint32_t tsLastReport = 0;

// * ฟังก์ชันสำหรับการเชื่อมต่อ MQTT เข้ากับ Node Red
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqttClient, mqttUsername, mqttPassword)) {
      Serial.println("Connected !");
      client.subscribe(Subscribe);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Try again in 1 seconds !");
      delay(1000);
    }
  }
}

void connectedWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;

  for (int i = 0; i < length; i++) {
    message = message + char(payload[i]);
  }

  Serial.println(message);
}

void sendHeartBeat() {
  pox.update();
  BPM = pox.getHeartRate();
  lastMsg = initialMillis;
  ++value;
  DataString = "{ \"BPM\": " + String(pox.getHeartRate()) + "}";
  DataString.toCharArray(msg, 100);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish(Subscribe, msg);
  delay(1);
  client.loop();
}

void onBeatDetected() {
  Serial.println("Beat!");
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart rate:");
    Serial.print(pox.getHeartRate());
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");

    // Run function for sending value here!
    sendHeartBeat();
    tsLastReport = millis();
  }
}


void setup() {
  Serial.begin(115200);
  Serial.print("Initializing pulse oximeter..");

  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;)
      ;
  } else {
    Serial.println("SUCCESS");
  }
  // Function that when to put finger to sensor that'll back to this function to do the task!
  // เมื่อคุณเอานิ้วมาวางบน Sensor มันจะกลับมาทำงานที่ฟังก์ชันนี้
  pox.setOnBeatDetectedCallback(onBeatDetected);
  connectedWiFi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  client.subscribe(Subscribe);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  pox.update();
}
