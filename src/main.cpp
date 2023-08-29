#include <Arduino.h>

// Library for getting date & time from internet
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// my wifi
const char* ssid     = "Hoang Yen";
const char* password = "VNPT123@@2021";

//define server for timeClient
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
int presecond = 0; // check changing of time


String timestring(int value);

void setup(){
  Serial.begin(115200);

  // connect Wifi
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  //Starting timeClient
  timeClient.begin();
  timeClient.setTimeOffset(+7*60*60);//Viet Nam's timezonr is +7
}

void loop() {
  timeClient.update();
  if (presecond != timeClient.getSeconds()){
    presecond = timeClient.getSeconds();
    String hour = timestring(timeClient.getHours());
    String minutes = timestring(timeClient.getMinutes());
    String second = timestring(presecond);
    String time = hour + ":" + minutes + ":" + second;
    Serial.println(time);
  }
}

String timestring(int value){
  String time = "";
  if(value < 10){
    time = "0" + String(value);
  }
  else{
    time = String(value);
  }
  return time;
}