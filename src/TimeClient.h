#include <Arduino.h>
#define STRING_NULL ""

void    TimeClientSetup();
String  TimeClientGet();
void    TimeClientSet(String time);
String  TimeToString(uint8_t value);