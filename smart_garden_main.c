#include "arduino_secrets.h"
// SimpleDHT - Version: Latest 
#include <SimpleDHT.h>

#define DHTPIN 2
#define SOILPIN A0
#define FLOWMETERPIN A1
#define VALVEPIN 3

#define UPDATE_RATE 1500 //update sensor values every 1500ms

/* 
  Sketch generated by the Arduino IoT Cloud Thing "SmartGarden"
  https://create.arduino.cc/cloud/things/50892e59-50bc-4a46-afe3-78dc3e0d34ee 

  Arduino IoT Cloud Properties description

  The following variables are automatically generated and updated when changes are made to the Thing properties

  float humidity;
  float temperature;
  float waterFlow;
  int soilMoisture;
  bool valveOpen;

  Properties which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"

#define SOIL_THRESH 300 //given as number between 0-1023

SimpleDHT22 dht22(DHTPIN);

unsigned long latest_millis = 0;
unsigned long flow_millis = 0;

volatile float flow;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 
  pinMode(SOILPIN, INPUT);
  pinMode(FLOWMETERPIN, INPUT);
  pinMode(VALVEPIN, OUTPUT);
  set_valve(0);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  attachInterrupt(FLOWMETERPIN, triggerPulse, RISING);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information youll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  flow = 0.0;
}

void loop() {
  ArduinoCloud.update();
  // Your code here 
  
  if(millis() - latest_millis >= UPDATE_RATE){
    update_vals();
    latest_millis = millis();
  }
  if(millis() - flow_millis >= 1000){
    reset_flow();
    flow_millis = millis();
  }
}

void update_vals(){
  float temperature_raw = 0;
  float humidity_raw = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht22.read2(&temperature_raw, &humidity_raw, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT22 failed, err="); Serial.println(err);delay(1000);
    return;
  }
  
  temperature = read_temp(temperature_raw);
  humidity = read_hum(humidity_raw);
  waterFlow = get_water_flow();
  soilMoisture = read_soil_moisture();

  if(soilMoisture < SOIL_THRESH){
    valveOpen = 1;
    set_valve(valveOpen);
  }
  else{
    valveOpen = 0;
    set_valve(valveOpen);
  }
}

float read_temp(float temp_raw){
  float temp = (float)temp_raw;
  temp = (1.8 * temp) + 32.0;
  return temp;
}

float read_hum(float hum_raw){
  float hum = (float)hum_raw;
  return hum;
}

float get_water_flow(){ //given in L/s
  float flow_l_s = flow / 450.0;
  Serial.print("Flow: ");
  Serial.println(flow_l_s);
  return flow_l_s;
}

void reset_flow(){
  flow = 0.0;
}

void triggerPulse(){
  //Serial.println("triggered");
  flow += 1.0;
}

int read_soil_moisture(){
  int moisture = analogRead(SOILPIN);
  return moisture;
}

void set_valve(bool open_or_closed){
  digitalWrite(VALVEPIN, !open_or_closed);
}


void onTemperatureChange() {
  // Do something
}


void onHumidityChange() {
  // Do something
}


void onSoilMoistureChange() {
  // Do something
}


void onWaterFlowChange() {
  // Do something
}







void onValveOpenChange() {
  Serial.println(valveOpen);
  set_valve(valveOpen);
}