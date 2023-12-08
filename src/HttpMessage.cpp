#include "HttpMessage.h"
#include "LedMatrix.h"


#define STRING_NULL ""
// http://watchmatrix.x10.mx/WatchMatrix/Message.json
const char *MessagePath = "http://watchmatrix.x10.mx/WatchMatrix/Message.json";
const char *SetMessagePhp = "http://watchmatrix.x10.mx/WatchMatrix/SetMessage.php";
HTTPClient http; // Declare object of class HTTPClient
WiFiClient client;
String payload, Pre_payload = "";
boolean skip = false;
Message mess;
int pre_time = 0;

void splitString(String inputString, String words[MAX_WORDS], int &wordCount) {
  for (int i = 0; i < MAX_WORDS; i++)
  {
    words[i] = 0;
  }
  
  int spaceIndex = inputString.indexOf(' ');

  while (spaceIndex != -1) {
    words[wordCount++] = inputString.substring(0, spaceIndex);
    inputString = inputString.substring(spaceIndex + 1);
    spaceIndex = inputString.indexOf(' ');

    if (wordCount >= MAX_WORDS) {
      break;
    }
  }

  words[wordCount++] = inputString;  // Add the last word
}

void ProcessMessage(String payload){

  DynamicJsonDocument cmdJson(1024);
  DeserializationError error = deserializeJson(cmdJson, payload);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
  }
  else
  {
    // Protocol, Stt, MCU
    const char *stt = cmdJson["State"];
    mess.State = String(stt).toInt();

    const char *str = cmdJson["Mess"];
    mess.Mess = String(str);
  }
 
  // Call the function to populate the array
  mess.wordCount = 0;
  mess.index = 0;
  splitString(mess.Mess, mess.words, mess.wordCount);

  // Print the result
  Serial.println("Print the result: ");
  for (int i = 0; i < mess.wordCount; i++) {
    Serial.println(mess.words[i]);
  }
}

void GetMessage()
{
  http.begin(client, MessagePath);     //Specify request destination
  int httpCode = http.GET(); // Send the request
  if (httpCode == 200)
  {
    payload = http.getString();
    if (payload != Pre_payload)
    {
      Pre_payload = payload;
      Serial.println(payload); // Print request response payload-chuoi json
      ProcessMessage(payload);
    }
  }
  http.end(); // close connection with MessagePath

  if (mess.State == 1)
  {
    int time = millis();
    if (time - pre_time > 650)
    {
      pre_time = time;
      MatrixDisplayMessage(mess.words[mess.index++]);
      delay(100);
      if (mess.index == mess.wordCount)
      {
        mess.index = 0;
      }
    }
  }
}
void callDisplay(){
  if (mess.State == 1)
  {
    MatrixDisplayMessage(mess.Mess);
  }
}

void PostMesage(Message mess)
{
  if (WiFi.status() == WL_CONNECTED)
  {

    String postData;
    postData = "&State=" + String(mess.State) + "&Mess=" + mess.Mess;

    http.begin(client, SetMessagePhp);                                   // Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // Specify content-type header

    int httpCode = http.POST(postData); // Send the request
    http.end();
    if (httpCode == 200)
    {
      Serial.println(postData);
      Serial.println("Status connect gate uploaded successfully.");
    }
    else
    {
      Serial.println(httpCode);
      Serial.println("Failed to upload values. \n");
    }    
  }
  else
  {
    Serial.println("Connect Wifi Error!!!");
  }
}

void CmdGetMessage()
{
  String stt = "";
  String str = "";
  Serial.readString(); // remove serial buffer
  Serial.println(" mess: ");
  Serial.print(" mess.State: ");
  Serial.println(mess.State);
  Serial.print(" mess.Mess: ");
  Serial.println(mess.Mess);
  Serial.println("Display message: ");
  Serial.println("   Enter  on/off   to change message dispaly state");
  Serial.println("   Enter  set      to change message string");

  while (1) // wait to enter cmd
    if (Serial.available())
    {
      stt = Serial.readStringUntil('\r');
      stt.trim();
      Serial.readString(); // remove serial buffer
      if (stt == MESSAGE_OFF)
      {
        mess.State = 0;
        PostMesage(mess);
        break;
      }
      else if (stt == MESSAGE_ON || stt == MESSAGE_SET)
      {
        mess.State = 1;
        if(stt == MESSAGE_SET){
          Serial.println("   Enter new message: ");
          while (1) // wait to enter new message
          {
            if (Serial.available())
            {
              mess.Mess = Serial.readStringUntil('\r');
              mess.Mess.trim();
              Serial.readString(); // remove serial buffer
              break;
            }
          }
        }
        else
        {          
          Serial.print("   Display messsage: ");
          Serial.println(mess.Mess);
        }
        PostMesage(mess);
        break;
      }
      else
      {
        Serial.println("ERROR: Command incorect");
        break;
      }
    }
}

boolean IsDisplayMess(){
  return mess.State;
}

void InitMess(){
  GetMessage();
  Serial.println("InitMess");
  Serial.println("mess.Mess" + mess.Mess);
  Serial.println("mess.State" + mess.State);
  mess.State = 0;
  PostMesage(mess);
}


void LedInfoPost(const char *phplink, String poststring){
  if (WiFi.status() == WL_CONNECTED)
  {
    http.begin(client, phplink);                                         // Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // Specify content-type header

    Serial.println(poststring);
    int httpCode = http.POST(poststring); // Send the request
    http.end();
    if (httpCode == 200)
    {
      Serial.println("Status connect gate uploaded successfully.");
    }
    else
    {
      Serial.println(httpCode);
      Serial.println("Failed to upload values. \n");
    }
  }
  else
  {
    Serial.println("Connect Wifi Error!!!");
  }
}


String LedInfoGet(const char *Jsonlink){
  String LedInfo = "";
  http.begin(client, Jsonlink);     //Specify request destination
  int httpCode = http.GET(); // Send the request
  if (httpCode == 200)
  {
    LedInfo = http.getString();
  }
  http.end(); // close connection with MessagePath
  return LedInfo;
}