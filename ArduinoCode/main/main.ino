#include <WiFiEsp.h>
#include <SoftwareSerial.h>
#include <PubSubClient.h>
 
#define WIFI_SSID "Cudy-D784"
#define WIFI_PASSWORD "244466666"
#define RX_PIN 2
#define TX_PIN 3
#define TMP A0

SoftwareSerial esp(RX_PIN, TX_PIN); // rx, tx
WiFiEspClient wifiClient;
PubSubClient mqttClient(wifiClient);

const char* mqttServer = "192.168.10.101";
const int mqttPort = 1883;
const char* mqttTopic = "sensors_data";

void setup() {
  Serial.begin(9600);
  esp.begin(9600);
  WiFi.init(&esp);
  
  while (WiFi.begin(WIFI_SSID, WIFI_PASSWORD) != WL_CONNECTED);
  
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  mqttClient.setServer(mqttServer, mqttPort);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }

  mqttClient.loop();
  sendData();
  delay(10000);
}

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (mqttClient.connect("ArduinoClient")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void sendData() {
  float tmp_sensor = analogRead(TMP); // Example temperature value, replace with your actual data
  float tmp_voltage = (tmp_sensor / 1024.0) * 5.0;
  float temperature = (tmp_voltage - 0.5) * 100;
  // Convert the temperature to a string
  char temperatureString[6];
  dtostrf(temperature, 4, 2, temperatureString);

  Serial.print("Sending temperature: ");
  Serial.println(temperatureString);

  mqttClient.publish(mqttTopic, temperatureString);
}
