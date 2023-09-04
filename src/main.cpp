#include <Arduino.h>

// Library for getting date & time from internet
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define PIN D6
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN,
                            NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
                            NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                            NEO_GRB            + NEO_KHZ800);
const uint16_t colors[] = {
  matrix.Color(255, 0, 0),
  matrix.Color(0, 255, 0),
  matrix.Color(0, 255, 255),
  matrix.Color(254, 1, 121),
  matrix.Color(244, 72, 167),
  matrix.Color(220, 163, 0),
};

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
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(4);
  matrix.setTextColor(colors[2]);
  delay(10);

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
    matrix.fillScreen(0);
    matrix.setCursor(1, 2);
    matrix.setTextColor(colors[5]);
    matrix.print(time);    
    matrix.show();
  }  
  // delay(5000);            
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