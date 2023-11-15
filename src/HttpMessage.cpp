#include "HttpMessage.h"


#define STRING_NULL ""

const char *MessagePath = "http://watchmatrix.000webhostapp.com/Watchmatrix/Message.json";
HTTPClient http; // Declare object of class HTTPClient
WiFiClient client;
String payload, Pre_payload = "";
boolean skip = false;



String GetMessage()
{
  payload = STRING_NULL;
  int begincode = http.begin(client, MessagePath);     //Specify request destination
  if (begincode)
  {
    int httpCode = http.GET(); // Send the request
                               // Get the response payload from server
    if (httpCode == 200)
    {
      payload = http.getString();
      http.end(); // Close connection
      if (payload != Pre_payload)
      {
        Pre_payload = payload;
        if (skip)
        {
          Serial.println(payload); // Print request response payload-chuoi json
          return payload;
        }
        skip = true;
      }
    }
    else
    {
      Serial.print("httpCode != 200:  ");
      Serial.println(httpCode);
    }
  }
  return STRING_NULL;
}