#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>


#define     PIN     D6
#define     COLOR_INDEX     6
#define     FONT_WIDTH_4x6  4
#define     FONT_WIDTH_5x7  5

#define COMMAND_COLOR            "color"
#define COMMAND_BRIGHT           "bright"
#define COMMAND_GETSERVER        "server"

typedef struct 
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t Bright;
}LED_INFO;


void MatrixSetup();

void MatrixDisplayTime(String str);
void MatrixDisplayMessage(String str);

void CmdSetLedColor();
void CmdSetLedBright();

void CmdGetServer();

