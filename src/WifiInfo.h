#include <Arduino.h>
#include <EEPROM_private.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <string.h>

extern "C" {
    #include <BaseLib.h>
    #include <Memory.h>
}
#define EEPROM_SIZE             0x1000
#define MAX_NUNBER_OF_CONFIG    5
#define NUNBER_CFG_LEN          2
#define ADD_DATA_START          0x20
#define COMMAND_ADD             "add"
#define COMMAND_CLEAR           "clear"
#define COMMAND_CONNECT         "connect"
#define COMMAND_AUTO_CONNECT    "autoconnect"
#define COMMAND_SHOW            "show"
#define COMMAND_HELP            "help"

#define LED 2  

enum{
    CMD_READ_ALL = 0,
    CMD_READ_CFG_1,
    CMD_READ_CFG_2,
    CMD_READ_CFG_3,
    CMD_READ_CFG_4,
    CMD_READ_CFG_5,
};

typedef struct
{    
    CHAR8      *ssid;
    CHAR8      *pass;
} WIFI_CFG;

typedef struct
{
    UINT16      Address;
    UINT8       ssidlenght;
    UINT8       passlenght;
} WIFI_CFG_INFO;


typedef struct
{
    UINT8           NumWifiCfg;
    WIFI_CFG_INFO   WifiDAta[MAX_NUNBER_OF_CONFIG];
}DATA_HEADER;

VOID EepromReadWifiInfo();

VOID WriteCfg(
    String ssid,
    String pass
);

WIFI_CFG ReadCfg(
    UINT8 index
);

DATA_HEADER ReadHeader();
VOID CmdResetEEPROM();
VOID CmdAutoConnectWifi();
BOOLEAN ConnectWifi(
    WIFI_CFG WifiCfg
);

VOID CmdSetCfg();
VOID CmdShowWifiCfg();
VOID CmdConnectWifi();
VOID CmdHelpdisplay();
VOID LedStt();

