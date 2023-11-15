#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>


#define     PIN     D6
#define     COLOR_INDEX     6
#define     FONT_WIDTH_4x6  4
#define     FONT_WIDTH_5x7  5
void MatrixSetup();

void MatrixDisplayTime(String str);
void MatrixDisplayMessage(String str);
