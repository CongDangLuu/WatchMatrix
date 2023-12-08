#include "LedMatrix.h"
#include "HttpMessage.h"

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
  matrix.Color(210, 162, 20),
  matrix.Color(250, 170, 20),
};

const char *SetBrightPhp = "http://watchmatrix.x10.mx/WatchMatrix/SetBright.php";
const char *BrightJson = "http://watchmatrix.x10.mx/WatchMatrix/Brightness.json";

const char *SetColorPhp = "http://watchmatrix.x10.mx/WatchMatrix/SetColor.php";
const char *ColorJson = "http://watchmatrix.x10.mx/WatchMatrix/Color.json";

LED_INFO LedInfor;
int16_t cursol = matrix.width();


uint8_t SerialValue(){
  String str = "";
  int value = 0;

  Serial.readString(); // remove serial buffer

  while (1) // wait to enter value
  {
    if (Serial.available())
    {
      str = Serial.readStringUntil('\r');
      str.trim();
      value = str.toInt();
      Serial.readString(); // remove serial buffer
      if (value < 0 || value > 255)
      {
        Serial.println("Error: value must be > 0 & < 256!!! ");
        Serial.println("Re-Enter this value: ");

        continue;
      }
      break;
    }
  }
  return uint8_t(value);
}

void CmdSetLedColor(){

  matrix.getTextColor(&LedInfor.r, &LedInfor.g, &LedInfor.b);
  Serial.println("Current color R - G - B : " + String(LedInfor.r) + " - " + String(LedInfor.g) + " - " + String(LedInfor.b));

  Serial.readString(); // remove serial buffer
 
  Serial.println("Enter new Red value: ");
  LedInfor.r = SerialValue();

  Serial.println("Enter new Green value: ");
  LedInfor.g = SerialValue();

  Serial.println("Enter new Blue value: ");
  LedInfor.b = SerialValue();
  Serial.println("New color R - G - B : " + String(LedInfor.r) + " - " + String(LedInfor.g) + " - " + String(LedInfor.b));
  matrix.setTextColorRGB(LedInfor.r, LedInfor.g, LedInfor.b);
  String ColortPost = "&R=" + String(LedInfor.r) + "&G=" + String(LedInfor.g) +"&B=" + String(LedInfor.b);
  LedInfoPost(SetColorPhp, ColortPost);
}

void CmdSetLedBright(){
  LedInfor.Bright = matrix.getBrightness();

  Serial.println("Current led brightness: " + String(LedInfor.Bright));
  Serial.println("Enter new brightness value: ");
  LedInfor.Bright = SerialValue();

  Serial.println("Set new brightness value: "+ String(LedInfor.Bright));
  matrix.setBrightness(LedInfor.Bright);

  String BrightPost = "&Bright=" + String(LedInfor.Bright);
  LedInfoPost(SetBrightPhp, BrightPost);
}

void CmdGetServer(){
  // LED_INFO serverInfor;
  String BrightStr = LedInfoGet(BrightJson);
  Serial.println("BrightStr: " + BrightStr);
  String ColorStr = LedInfoGet(ColorJson);
  Serial.println("ColorStr: " + ColorStr);
}

void ServerGetBright(){
  DynamicJsonDocument cmdJson(1024);
  DeserializationError error = deserializeJson(cmdJson, LedInfoGet(BrightJson));
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
  }
  else
  {
    // Protocol, Stt, MCU
    const char *Brightstr = cmdJson["Bright"];
    LedInfor.Bright = String(Brightstr).toInt();
  }
}

void ServerGetColor(){
  String ColortStr = LedInfoGet(ColorJson);
  DynamicJsonDocument cmdJson(1024);
  DeserializationError error = deserializeJson(cmdJson, ColortStr);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
  }
  else
  {
    // Serial.println(" ColortStr: " + ColortStr);
    const char *Red = cmdJson["R"];
    LedInfor.r = String(Red).toInt();
    const char *Green = cmdJson["G"];
    LedInfor.g = String(Green).toInt();
    const char *Blue = cmdJson["B"];
    LedInfor.b = String(Blue).toInt();
  }
}

void MatrixSetup()
{
    // LedInfor.r = 255;
    // LedInfor.g = 0;
    // LedInfor.b = 127;
    // LedInfor.Bright = 4;
    ServerGetColor();
    ServerGetBright();
    
    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(LedInfor.Bright);
    matrix.setTextColorRGB(LedInfor.r, LedInfor.g, LedInfor.b);
}

void MatrixDisplayTime(String str){
    matrix.setFontsize(FONT_SIZE_4x6);
    matrix.fillScreen(0);
    matrix.setCursor(1, 2);
    matrix.print(str);
    matrix.show();
}


void MatrixDisplayMessage(String str){
    uint8_t lenbitmap = str.length() *FONT_WIDTH_5x7;
    matrix.setFontsize(FONT_SIZE_5x7);
    matrix.fillScreen(0);
    if (lenbitmap < 32)
    {
      cursol = (int16_t)((32 - lenbitmap)/2) - 1;
    } else {
      if(--cursol < -2 * lenbitmap){
        cursol = matrix.width();
      }
    }    
    matrix.setCursor(cursol, 1);
    matrix.print(str);
    matrix.show();
}