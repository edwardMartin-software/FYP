
#include "certs.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"



// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

const int enable1Pin = 14;
const int motor1Pin1 = 32; 
const int motor1Pin2 = 13;
const int motor2Pin1 = 25; 
const int motor2Pin2 = 26;




WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

void connectedMessage(){
  StaticJsonDocument<200> doc;
  doc["Time"] = String("Hello from ESP32");
  
  char connectedMessage[512]; 
  serializeJson(doc, connectedMessage);
  client.publish(AWS_IOT_PUBLISH_TOPIC, connectedMessage);
}

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  // Create a message handler
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
  connectedMessage();
}
int previousTime = 0;
int startTime = 0;
int newTime = 1;
int rssiDatas[3];
int updateNetworks(){
  int n = WiFi.scanNetworks(); 
  for (int count = 0; count <= 3; count++){
    for (int i = 0; i < n; ++i) {                
        if( WiFi.SSID(i) == "esp1" ){ //another esp32
            rssiDatas[0] = WiFi.RSSI(i);
          }else if( WiFi.SSID(i) == "esp2" ){ //another esp32
            rssiDatas[1] = WiFi.RSSI(i);
          }else if(WiFi.SSID(i) == "esp3" ){ //another esp32
            rssiDatas[2] = WiFi.RSSI(i);
          }
    }
  }
}
void publishMessage()
{
  updateNetworks();
  int sendTime = millis()/1000;
  Serial.print("Sent: ");
  Serial.println(sendTime);
  StaticJsonDocument<200> doc;
  doc["Time"] = sendTime;
  doc["RSSI1"] = rssiDatas[0];
  doc["RSSI2"] = rssiDatas[1];
  doc["RSSI3"] = rssiDatas[2];
  newTime = sendTime + 5;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
  Serial.println("Sent message");
}

signed int value1 = 0;
signed int value2 = 0;
bool start = false;
void messageHandler(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* drive = doc["drive"];
  const char* turn = doc["turn"];
  const char* Rstart = doc["Start"];
  String driveMQTT(drive);
  String turnMQTT(turn);
  String localstart(Rstart);
  if(localstart.equals("true")){ 
   start = true;
  }
  if(localstart.equals("false")){
    start = false;
  }
  if(localstart.equals("done")){
    start = false;
    previousTime = 0;
  }
  int forward = 0;
  int sideways = 0;
  value1 = driveMQTT.toInt();
  value2 = turnMQTT.toInt();

  

  if(value1 < 11 && value1 > -11 && value1 != 0){
    forward = value1;
    Serial.print("forward = ");
    Serial.println(forward);
  }else if(value2 < 11 && value2 > -11 && value2 != 0){
    sideways = value2;
  }else{
    value1 = 0;
    value2 = 0;
  }

 
}


void runMotors(int drive, int turn){
  if(drive > 0){
    Forward(drive);
  }
  if(drive < 0){
    Reverse(drive);
  }
  if(turn > 0){
    Right(turn);
  }
  if(turn < 0){
    Left(turn);
  }
  if( turn == 0 && drive == 0){
    Serial.println("Not Moving!");
    Stop();
  }
}
void Stop(){
  ledcWrite(0, 0);
  ledcWrite(1, 0);
  ledcWrite(2, 0);
  ledcWrite(3, 0);
}
void Forward(int drive){
  Serial.print("drive = ");
  Serial.println(drive);
  ledcWrite(0, (150 + (10*drive) ) );
  ledcWrite(1, 0 );
  ledcWrite(2, (150 + (20*drive) ) );
  ledcWrite(3, 0 );
}
void Reverse(int drive){
  Serial.print("drive = ");
  Serial.println(drive);
  Serial.println((((255/10)*drive)));
  drive = drive * -1;
  ledcWrite(3, (150 + (20*drive) ) );
  ledcWrite(2, 0 );
  ledcWrite(1, (150 + (10*drive) ) );
  ledcWrite(0, 0 );
}
void Left(int turn){
  Serial.print("turn = ");
  Serial.println(turn);
  turn = turn * -1;
  ledcWrite(0, (150 + (20*turn) ) );
  ledcWrite(1, 0 );
  
}
void Right(int turn){
  Serial.print("turn = ");
  Serial.println(turn);
  ledcWrite(2, (150 + (20*turn) ) );
  ledcWrite(3, 0 );
}
void setup() {
  Serial.begin(9600);
  pinMode(enable1Pin, OUTPUT);
  ledcAttachPin(motor1Pin1, 0); // assign motor pins to a channel
  ledcAttachPin(motor1Pin2, 1);
  ledcAttachPin(motor2Pin1, 2); // assign motor pins to a channel
  ledcAttachPin(motor2Pin2, 3);
  ledcSetup(0, 4000, 8); // 12 kHz PWM, 8-bit resolution
  ledcSetup(1, 4000, 8);
  ledcSetup(2, 4000, 8); // 12 kHz PWM, 8-bit resolution
  ledcSetup(3, 4000, 8);
  connectAWS();
}

void loop() {
  digitalWrite(enable1Pin, HIGH);
  if(previousTime == 0 && start == true){
    previousTime = millis()/1000;
    publishMessage();
  }
  if(previousTime == newTime && start == true){
    publishMessage();
  }
  if(start == false){
    previousTime = 0;
  }else{
    previousTime = millis()/1000;
  }

  runMotors(value1,value2);
  client.loop();
  
}


   
