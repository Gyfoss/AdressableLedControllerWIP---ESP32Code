#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

void handleRoot() {
  // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN, HIGH);
  
    // wait for a second
    delay(3000);
  
    // turn the LED off by making the voltage LOW
    digitalWrite(LED_BUILTIN, LOW);
}

void handleGet() {
  for (size_t i = 0; i < 50; i++)
  {
    // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN, HIGH);
  
    // wait for a second
    delay(100);
  
    // turn the LED off by making the voltage LOW
    digitalWrite(LED_BUILTIN, LOW);
  
    // wait for a second
    delay(100);
  }
  
}

void handlePost() {
    /*
    digitalWrite(LED_BUILTIN, HIGH);
    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    */
    if (server.hasArg("ssid") && server.hasArg("password"))
    {
      String ssid = server.arg("ssid");
      String password = server.arg("password"); 
      Serial.print(ssid + " " + password + "\n");
    }
}

void handleUpload() {
    
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  delay(250);
  WiFi.softAP("ESP32", "passworld");
  server.on("/", handleRoot);
  server.on("/get", HTTP_GET, handleGet);
  server.on("/post", HTTP_POST, handlePost);
  server.begin();
}

void loop() {
  server.handleClient();
}