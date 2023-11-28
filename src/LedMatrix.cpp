#include "LedMatrix.h"

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

uint8_t r, g, b;
int16_t cursol = matrix.width();


void MatrixSetup()
{
    r = 255;
    g = 0;
    b = 127;
    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(4);
    matrix.setTextColorRGB(r, g, b);
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