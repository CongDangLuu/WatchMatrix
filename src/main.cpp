#include <Arduino.h>

// Library for getting date & time from internet

#include <ESP8266WiFi.h>


#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "HttpMessage.h"
#include "WifiInfo.h"
#include "LedMatrix.h"
#include "TimeClient.h"

int selectmode = 1;
String Messagestr = STRING_NULL;
String DisplayStr = STRING_NULL;
int pretime = 0;

//void define
VOID SerialCommand();

void setup(){
  Serial.begin(115200);
  delay(10);

  // connect Wifi
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  EEPROM.begin(EEPROM_SIZE);
  CmdAutoConnectWifi();

  //Starting timeClient
  TimeClientSetup();
  MatrixSetup();
  InitMess();
}

void loop() {
  SerialCommand();
  if (!IsDisplayMess()){
    DisplayStr = TimeClientGet();
    if (DisplayStr != STRING_NULL)
    {
      MatrixDisplayTime(DisplayStr);
    }
  } else {
    GetMessage();
  }
}


VOID SerialCommand(){
    if (Serial.available())
    {
        String cmd = Serial.readStringUntil('\r');
        cmd.trim();
        Serial.readString(); // remove serial buffer
        
        if (cmd == COMMAND_CLEAR)
        {
            Serial.println("COMMAND_CLEAR");
            CmdResetEEPROM();
        }
        else if (cmd == COMMAND_ADD)
        {
            Serial.println("COMMAND_ADD");
            digitalWrite(LED, HIGH);
            CmdSetCfg();
        }
        else if (cmd == COMMAND_CONNECT)
        {
            Serial.println("COMMAND_CONNECT");
            CmdConnectWifi();
        }
        else if (cmd == COMMAND_AUTO_CONNECT)
        {
            Serial.println("COMMAND_AUTO_CONNECT");
            CmdAutoConnectWifi();
        }
        else if (cmd == COMMAND_SHOW)
        {
            Serial.println("COMMAND_SHOW");
            CmdShowWifiCfg();
        }
        else if (cmd == COMMAND_HELP)
        {
            Serial.println("COMMAND_HELP");
            CmdHelpdisplay();
        }
        else if (cmd == COMMAND_MESSAGE)
        {
            Serial.println("COMMAND_MESSAGE");
            CmdGetMessage();
        }
        else if (cmd == COMMAND_COLOR)
        {
            Serial.println("COMMAND_COLOR");
            CmdSetLedColor();
        }
        else if (cmd == COMMAND_BRIGHT)
        {
            Serial.println("COMMAND_BRIGHT");
            CmdSetLedBright();
        }
        else if (cmd == COMMAND_GETSERVER)
        {
            Serial.println("COMMAND_GETSERVER");
            CmdGetServer();
        }
        else
        {
            Serial.println("Command not found");
        }
    }
}