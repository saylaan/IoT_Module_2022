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
String hostname = "ESP32 Sensor v0.1";
String serverName;
int value = 0;
int state = 1;
int prev1 = 1;
int prev2 = 1;
int prev3 = 1;

int prev4 = 1;
int prev5 = 1;
int prev6 = 1;

int prev7 = 1;
int prev8 = 1;
int prev9 = 1;

void setup() {
  
  // pour affichage dans le moniteur série
  pinMode(23, OUTPUT);
pinMode(18, OUTPUT);
pinMode(19, INPUT);
  Serial.begin ( 115200 );
  Serial.println("");
  
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

  String addSensorUrl = serverName + String("sensor");
  HTTPClient http;
  http.begin(addSensorUrl.c_str());
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST("{\"name\":\"" + hostname 
  + "\",\"details\":\"Première version test\",\"ip\":\""+ WiFi.macAddress() +"\"}");
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
  state = digitalRead(19);
  Serial.println(state);
  String data;
  if (state == 0 /*&& prev1 == 0 && prev2 == 0 && prev3 == 0 && prev4 == 0 && 
  prev5 == 0 && prev6 == 0 && prev7 == 0 && prev8 == 0 && prev9 == 0*/) 
  {
   data = "{\"detected\":\"yes\"}";
   digitalWrite(23, HIGH);
  }
  else
  {
   data = "{\"detected\":\"no\"}";
   digitalWrite(23, LOW);
  }
 /* prev9 = prev8;
  prev8 = prev7;
  prev7 = prev6;
  prev6 = prev5;
  prev5 = prev4;
  prev4 = prev3;
  prev3 = prev2;
  prev2 = prev1;
  prev1 = state;
 */
 
  int httpResponseCode = http.POST(data);
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
