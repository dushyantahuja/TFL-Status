#include "Arduino.h"
#include "TFLStatus.h"
#include <WiFiClientSecure.h>
#include "ArduinoJson.h"

const char fingerprint[] PROGMEM = "78 87 74 76 99 e2 f8 1b e0 9d c7 07 12 bd 46 1c aa 37 ad 69"; //TFL fingerprint
const size_t capacity = 6*JSON_ARRAY_SIZE(0) + 4*JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + 2*JSON_OBJECT_SIZE(4) + 2*JSON_OBJECT_SIZE(7) + 2*JSON_OBJECT_SIZE(9) + JSON_OBJECT_SIZE(11) + 2450;
DynamicJsonDocument doc(capacity);
const char *host = "api.tfl.gov.uk";
const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80

TFLStatus::TFLStatus(String Line, String Key, String ID){
  _Key = Key;
  _Line = Line;
  _Id = Id;
}

void TFLStatus::updateStatus(){
  WifiFiClientSecure httpsClient;
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
  /*if(r==30) {
    Serial.println("Connection failed");
  }
  else {
    Serial.println("Connected to web");
  }*/

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
  deserializeJson(doc, line);
  _LineStatus = doc[0]["lineStatuses"][0]["statusSeverityDescription"];
  _LineSeverity = doc[0]["lineStatuses"][0]["statusSeverity"];
  _LineReason = doc[0]["lineStatuses"][0]["reason"];
}

char* getLineStatus(){
  return _LineStatus
}
int getLineSeverity(){
  return _LineSeverity
}
char* getLineReason(){
  return _LineReason
}
