#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "updateNumber.h"

const char* ssid     = "House Fellas";
const char* password = "27Ballybrit";
WebServer server(80);   //instantiate server at port 80 (http port)
int n = WiFi.scanNetworks();
String networks = "";
void updateNetworks(){
  n = WiFi.scanNetworks();
  networks = "";
  for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            
            
            if( WiFi.SSID(i) == "House Fellas" ){ //home wifi
              networks = networks + WiFi.SSID(i) + ":" + WiFi.RSSI(i)+ "\n";
              Serial.println(networks + WiFi.SSID(i) + ":" + WiFi.RSSI(i)+ "\n");
            }
            if( WiFi.SSID(i) == "nubia Z17 lite" ){ // mobile hotspot
              networks = networks + WiFi.SSID(i) + ":" + WiFi.RSSI(i)+ "\n";
              Serial.println(networks + WiFi.SSID(i) + ":" + WiFi.RSSI(i)+ "\n");
            }
            if( WiFi.SSID(i) == "yourAP" ){ //another esp32
              networks = networks + WiFi.SSID(i) + ":" + WiFi.RSSI(i)+ "\n";
              Serial.println(networks + WiFi.SSID(i) + ":" + WiFi.RSSI(i)+ "\n");
            }
            delay(10);
        }
        
  
}
void wsSetup(){
  Serial.print("Setting AP (Access Point)…");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   updateNetworks();
     server.on("/update", [](){
      server.send(200, "text/html", "<head>  <meta http-equiv=\"refresh\" content=\"4\"></head>" + networks);
      
      
    });
  
  server.begin(); 
  Serial.println("Web server started!");
}
 
void wsLoop(){
  server.handleClient();
   

    updateNetworks();
//    delay(5000);
//  }
  
}
 
