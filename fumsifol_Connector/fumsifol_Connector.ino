/*
 * copyright: ayoade adetunji 2017
 */
 
#include <EEPROM.h>
#include <ESP8266WiFi.h>
    #define doorStatus A0     //
    #define connStatus 16     //D0
    #define openClose 5       //D1
    #define beeper 4          //D2
    #define D3 0
    #define D4 2
    #define D5 14
    #define D6 12
    #define D7 13
    #define D8 15
    #define D9 3
    #define D10 1

boolean statusConn = false;

WiFiServer server(8080);      // Create an instance of the server and specify the port to listen on as an argument

IPAddress staticIP(192, 168, 8, 103); 
IPAddress gateway(192, 168, 8, 1); 
IPAddress subnet(255, 255, 255, 0); 

const char* ssid     = "REPENT";
const char* password = "psalm121";

void setup() {
  Serial.begin(115200);
  pinMode(doorStatus, INPUT);
  pinMode(openClose, OUTPUT);
  pinMode(beeper, OUTPUT);
  pinMode(connStatus, OUTPUT);

  digitalWrite(openClose, HIGH);
  digitalWrite(beeper, HIGH);
  Serial.println();
  Serial.print("openclose high");
  
  WiFi.config(staticIP, gateway, subnet);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  server.begin();
  Serial.println();
  Serial.print("Connecting to Access Point:");
  Serial.println(WiFi.SSID());
  Serial.println("Connected on:");
  Serial.println(WiFi.localIP());
  statusConn = true;
}

void loop() {
  if(statusConn){
  digitalWrite(connStatus, HIGH);
  WiFiClient client = server.available();
  if (!client) {
     if(doorOpen()){
    client.print("OPEN");
    Serial.println("Door open");
  }else{
    client.print("CLOSE");
    Serial.println("Door close");
  }
  delay(1000);
    return;
  }
  // Wait until the client sends some data
  while(!client.available()){
   
    delay(1);
  }

  // Read the first line of the request
  String req = client.readString();
  delay(5);
  Serial.println(req.substring(2,10));
  processReq(req.substring(2,10));
  client.flush();
  delay(10);

  if(doorStatus){
    client.print("OPEN");
    Serial.print("Door open");
  }else{
    client.print("CLOSE");
    Serial.print("Door close");
  }
  
  client.flush();
  Serial.flush();
  delay(1);
  }else{
   digitalWrite(connStatus, HIGH);
   delay(500);
   digitalWrite(connStatus, LOW);
   delay(500);
   return;
  }
}

void processReq(String task){
  Serial.println(task);
  if(task.equals("open1500")){
    digitalWrite(openClose, LOW);
    Serial.println("open");
    delay(6000);
    digitalWrite(openClose, HIGH);
  }else if(task.equals("close400")){
    digitalWrite(openClose, LOW);
    Serial.println("close");
  }else if(task.equals("beepON10")){
    Serial.println("beep on");
    digitalWrite(beeper, HIGH);
  }else if(task.equals("beepOFF1")){
    Serial.println("beep off");
    digitalWrite(beeper, LOW);
  }else if(task.equals("lose400")){
    digitalWrite(openClose, LOW);
    Serial.println("close");
  }else if(task.equals("pen1500")){
    digitalWrite(openClose, LOW);
    Serial.println("open");
    delay(6000);
    digitalWrite(openClose, HIGH);
  }else if(task.equals("eepON10")){
    Serial.println("beep on");
    digitalWrite(beeper, HIGH);
  }else if(task.equals("eepOFF1")){
    Serial.println("beep off");
    digitalWrite(beeper, LOW);
  }
  
}

boolean doorOpen(){
  float outV = analogRead(A0);
  float resistance = 3.3*(outV/1023.0);
  if(resistance>1)return true;
  return false;
}

