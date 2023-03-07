# Sensor-de-contaminantes-

/////////////////////////////Librerias/////////////////////////////////////////////

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include “PMS.h”
#include <SoftwareSerial.h>

////////////////////////// Librería De DTH11//////////////////////

#include “DHT.h”
#define DHTTYPE DHT11 
const int DHTPin = D3; 
DHT dht (DHTPin, DHTTYPE); /

#define ON_Board_LED 2 
SoftwareSerial pmSerial1(D5, D6); 
PMS pms(pmSerial1);

PMS:DATA data;

const int MQ_PIN = A0;
const char* ssid =“”; 
const char* password = “”; 
const char* host = “script.google.com”; 
const int httpsPort = 443;

WiFiClientSecure client; 

String GAS_ID = “AKfycbxyyq-OWugLdDUjeRBbmAmoGp7wCMiKXpPfgX2iBCCC7CgjEyM”; void setup()

Serial.begin(115200); delay(500);

Serial.begin(9600); pmSerial1.begin(9600); dht.begin(); 
delay (30000);

WiFi.begin(ssid, password); 
Serial.println(“”);

pinMode(ON_Board_LED,OUTPUT); 
digitalWrite(ON_Board_LED, HIGH); 

/////////////////////////////////////////////////////////////////
////////////// Esperar conexión ////////////////////
///////////////////////////////////////////

Serial.print(“Connecting”);
while (WiFi.status() != WL_CONNECTED) { 
Serial.print(“.”);
digitalWrite (ON_Board_LED, LOW); 
delay(250);
digitalWrite (ON_Board_LED, HIGH); 
delay(250);}
digitalWrite (ON_Board_LED, HIGH);
Serial.println(“”);

Serial.print(“Successfully connected to : “);
Serial.println(ssid);
Serial.print( “IP address: “ );
Serial.println(WiFi.localIP());
Serial.println(); 
client.setInsecure();} void loop () {

if (pms.read(data)) {
float h = dht.readHumidity();
float t = dht.readTemperature();
int raw_adc = analogRead(MQ_PIN);
float value_adc = raw_adc * (3.3 / 10);
float dds = value_adc - 30;

if (isnan(h) || isnan(t) || pms.read(data) || isnan(dds)) { Serial.println( “la coneccion fallo !” );
delay(500); return; }

String Temp = “Temperature : “ + String(t) + “ °C”;
String Humi = “Humidity : “ + String(h) + “ %”;
String Pm2 = “Nitrogen :” + String(data.PM_AE_UG_2_5) + “ug/m3”; String Pm10 = “Monoxide:” + String(data.PM_AE_UG_10_0) + “ug/m3”; String nix = “Dioxide : “ + String(dds) + “ ppm”;

Serial.println(Temp);
Serial.println(Humi);
Serial.println(Pm2);
Serial.println(Pm10);
Serial.println(nix);

SendData (t, h, data.PM_AE_UG_2_5, data.PM_AE_UG_10_0, dds);
void sendData (float tem, int hum, int pm2, int pm10, int moxs) {
Serial.println(“==========”);
Serial.print( “connecting to “ );
Serial.println(host);

if (! client. connect (host, httpsPort)) { Serial.println( “connection failed” ); return; }
String string_temperature = String(tem);
string_temperature = String(tem, DEC);
String string_humidity = String(hum, DEC);
String string_nitrogen = String(pm2);
String string_monoxide = String(pm10);
String string_dioxide = String(moxs);

String url = “/macros/s/” + GAS_ID + “/exec?temperature=” + string_temperature + “&humidity=” + string_humidity + “&nitrogen=” + string_nitrogen + “&monoxide=” + string_monoxide + “&dioxide=” + string_dioxide;

Serial.print(“requesting URL: “);
Serial.println(url);
client.print(String( “GET “ ) + url + “ HTTP/1.1\r\n” + “Host: “ + host + “\r\n” + “User-Agent: BuildFailureDetectorESP8266\r\n” + “Connection: close\r\n\r\n”);
Serial.println( “request sent” );
Serial.println( “headers received”);
while (client.connected());
String line = client.readStringUntil( ‘\n ‘); if (line == “\r” ) {
break; }}
String line = client.readStringUntil( ‘\n’ ); 

if (line.startsWith( “{\”state\”:\”success\”” )) {
Serial.println( “esp8266/Arduino CI successfull!” );
} else {
Serial.println( “esp8266/Arduino CI has failed” ); }
Serial.print( “reply was : “
Serial.println(line);
Serial.println( “closing connection” );
Serial.println( “==========” );
Serial.println();
