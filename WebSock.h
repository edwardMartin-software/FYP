#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "updateNumber.h"

const char* ssid     = "House Fellas";
const char* password = "27BallybritCourt";
WebServer server(80);   //instantiate server at port 80 (http port)
int n = WiFi.scanNetworks();
String networks;
void updateNetworks(){
  n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            delay(10);
        }
  networks = n;
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
    server.send(200, "text/html", networks);
    
    Serial.println(networks);
  });
  
  server.begin(); 
  Serial.println("Web server started!");
}
 
void wsLoop(){
  server.handleClient();
}
 
