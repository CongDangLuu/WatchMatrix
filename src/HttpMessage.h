#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#define COMMAND_MESSAGE         "mess"
#define MESSAGE_ON              "on"
#define MESSAGE_OFF             "off"
#define MESSAGE_SET             "set"

#define MAX_WORDS   10
typedef struct
{
    int State;
    String Mess;
    String words[MAX_WORDS];
    int wordCount;
    int index;
}Message;

boolean IsDisplayMess();
void InitMess();
void CmdGetMessage();
void GetMessage();
void callDisplay();

//
void LedInfoPost(const char *phplink, String poststring);
String LedInfoGet(const char *Jsonlink);
