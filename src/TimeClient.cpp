#include "TimeClient.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

//define server for timeClient
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

int presecond = 0; // check changing of time

void TimeClientSetup()
{
    timeClient.begin();
    timeClient.setTimeOffset(+7 * 60 * 60); // Viet Nam's timezonr is +7
}

String TimeClientGet()
{
    timeClient.update();
    if (presecond != timeClient.getSeconds())
    {
        presecond = timeClient.getSeconds();
        String hour = TimeToString(timeClient.getHours());
        String minutes = TimeToString(timeClient.getMinutes());
        String second = TimeToString(presecond);
        String time = hour + ":" + minutes + ":" + second;
        return time;
    }
    return STRING_NULL;
}

String TimeToString(uint8_t value)
{
    String time = "";
    if (value < 10)
    {
        time = "0" + String(value);
    }
    else
    {
        time = String(value);
    }
    return time;
}

void TimeClientSet(String time)
{
    return;
}