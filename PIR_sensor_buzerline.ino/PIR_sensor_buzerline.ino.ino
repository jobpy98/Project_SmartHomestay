#include <Blynk.h>

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClientSecureAxTLS.h>

void Line_Notify(String message) ;

#define LINE_TOKEN "VYk91BFcxwjLqxzhzwZIcLhVaa6y1kJ1UReuxA0NxrZ"// line TOKEN 
 char auth[] = "onsDuMROQ9fbYl5RE1XOGnqO4kDZpg8i"; // Blynk TOKEN 
 char ssid[] = "AAABBB";
 char pass[] = "0981195870";
 
 int PIR    = D1;
 int Buzzer = D2;
 int PIRStatus = 0;
 int Status = 0;
  WidgetLCD LCD(V0);
  WidgetLED LED(V1);
  
void setup()
 {
 Serial.begin(9600);
 pinMode(PIR,INPUT);
 pinMode(Buzzer,OUTPUT);
 Blynk.begin(auth, ssid, pass);
 }


 BLYNK_WRITE(V2){ 
  Serial.println(param.asInt()); 
   Serial.println(Status); 
    if (param.asInt()){
      Status = 1;
      LCD.clear();
      LCD.print(1,0,"On The System"); 
      LCD.print(4,1,"<<Mode>>");
      LED.off();
     } else {  
     Status = 0;  
      LED.off();
      LCD.clear(); 
      LCD.print(1,0,"Off The System"); 
      LCD.print(4,1,"<<Mode>>");
      digitalWrite(Buzzer, LOW);
 }
}

void loop(){
  Blynk.run();
  if (Status == 1){
    if (digitalRead(PIR)){
      LCD.clear(); 
      LCD.print(2,0,"Some One Come"); 
      LCD.print(4,1,"<<Danger>>");  
      if (!PIRStatus){
        PIRStatus = 1;
        digitalWrite(Buzzer, HIGH);
        Blynk.notify("Some One Come");
        Line_Notify("Some One Come");
        LED.on();
        delay(1000);
       LCD.clear(); 
       LCD.print(1,0,"Everything OK "); 
       LCD.print(4,1,"<< OK >>");  
        digitalWrite(Buzzer, LOW);  
        LED.off();
        PIRStatus = 0;
        LED.off();
    }
  }
  }
  else
  {
    LED.off();
     Serial.println("off");
  }
 }

 void Line_Notify(String message) {
  axTLS::WiFiClientSecure client; // กรณีขึ้น Error ให้ลบ axTLS:: ข้างหน้าทิ้ง

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");  
    return;
  } 
  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Connection: close\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  // Serial.println(req);
  client.print(req);
    
  delay(20);

  // Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
}
