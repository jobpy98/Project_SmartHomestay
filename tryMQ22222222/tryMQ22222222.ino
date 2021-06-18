#include <ESP8266WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>

#define wifi_ssid "AAABBB"
#define wifi_password "0981195870"
#define mqtt_server "35.240.156.35"
#define mqtt_user "root"
#define mqtt_password "12345678"

#define LDR "sensor/mq2"

WiFiClient espClient;
PubSubClient client(espClient);
int led = D1;
int buzzer = D4;
int smokeA0 = A0;
 //Your threshold value
int sensorThres = 150;
void setup() {
   pinMode(led, OUTPUT);
   pinMode(buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Generate client name based on MAC address and last 8 bits of microsecond counter
    String clientName;
    clientName += "esp8266-";
    uint8_t mac[6];
    WiFi.macAddress(mac);
    clientName += macToStr(mac);
    clientName += "-";
    clientName += String(micros() & 0xff, 16);
    Serial.print("Connecting to ");
    Serial.print(mqtt_server);
    Serial.print(" as ");
    Serial.println(clientName);


    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    if (client.connect((char*) clientName.c_str())) {
      //if (client.connect((char*) clientName.c_str()), mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(500);
    }
  }
}


void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Wait a few seconds between measurements.
  delay(1000);

  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  delay(1000);
  
 int analogSensor = analogRead(smokeA0);
  Serial.print("LPG : ");
  Serial.println(String(sensorValue).c_str());
  client.publish(LDR, String(sensorValue).c_str(), true);
  if (analogSensor > sensorThres)
  {
     tone(buzzer, 1000, 200);
   digitalWrite(buzzer, LOW);
   digitalWrite(led, HIGH);
  }
  else { 
    digitalWrite(buzzer, HIGH);
    noTone(buzzer);
    digitalWrite(led, LOW);
  }
  delay(1000);

}
