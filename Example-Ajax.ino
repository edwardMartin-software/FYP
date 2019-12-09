/*
 * ESP32 AJAX Demo
 * Updates and Gets data from webpage without page refresh
 * https://circuits4you.com
 */
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include "Ajax Header.h"  //Web page header file

WebServer server(80);

const int motor1Pin1 = 12; 
const int motor1Pin2 = 27; 
const int motor2Pin1 = 25; 
const int motor2Pin2 = 26; 
const int enable1Pin = 14; 

//Enter your SSID and PASSWORD
const char* ssid = "nubia Z17 lite";
const char* password = "backrow8";

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

 int count = 0;
 int previousRSSI;
 String networks = "Locating";
void updateNetworks(){
  int n = WiFi.scanNetworks(); 
  

  for(int i = 0; i < n; i++){
    if(WiFi.SSID(i) == "nubia Z17 lite"){
      if(count == 0){
        previousRSSI = WiFi.RSSI(i);
      }else{
        if(WiFi.RSSI(i) > previousRSSI + 3 ){
          networks = "moving towards ";//+String(WiFi.RSSI(i)) + "> previous: "+String(previousRSSI);
        }
        else if( WiFi.RSSI(i) < previousRSSI - 3 ){
          networks = "moving away from";//+WiFi.SSID(i);
        }else{
          networks = "not moving";
        }
        previousRSSI = WiFi.RSSI(i);
      }
//      networks += " "+String(WiFi.RSSI(i)) + " "+ String(count);
      
    }
  }
 
 count++;
 
// String adcValue = String(a);
 
 server.send(200, "text/plane", networks); //Send ADC value only to client ajax request
}

//===============================================================
// Setup
//===============================================================

void setup(void){
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

/*
//ESP32 As access point
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);
*/
//ESP32 connects to your wifi -----------------------------------
  WiFi.mode(WIFI_STA); //Connectto your wifi
  WiFi.begin(ssid, password);

  Serial.println("Connecting to ");
  Serial.print(ssid);

  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
      Serial.print(".");
    }
    
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
//----------------------------------------------------------------
 
  server.on("/", handleRoot);      //This is display page
  server.on("/Networks", updateNetworks);//To get update of location only
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
int farFromCount = 5;
void loop(void){
  server.handleClient();
  delay(1);
//  if(farFromCount < 10 && farFromCount > 0){
//    if(networks == "not moving"){
//      Serial.println("motors off");
//      
//    }
//    if(networks == "moving away from"){
//      Serial.println("motors on");
//      farFromCount= farFromCount + 1;
//      
//      
//    }
//    if(networks == "moving towards"){
//      Serial.println("motors on");
//      farFromCount = farFromCount-1;
//      
//    }
//    Serial.println("farFromCount: "+ String(farFromCount));
//    delay(1900);
//  }else{
//    Serial.println("out of range");
//  }
}
