#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <TridentTD_LineNotify.h>
#define BLYNK_PRINT Serial

#define LINE_TOKEN "9Tu639wzHyIIQR3awfYdKFvJBJUWCFgb3qt5LHgTUsC"

char auth[] = "xPP2W39ZF1wEIBW3ixox6pjXxscwxcJS";
char ssid[] = "AAABBB";
char pass[] = "0981195870";
BlynkTimer timer;

#define redLED   D5   
#define greenLED D6               
#define DHTPIN   D3              
#define DHTTYPE DHT22           // DHT22 if useDHT22
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  
  Serial.begin(9600);
  dht.begin();
  timer.setInterval(10000, Sensor);
  Blynk.begin(auth, ssid, pass);
  LINE.setToken(LINE_TOKEN);
}
void loop()
{
  Blynk.run(); 
  timer.run(); 
 
  
}

void Sensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(5000);
    return;
  }
  Serial.print("Humidity is: "); 
  Serial.println(h, 1);
  Serial.print("Temperature is: "); 
  Serial.println(t, 1);
  
  Blynk.virtualWrite(V2, h);
  Blynk.virtualWrite(V3, t);
  LINE.notify("Humidity is: "+String(h)+" %"); 
  LINE.notify("Temperature is: "+String(t)+" C"); 
}
