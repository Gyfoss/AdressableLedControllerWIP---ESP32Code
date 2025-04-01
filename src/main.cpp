#include <WiFi.h>
#include <WebServer.h>
#include "SPIFFS.h"
#include "FS.h"

using namespace std;

#define BUTTON_PIN       14
#define TX_PIN           1
#define RX_PIN           3
#define SHORT_PRESS_TIME 5000

int lastState = HIGH;  
int currentState;
unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;

WebServer server(80);

void removeFile(String fileName)
{
  SPIFFS.remove("/"+fileName);
}

void saveFile(String fileName, String data)
{
    File file = SPIFFS.open("/" + fileName, FILE_WRITE);
    if(!file)
    {
      Serial.println("There was an error opening the file for writing");
      return;
    }
    if(file.print(data)) {
        Serial.println("File was written");
    }else {
        Serial.println("File write failed");
    }
    file.close();
}

String readFile(String fileName)
{
  String data = "";
  File file = SPIFFS.open("/" + fileName, "r");
    if(!file)
    {
      Serial.println("There was an error opening the file for reading");
      return "Error";
    }
    while (file.available())
    {
      data += (char)file.read();
    }
    
    file.close();
    return data;
}

void handleRoot() {
  // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN, HIGH);
  
    // wait for a second
    delay(3000);
  
    // turn the LED off by making the voltage LOW
    digitalWrite(LED_BUILTIN, LOW);
}

void handlePost() {

    if (server.hasArg("ssid") && server.hasArg("password"))
    {
      String ssid = server.arg("ssid");
      String password = server.arg("password"); 
      Serial.print(ssid + " " + password + "\n");
      String to_save = ssid + " " + password;
      saveFile("creditentials.txt", to_save);
    }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  if(!SPIFFS.begin(true))
  {
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
  }
  WiFi.mode(WIFI_AP);
  delay(250);
  WiFi.softAP("ESP32", "passworld");
  server.on("/", handleRoot);
  server.on("/post", HTTP_POST, handlePost);
  server.begin();
}

int counter = 0;

void loop() {
  server.handleClient();

  currentState = digitalRead(BUTTON_PIN);

  if (readFile("creditentials.txt").length() > 0)
  {
    Serial1.print(readFile("creditentials.txt"));
  }

  if (lastState == HIGH && currentState == LOW)       // button is pressed
    pressedTime = millis();
  else if (lastState == LOW && currentState == HIGH) { // button is released
    releasedTime = millis();

    long pressDuration = releasedTime - pressedTime;

    if ( pressDuration > SHORT_PRESS_TIME ) 
    {
      removeFile("creditentials.txt");
    }
    else
    {
      Serial.println("Long Press");
      Serial.println(readFile("creditentials.txt"));
    }
      
  }


  lastState = currentState;
}