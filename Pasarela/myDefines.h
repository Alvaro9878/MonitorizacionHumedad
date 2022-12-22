/**********************************************************************
   CONFIGS.- WiFi & MQTT PROTOCOL
***********************************************************************/
#define WIFI_SSID           "XXXXXXXX"
#define WIFI_PASSWORD       "XXXXXXXX"

#define WIFI_CHANNEL 6

// datos para actualización
#define HTTP_OTA_ADDRESS      F("192.168.1.132")         // Address of OTA update server
#define HTTP_OTA_PATH         F("/esp8266-ota/update") // Path to update firmware
#define HTTP_OTA_PORT         1880                     // Port of update server
                                                       // Name of firmware
#define HTTP_OTA_VERSION      String(__FILE__).substring(String(__FILE__).lastIndexOf('\\')+1) + ".nodemcu" 


//#define OTA_URL             "https://huertociencias.uma.es/esp8266-ota-update"
//#define OTA_FINGERPRINT     "fd416a4a7451f42849ace38b768a6a0cf8918b86" // sustituir valor
//#define HTTP_OTA_VERSION      String(__FILE__).substring(String(__FILE__).lastIndexOf('\\')+1) + ".nodemcu" 
#define VERSION_MAJOR 1
#define VERSION_MINOR 0
//#define __DEBUG__ true   //set to true for debug output, false for no debug ouput
//#define Serial if(__DEBUG__)Serial

#define TYPE_NODE String("espnow")

#define WATCHDOG_TIMEOUT_S 8  //time in ms to trigger the watchdog

#define SERIAL_BAUD_RATE    115200
#define LED_STATUS 16 

#define CHECK_UPDATE_TIMER 10L 
