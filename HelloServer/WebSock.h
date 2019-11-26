#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "scopeLam.h"
#include "page2.h"
#include "motorPage.h"
#include "updateNumber.h"

const char* ssid     = "nubia Z17 lite";
const char* password = "backrow8";
WebServer server(80);   //instantiate server at port 80 (http port)

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
 
  server.on("/home", [](){
    server.send(200, "text/html", homePage);
    Serial.println("Homepage sent");
  });
   server.on("/update", [](){
    server.send(200, "text/html", updateNumber);
    Serial.println("updateNumber sent");
  });

  server.on("/motor", [](){
    server.send(200, "text/html", motorPage);
    Serial.println("motorPage sent");
  });
 
  server.on("/page2", [](){
    server.send(200, "text/html", page2);
    Serial.println("page2 sent");
  });

  server.begin(); 
  Serial.println("Web server started!");
}
 
void wsLoop(){
  server.handleClient();
}
 
