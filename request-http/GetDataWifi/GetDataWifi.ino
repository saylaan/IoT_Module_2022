// inclusion des bibliothèques utiles
// pour la communication WiFi
#include "WiFi.h"
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>

// modifiez ces deux constantes pour qu'elles contiennent les caractéristiques de
// votre réseau Wifi
#define ssid      "tamer"       // le nom (SSID) de votre réseau WiFi
#define password  "123oui123"  // votre mot de passe WiFi
String hostname = "ESP32 Device v0.1";
String serverName;
int value = 0;

void setup() {
  
  // pour affichage dans le moniteur série
  pinMode(23, OUTPUT);
  Serial.begin ( 115200 );
  Serial.println("");

  digitalWrite(23, HIGH);
  delay (500);
  digitalWrite(23, LOW);
  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(hostname.c_str()); //define hostname
  // initialisation de la communication WiFi
  WiFi.begin ( ssid, password );
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); Serial.print ( "." );
  }
  Serial.println ( "" );
  Serial.print ( "Maintenant connecte a " );
  Serial.println ( ssid );
  Serial.print ( "Adresse IP: " );
  Serial.println ( WiFi.localIP() );
  Serial.println ( WiFi.dnsIP() ); // <= pour avoir l'ip de la gateway et donc faire les requetes HTTP ( ne pas oublier le port )
  serverName = String( String("http://") + WiFi.dnsIP().toString() + String(":8080/"));  

  String addDeviceUrl = serverName + String("device");
  HTTPClient http;
  http.begin(addDeviceUrl.c_str());
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST("{\"name\":\"" + hostname + "\",\"details\":\"Première version test\",\"ip\":\""+ WiFi.macAddress() +"\",\"version\":\"0.1\",\"status\":\"1\"}");
  if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
    http.end();
}

void loop() {
  String demoUrl = serverName + String("demo");
  HTTPClient http;
  http.begin(demoUrl.c_str());
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.GET();
  if (httpResponseCode>0)
  {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String status = http.getString();
        Serial.println(status);
      if (status == "on")
      {
      digitalWrite(23, HIGH);
      }
      else
      {
        digitalWrite(23, LOW);
      }
   }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      
      
      // Free resources
    http.end();
}
