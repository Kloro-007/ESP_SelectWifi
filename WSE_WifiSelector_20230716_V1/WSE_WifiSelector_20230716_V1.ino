// Import required ESP libraries
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>  
#endif
#include <ESPAsyncWebServer.h>

const char* ssid = "ESPBOARD";
const char* password = "13081993";

long t_last;

// Server settings
AsyncWebServer server(80); // Create server on port 80

extern const char index_html[] PROGMEM;
extern const char redirect_html[] PROGMEM;

const uint8_t N_MAX_WIFI = 10;
char str_JsonList [9 + 56*N_MAX_WIFI - 1 + 2 + 1] = {0};

void setup() {
  // Start with a delay to be sure to get first statements in the serial
  delay(500);
  
  Serial.begin(115200);
  Serial.println(F("Program start."));

  // Start board in access point mode
  //ESP.eraseConfig();
  WiFi.mode(WIFI_AP_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.persistent(false);
  WiFi.setAutoConnect(false);

  // Create wifi
  WiFi.softAP(ssid, password);

  // Get IP
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");  Serial.println(IP);

  // Register what to do when request is received
  // Route for root / web page
  server.on("/", HTTP_GET, getHomePage);

  // Send a GET request to <ESP_IP>/getWifiList
  server.on("/getWifiList", HTTP_GET, getWifiList);
  
  // Send a GET request to <ESP_IP>/update?IN_Sel=ssid
  server.on("/update", HTTP_GET, getUpdate);

  // Start the server
  server.begin();

  // Print WiFi status
  WiFi.printDiag(Serial);

  // Disconnect from an AP if it was previously connected
  WiFi.disconnect();
  delay(500);

  t_last = -5000;
}

void loop() {
  // reguarly update list of available APs as JSON string
  long t_now = millis();
  if (t_now - t_last > 5000) {
    t_last = t_now;    

    // Get available wifi
    uint8_t numberOfNetworks = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);
    if (numberOfNetworks < 0){
      Serial.println("Error scanning networks.");
      return;
    } else if (numberOfNetworks == 0){
      Serial.print("TODO: Handle case where no wifi is found");
      return;
    }

    Serial.print("Found "); Serial.print(numberOfNetworks); Serial.println(" wifi network(s):");
    int16_t n_SglStrgth[numberOfNetworks] = {0};
    char str_WifiNames [numberOfNetworks][32+1] = {0};

      for (int i = 0; i < numberOfNetworks; i++){
        String str_ssid = WiFi.SSID(i);
        
        char  str_ssid_char [str_ssid.length()+1] = {0};        
        str_ssid.toCharArray(str_ssid_char, str_ssid.length()+1);
        
        strcpy(str_WifiNames[i], str_ssid_char );

        n_SglStrgth[i] = WiFi.RSSI(i);
        
        //Serial.print(str_WifiNames[i]); Serial.print(" - "); Serial.println(n_SglStrgth[i]);
      }

    // Create JSON string
    const uint8_t N_WIFI_LIM = min(N_MAX_WIFI, numberOfNetworks);
    createWifiJson(N_WIFI_LIM, str_WifiNames, n_SglStrgth, str_JsonList);
    //Serial.println(str_JsonList);   
  }
}

void delayFun(long dt){
  long t_start = millis();
  long t_now = millis();
  while(t_now - t_start < dt){
    t_now = millis();
    yield();
  }
}

void getHomePage(AsyncWebServerRequest *request){
  // Show home page
  request->send_P(200, "text/html", index_html);
}

void getWifiList(AsyncWebServerRequest *request){
  // Show home page
  request->send_P(200, "application/json", str_JsonList);
}


void getUpdate(AsyncWebServerRequest *request){     
  // Show redirect page
  request->send_P(200, "text/html", redirect_html);

  // Constant
  const char STR_GET_PARAM []= "IN_Sel";
  
  if (!request->hasParam(STR_GET_PARAM)) {
    return;
  }

  String str_TmpMsg = request->getParam(STR_GET_PARAM)->value();
  Serial.print("TODO: Handle connection to '"); Serial.print(str_TmpMsg); Serial.println("'");
  
}

void createWifiJson(const uint8_t N_NbSgl, char str_WifiNames [][33], const int16_t* n_SglStrength, char* str_JsonList){
  strcpy(str_JsonList, "{\"List\":[");
  uint8_t idx_LastChar = 8;

        for (int i = 0; i < N_NbSgl; i++){
            uint8_t n_NewEntryLgth = 9 + strlen(str_WifiNames[i]) + 9 + 4 + 2;
            
            char str_ActEntry [n_NewEntryLgth + 1 ] = {0};
            sprintf(str_ActEntry, "{\"ssid\":\"%s\",\"rssi\":%4i},", str_WifiNames[i], n_SglStrength[i]);

            strcat(str_JsonList, str_ActEntry );
            
            idx_LastChar += n_NewEntryLgth;
        }
        str_JsonList[idx_LastChar] = 0;

        strcat(str_JsonList, "]}");
}
