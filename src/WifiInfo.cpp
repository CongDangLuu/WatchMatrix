#include "WifiInfo.h"
VOID printChar(UINT8 c)
{
    if (c > 19 && c < 128)
    {
        Serial.print((CHAR8)(c));
    }
    else
    {
        Serial.print(c);
    }
     Serial.print(" ");
}


UINT16 GetNunmCfg(){
    UINT16 NumCfg = (UINT16) ((EEPROM.read(0) << 8) | (EEPROM.read(1)));
    Serial.print("NumCfg : ");
    Serial.println(NumCfg);
    return NumCfg;
}


VOID EepromReadWifiInfo()
{
    Serial.println("\nReading EEPROM...");
    String ssid = "";
    String pass = "";
    if (EEPROM.read(0) != 0)
    { // neu duu lieu doc ra tu EEPROM khac 0 thi doc du lieu
        for (UINT8 i = 0; i < 200; ++i)
        { // 32 o nho dau tieu la chua ten mang wifi SSID
            UINT8 charr = EEPROM.read(i);
            ssid += char(charr);
            printChar(charr);
        }
    }
    else
    {
        Serial.println("Config not found.");
    }
    Serial.println("------------------------------------------------------------------------------");
}



DATA_HEADER ReadHeader()
{
    DATA_HEADER header;
    ZeroMem(&header, sizeof(DATA_HEADER));
    if (EEPROM.read(0) > 0)
    {
        EEPROM.get(0, header);
    }
    return header;
}

WIFI_CFG ReadCfg(
    UINT8 index
)
{    
    DATA_HEADER     Header;
    WIFI_CFG        WifiCfg;
    UINT8           *buffer;
    UINT16          len;
    UINT16          Add;
    WIFI_CFG_INFO   WifiDAta;

    Header = ReadHeader();

    WifiDAta = Header.WifiDAta[index];
    Add = WifiDAta.Address;
    len = WifiDAta.passlenght + WifiDAta.ssidlenght;

    WifiCfg.ssid = (CHAR8 *)AllocatePool(WifiDAta.ssidlenght);
    WifiCfg.pass = (CHAR8 *)AllocatePool(WifiDAta.passlenght);

    buffer = (UINT8 *)AllocatePool(len);
    EEPROM.readPtr(Add, buffer, len);

    CopyMem(WifiCfg.ssid, buffer, WifiDAta.ssidlenght);
    CopyMem(WifiCfg.pass, buffer + WifiDAta.ssidlenght, WifiDAta.passlenght);

    free(buffer);
    return WifiCfg;
}


VOID WriteCfg(
    String ssid,
    String pass
)
{
    WIFI_CFG        WifiCfg;
    WIFI_CFG_INFO   WifiInfo;
    DATA_HEADER     Header;
    UINT8           NumWifiCfg;

    WifiInfo.passlenght = pass.length() + 1;
    WifiInfo.ssidlenght = ssid.length() + 1;

    WifiCfg.pass = (CHAR8 *)AllocatePool(WifiInfo.passlenght);
    WifiCfg.ssid = (CHAR8 *)AllocatePool(WifiInfo.ssidlenght);

    ssid.toCharArray(WifiCfg.ssid, WifiInfo.ssidlenght);
    pass.toCharArray(WifiCfg.pass, WifiInfo.passlenght);

    // Update Header data
    Header = ReadHeader();

    NumWifiCfg = Header.NumWifiCfg;
    Header.NumWifiCfg = NumWifiCfg + 1;
    WifiInfo.Address = ADD_DATA_START;

    if (NumWifiCfg > 0)
    {
        WifiInfo.Address = Header.WifiDAta[NumWifiCfg - 1].Address + Header.WifiDAta[NumWifiCfg - 1].passlenght + Header.WifiDAta[NumWifiCfg - 1].ssidlenght + 1;
    }
    Header.WifiDAta[NumWifiCfg] = WifiInfo;
    EEPROM.put(0, Header);

    // Write ssid and pass into EEPROM  in address WifiInfo.Address
    EEPROM.writePtr(WifiInfo.Address, WifiCfg.ssid, WifiInfo.ssidlenght);
    EEPROM.writePtr(WifiInfo.Address + WifiInfo.ssidlenght, WifiCfg.pass, WifiInfo.passlenght);
    free(WifiCfg.pass);
    free(WifiCfg.ssid);
    EEPROM.commit();
}

VOID CmdResetEEPROM(){
    UINT8 *buffer = (UINT8 *)AllocatePool(EEPROM_SIZE);
    ZeroMem(buffer, EEPROM_SIZE);
    EEPROM.writePtr(0, buffer, EEPROM_SIZE);
    EEPROM.commit();
    free(buffer);
}

BOOLEAN IsCfgAvailable(WIFI_CFG WifiCfgDes){
    DATA_HEADER Header;
    WIFI_CFG WifiCfg;

    Header = ReadHeader();
    if (Header.NumWifiCfg == 0)
    {
        return false;
    }
 
    for (UINT8 i = 0; i < Header.NumWifiCfg; i++)
    {
        WifiCfg = ReadCfg(i);
        if ((memcmp(WifiCfgDes.ssid, WifiCfg.ssid, strlen(WifiCfgDes.ssid)) == 0) &&
            (memcmp(WifiCfgDes.pass, WifiCfg.pass, strlen(WifiCfgDes.pass)) == 0)){
            return true;
        }
    }
    return false;
}

BOOLEAN ConnectWifi(
    WIFI_CFG WifiCfg
){
    UINT8 cnt;

    WiFi.disconnect();
    WiFi.begin(WifiCfg.ssid, WifiCfg.pass);
    cnt = 0;
    Serial.print("Connect to Wifi: ");
    Serial.println(WifiCfg.ssid);

    Serial.print("           pass: ");
    Serial.println(WifiCfg.pass);
    while ((WiFi.status() != WL_CONNECTED) && (cnt++ < 30))
    {   
        digitalWrite(LED, !digitalRead(LED));
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    if (cnt < 30){ //20s
        digitalWrite(LED, LOW);
        return true;
    } else {
        digitalWrite(LED, HIGH);
        WiFi.disconnect();
        return false;
    }
}
VOID CmdAutoConnectWifi(){
    DATA_HEADER Header;
    WIFI_CFG WifiCfg;
  
    Header = ReadHeader();
    if(Header.NumWifiCfg == 0){
        Serial.println("Error: Not found config Wifi in EEPROM!");
        return;
    }
    if(Header.NumWifiCfg > MAX_NUNBER_OF_CONFIG ){
        CmdResetEEPROM();
        Serial.println("Error: Not found config Wifi in EEPROM!");
        return;
    }
    for (UINT8 i = 0; i < Header.NumWifiCfg; i++)
    {
        Serial.println();
        Serial.print("Index: ");
        Serial.println(i);
        WifiCfg = ReadCfg(i);        
    
        if (ConnectWifi(WifiCfg))
        {
            Serial.print("Connect to Wifi: ");
            Serial.print(WifiCfg.ssid);
            Serial.println("- Sucessfull!");
            if (i > 0)
            {
                WIFI_CFG_INFO temp = Header.WifiDAta[0];
                Header.WifiDAta[0] = Header.WifiDAta[i];
                Header.WifiDAta[i] = temp;
                EEPROM.put(0, Header);
                EEPROM.commit();
            }
            break;
        }
    }   
}

/*
    Connect wifi entered from serial.
    if it's able to connect, write ssid and passwork to EEPROM
*/
VOID CmdSetCfg(){
    WIFI_CFG   WifiCfg;

    String ssid ="";
    String pass ="";
    Serial.readString();//remove serial buffer
    Serial.println("Enter your Wifi name: ");
    while (1)//wait to enter wifi name
    {
        if (Serial.available())
        {
            ssid = Serial.readStringUntil('\r');
            ssid.trim();
            Serial.readString();//remove serial buffer
            Serial.println("Enter your Wifi password: ");
            while (1)//wait to enter wifi passwork
            {
                if (Serial.available())
                {
                    pass = Serial.readStringUntil('\r');
                    pass.trim();
                    Serial.readString();//remove serial buffer
                    break;
                }
            }
            break;
        }
    }
    WifiCfg.pass = (CHAR8 *)AllocatePool(pass.length() +1);
    WifiCfg.ssid = (CHAR8 *)AllocatePool(ssid.length() +1);

    ssid.toCharArray(WifiCfg.ssid, ssid.length() +1);
    pass.toCharArray(WifiCfg.pass, pass.length() +1);
    if (ConnectWifi(WifiCfg))
    {
        Serial.print("Connect to Wifi: ");
        Serial.print(WifiCfg.ssid);
        Serial.println(" - Sucessfull!");
        if (IsCfgAvailable(WifiCfg))
        {
            Serial.print("Wifi ");
            Serial.print(WifiCfg.ssid);
            Serial.println(" is available in EEPROM");
        }
        else
        {
            Serial.print("Update Wifi ");
            Serial.print(WifiCfg.ssid);
            Serial.println(" to EEPROM");
            if (EEPROM.read(0) >= MAX_NUNBER_OF_CONFIG){
                CmdResetEEPROM();
            }
            WriteCfg(ssid, pass);
        }
    } else {
        Serial.print("Can connect to wifi ");
        Serial.println(WifiCfg.ssid);
        Serial.println("Please double-check your information");
        CmdAutoConnectWifi();
    }     
}

VOID CmdShowWifiCfg(){
    DATA_HEADER Header;
    WIFI_CFG WifiCfg;
  
    Header = ReadHeader();
    if(Header.NumWifiCfg == 0){
        Serial.println("Error: Not found config Wifi in EEPROM!");
        return;
    }
    Serial.println("Wifi configs are available: ");
    for (UINT8 i = 0; i < Header.NumWifiCfg; i++)
    {
        WifiCfg = ReadCfg(i);

        Serial.print(" Index: ");
        Serial.print(i);
        Serial.print("     Name: ");
        Serial.print(WifiCfg.ssid);
        Serial.print("     Pass: ");
        Serial.println(WifiCfg.pass);
    }   
}

VOID CmdConnectWifi(){

    DATA_HEADER     Header;
    WIFI_CFG        WifiCfg;
    UINT8           index;
    String          str;
    Header = ReadHeader();
    if(Header.NumWifiCfg == 0){
        Serial.println("Error: Not found config Wifi in EEPROM!");
        return;
    }
    CmdShowWifiCfg();
    Serial.readString(); // remove serial buffer
    Serial.println("Enter index of wifi: ");
    while (1)
    {
        if (Serial.available())
        {
            str = Serial.readStringUntil('\r');
            str.trim();
            Serial.readString(); // remove serial buffer
            index = str.toInt();
            WifiCfg = ReadCfg(index);

            if (ConnectWifi(WifiCfg))
            {
                Serial.print("Connect to Wifi: ");
                Serial.print(WifiCfg.ssid);
                Serial.println(" - Sucessfull!");
            }
            else
            {
                Serial.print("Connect to Wifi: ");
                Serial.print(WifiCfg.ssid);
                Serial.println(" - Failed!");
            }
            break;
        }
    }
}
VOID CmdHelpdisplay(){
    Serial.println("Command: ");
    Serial.println("   1. show:         Display all connected wifis .");
    Serial.println("   2. add:          Add and connect new wifi.");
    Serial.println("   3. connect:      Select stored wifi to connect.");
    Serial.println("   4. autoconnect:  Find and connect with stored wifi automatically.");
    Serial.println("   5. clear:        Remove all stored wifis.");
    Serial.println("   5. help:         Display help information.");
    Serial.println();

    Serial.println("Note: Command must be in lower case");
}

