#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

// ============ CONFIG ============
const char* WIFI_SSID = "replace w wifi name";
const char* WIFI_PASS = "replace w wifi password";
const char* MQTT_SERVER = "replace with ip/mqtt server address";
const int MQTT_PORT = 1883;
const char* MQTT_TOPIC = "orion/flick";

const int SERVO_PIN = 18;
const int SERVO_ON_ANGLE = 150;
const int SERVO_OFF_ANGLE = 90;
const int SERVO_NEUTRAL = 120;

// ============ OBJECTS ============
WiFiClient espClient;
PubSubClient mqtt(espClient);
Servo servo;

// ============ MQTT CALLBACK ============
void onMessage(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  message.toLowerCase();

  Serial.print("Received: ");
  Serial.println(message);

  if (message == "on") {
    Serial.println("Flipping switch ON");
    servo.write(SERVO_ON_ANGLE);
    delay(500);
    servo.write(SERVO_NEUTRAL);
  } 
  else if (message == "off") {
    Serial.println("Flipping switch OFF");
    servo.write(SERVO_OFF_ANGLE);
    delay(500);
    servo.write(SERVO_NEUTRAL);
  }
}

// ============ WIFI CONNECT ============
void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP: ");
  Serial.println(WiFi.localIP());
}

// ============ MQTT CONNECT ============
void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Connecting to MQTT...");
    if (mqtt.connect("flick")) {
      Serial.println("connected!");
      mqtt.subscribe(MQTT_TOPIC);
      Serial.print("Subscribed to: ");
      Serial.println(MQTT_TOPIC);
    } else {
      Serial.print("failed (");
      Serial.print(mqtt.state());
      Serial.println(") retrying in 5s...");
      delay(5000);
    }
  }
}

// ============ SETUP ============
void setup() {
  Serial.begin(115200);
  Serial.println("Flick starting up...");

  servo.attach(SERVO_PIN);
  servo.write(SERVO_NEUTRAL);

  connectWiFi();

  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(onMessage);
  connectMQTT();

  Serial.println("Flick ready — waiting for commands from Orion");
}

// ============ LOOP ============
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  if (!mqtt.connected()) {
    connectMQTT();
  }
  
  mqtt.loop();
}