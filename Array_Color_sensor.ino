#include <WiFi.h>
#include <MQTT.h>
#define S0 27
#define S1 26
#define S2 25
#define S3 33
#define sensorOut 32
int frequency = 0;

const char ssid[] = "Aaditya";
const char pass[] = "Aaditya123";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "try", "try")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/red");
  // client.unsubscribe("/red");
  client.subscribe("/blue");
  // client.unsubscribe("/blue");
  client.subscribe("/green");
  // client.unsubscribe("/green");
  client.subscribe("/clear");
  // client.unsubscribe("/Clear");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // You need to set the IP address directly.
  client.begin("192.168.1.11", net);
  client.onMessage(messageReceived);

  connect();
pinMode(S0, OUTPUT);
pinMode(S1, OUTPUT);
pinMode(S2, OUTPUT);
pinMode(S3, OUTPUT);
pinMode(sensorOut, INPUT);
 
// Setting frequency-scaling to 20%
digitalWrite(S0,HIGH);
digitalWrite(S1,HIGH);
 
Serial.begin(9600);
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability
// Setting red filtered photodiodes to be read
digitalWrite(S2,LOW);
digitalWrite(S3,LOW);
// Reading the output frequency
frequency = pulseIn(sensorOut, LOW);
// Printing the value on the serial monitor
String fit = String(frequency, DEC);
delay(200);
if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 200) {
    lastMillis = millis();
    client.publish("/red",fit);
  }
// Setting Green filtered photodiodes to be read
digitalWrite(S2,HIGH);
digitalWrite(S3,HIGH);
// Reading the output frequency
frequency = pulseIn(sensorOut, LOW);
// Printing the value on the serial monitor
String fet = String(frequency, DEC);
delay(200);
if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 200) {
    lastMillis = millis();
    client.publish("/green",fet);
  }
// Setting Clear filtered photodiodes to be read
digitalWrite(S2,HIGH);
digitalWrite(S3,LOW);
// Reading the output frequency
frequency = pulseIn(sensorOut, LOW);
// Printing the value on the serial monitor
String fot = String(frequency,DEC);
delay(200);
if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 200) {
    lastMillis = millis();
    client.publish("/clear",fot);
  }
// Setting Blue filtered photodiodes to be read
digitalWrite(S2,LOW);
digitalWrite(S3,HIGH);
// Reading the output frequency
frequency = pulseIn(sensorOut, LOW);
// Printing the value on the serial monitor
String fat = String(frequency,DEC);
delay(200);
  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 200) {
    lastMillis = millis();
    client.publish("/blue",fat);
  }
}
