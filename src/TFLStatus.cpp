#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "ArduinoJson.h"
#include "TFLStatus.h"

const char fingerprint[] PROGMEM = "5B 2C 4C 7E AF 1A FE 11 8C E6 02 62 9B F0 EA A6 00 C1 A2 19"; //TFL fingerprint

const size_t capacity = 6*JSON_ARRAY_SIZE(0) + 4*JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + 2*JSON_OBJECT_SIZE(4) + 2*JSON_OBJECT_SIZE(7) + 2*JSON_OBJECT_SIZE(9) + JSON_OBJECT_SIZE(11) + 2450;
DynamicJsonDocument doc(capacity);
String line;
const char *host = "api.tfl.gov.uk";
const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80
WiFiClientSecure httpsClient;

TFLStatus::TFLStatus(String Line, String ID, String Key){
  _Key = Key;
  _Line = Line;
  _ID = ID;
}

void TFLStatus::updateStatus(){
  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(15000); // 15 Seconds
  delay(1000);

  //Serial.print("HTTPS Connecting");
  int r=0; //retry counter
  while((!httpsClient.connect(host, httpsPort)) && (r < 30)){
      delay(100);
      //Serial.print(".");
      r++;
  }

  String Link;

  //GET Data
  Link = "/line/"+_Line+"/status?app_id="+_ID+"&app_key="+_Key;

  //Serial.print("requesting URL: ");
  //Serial.println(host+Link);

  httpsClient.print(String("GET ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  //Serial.println("request sent");

  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      //Serial.println("headers received");
      //Serial.println(line);
      break;
    }
  }

  //Serial.println("reply was:");
  //Serial.println("==========");
  httpsClient.readStringUntil('\n'); // The API sends an extra line with just a number. This breaks the JSON parsing, hence an extra read
  while(httpsClient.connected()){
    line = httpsClient.readString();
    //Serial.println(line); //Print response
  }
  _Response = line;
  deserializeJson(doc, line);
  _LineStatus = doc[0]["lineStatuses"][0]["statusSeverityDescription"];
  _LineSeverity = doc[0]["lineStatuses"][0]["statusSeverity"];
  _LineReason = doc[0]["lineStatuses"][0]["reason"];
}

const char* TFLStatus::getLineStatus(){
  return _LineStatus;
}
int TFLStatus::getLineSeverity(){
  return _LineSeverity;
}
const char* TFLStatus::getLineReason(){
  return _LineReason;
}

String TFLStatus::getResponse(){
  return _Response;
}
