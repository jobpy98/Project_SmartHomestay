#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "AAABBB";
const char* password = "0981195870";
const String host = "192.168.218.120"; 

#include  <SoftwareSerial.h>
SoftwareSerial s_serial(D5, D6); // RX, TX

#include <ArduinoJson.h>

String Ex_String_Read;  // สร้างตัวแปรชื่อ Ex_String_Read ชนิด String เพื่อไว้เก็บข้อความที่จะทำการเก็บ
String user,pass;
int relay1 = D2;
void setup() {
  Serial.begin(9600);
  s_serial.begin(9600);
  pinMode(D2,OUTPUT);
  digitalWrite(relay1,HIGH);
  
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.println("Connected to Network/SSID");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  
  
}

void loop() {
  while(s_serial.available()) {  // วนรับค่าหากด้วย while loop ถ้าหาก Serial.available() (ถ้า Serial ถูกใช้งาน)
    Ex_String_Read = s_serial.readString();  // ใช้คำสั้ง Serial.readString() สำหรับเก็บค่า String ไว้ในตัวแปร Ex_String_Read
    Ex_String_Read.trim();
    Serial.println("Ex_String_Read = |" + Ex_String_Read + "|");
    user = getValue(Ex_String_Read,',',0);
    pass = getValue(Ex_String_Read,',',1);
    Serial.println("user = " + user);
    Serial.println("pass = " + pass);
    postdataToBase();
    delay(5000);
    Ex_String_Read = s_serial.readString();
    digitalWrite(relay1,HIGH);
  }
}

void postdataToBase(){
  HTTPClient http;
 
  //Post Data
  String postData = "user=" + user + "&pass=" + pass ;
  
  http.begin("http://"+host+"/select_control_relay.php");              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
 
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload
 
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  if(httpCode > 0){
//    const size_t bufferSize = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(12) + 370;
//    DynamicJsonBuffer jsonBuffer(bufferSize);
    StaticJsonDocument<200> doc;
    char msg[300];
    strcpy(msg, http.getString().c_str());
    Serial.println("msg : " + String(msg));
    DeserializationError error = deserializeJson(doc, msg);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }else{
      int success = doc["success"];
      if(success == 1){
        digitalWrite(relay1,LOW);
        Serial.println("Open");
        
      }else{
        digitalWrite(relay1,HIGH);
        Serial.println("Close");
      }
      delay(5000);
      digitalWrite(relay1,HIGH);
      Serial.println("Close");
    }
  }
 
  http.end();  //Close connection
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
