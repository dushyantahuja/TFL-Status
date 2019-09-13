#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <TFLStatus.h>

// Replace with your wifi credentials
const char * ssid = "***********";
const char * password = "***********";

// Get and App ID and API Key by registering on
// https://api-portal.tfl.gov.uk/signup

/*
    Currently setup to work only for Tube status - I may develop this further to include other modes as well
    Severity Levels for Tube Status are:
      severityLevel":1,"description":"Closed"
      severityLevel":2,"description":"Suspended"
      severityLevel":3,"description":"Part Suspended"
      severityLevel":4,"description":"Planned Closure"
      severityLevel":5,"description":"Part Closure"
      severityLevel":6,"description":"Severe Delays"
      severityLevel":7,"description":"Reduced Service"
      severityLevel":8,"description":"Bus Service"
      severityLevel":9,"description":"Minor Delays"
      severityLevel":10,"description":"Good Service"

    valid Lines:
      "id":"bakerloo","name":"Bakerloo"
      "id":"central","name":"Central",
      "id":"circle","name":"Circle"
      "id":"district","name":"District"
      "id":"hammersmith-city","name":"Hammersmith & City"
      "id":"jubilee","name":"Jubilee"
      "id":"metropolitan","name":"Metropolitan"
      "id":"northern","name":"Northern"
      "id":"piccadilly","name":"Piccadilly"
      "id":"victoria","name":"Victoria"
      "id":"waterloo-city","name":"Waterloo & City"
*/

String Key = "**********";
String Line = "central";
String ID = "**********";

TFLStatus tflstatus(Line,ID,Key);


void setup() {
  // put your setup code here, to run once:
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("WiFi Failed");
      while(1) {
          delay(1000);
      }
  }
}

void loop() {
  tflstatus.updateStatus();
  Serial.println(tflstatus.getLineReason());
  Serial.println(tflstatus.getLineStatus());
  Serial.println(tflstatus.getLineSeverity());
  delay(60000);       // Wait for 60 seconds
  // put your main code here, to run repeatedly:
}
