#include <ArduinoIoTCloud.h>
#include <WiFiConnectionManager.h>
#define SECRET_SSID "<SSID>"
#define SECRET_PASS "<PSK>"

const char THING_ID[] = "<thing id string here>";

const char SSID[]     = SECRET_SSID;    // Network SSID (name)
const char PASS[]     = SECRET_PASS;    // Network password (use for WPA, or use as key for WEP)

void onValveOpenChange();

float humidity;
float temperature;
float waterFlow;
int soilMoisture;
bool valveOpen;

void initProperties(){
  ArduinoCloud.setThingId(THING_ID);
  ArduinoCloud.addProperty(humidity, READ, 10 * SECONDS, NULL);
  ArduinoCloud.addProperty(temperature, READ, 10 * SECONDS, NULL);
  ArduinoCloud.addProperty(waterFlow, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(soilMoisture, READ, 10 * SECONDS, NULL);
  ArduinoCloud.addProperty(valveOpen, READWRITE, ON_CHANGE, onValveOpenChange);
}

ConnectionManager *ArduinoIoTPreferredConnection = new WiFiConnectionManager(SSID, PASS);
