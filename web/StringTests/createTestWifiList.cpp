#include <iostream>
#include <string.h>

using namespace std;

void createWifiJson(const short N_NbSgl, const char* str_WifiNames[], const short* n_SglStrength, char* str_JsonList);

int main(){
    const short N_WIFI = 2;

    char str_JsonList [9 + 56*N_WIFI - 1 + 2 + 1] = {0};

    const char* str_Wifi1 = "AVERYLOOOOOOOOOOOOOOOONGWIFINAME";
    const char* str_Wifi2 = "AVERYLOOOOOOOOOOOOOOOONGWIFINAME";
    const short n_SglStrgth [] = {-100, -100};
    const char* str_WifiNames [] = {str_Wifi1, str_Wifi2};

    createWifiJson(N_WIFI, str_WifiNames, n_SglStrgth, str_JsonList);

    cout << str_JsonList << endl;  
}

void createWifiJson(const short N_NbSgl, const char* str_WifiNames [], const short* n_SglStrength, char* str_JsonList){
        strcpy(str_JsonList, "{\"List\":[");
        short idx_LastChar = 8;

        for (int i = 0; i < N_NbSgl; i++){
            short n_NewEntryLgth = 9 + strlen(str_WifiNames[i]) + 9 + 4 + 2;
            
            char str_ActEntry [n_NewEntryLgth + 1 ] = {0};
            sprintf(str_ActEntry, "{\"ssid\":\"%s\",\"rssi\":%4i},", str_WifiNames[i], n_SglStrength[i]);

            strcat(str_JsonList, str_ActEntry );
            
            idx_LastChar += n_NewEntryLgth;
        }
        str_JsonList[idx_LastChar] = 0;

        strcat(str_JsonList, "]}");
}