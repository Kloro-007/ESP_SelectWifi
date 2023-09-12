#include <iostream>
#include <string.h>

using namespace std;

int main()
{
    const char* str_WifiName = "MyWifiName";
    short n_SglStrgth = -10;

    char str_StrOut [41+1] = {0};
    strcpy(str_StrOut, "{\"List\":[");
    cout << str_StrOut << endl;

    char str_ActEntry [9 + strlen(str_WifiName) + 9 + 4 + 1 + 1 ] = {0};
    sprintf(str_ActEntry, "{\"ssid\":\"%s\",\"rssi\":%4i}", str_WifiName, n_SglStrgth);
    cout << str_ActEntry << endl;

    strcat(str_StrOut, str_ActEntry);
    cout << str_StrOut << endl;    
}