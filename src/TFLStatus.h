/*
  TFLStatus.cpp - Library to get tube status from the TFL API
  Created by Dushyant Ahuja, July 2019.
  Released into the public domain.
*/

#ifndef TFLStatus_h
#define TFLStatus_h

#include "Arduino.h"
#include <ESP8266WiFi.h>

class TFLStatus
{
  public:
    TFLStatus(String Line, String ID, String Key); // For Tube Status
    void updateStatus();
    const char* getLineStatus();
    int getLineSeverity();
    const char* getLineReason();
    const char* getResponse();
  private:
    String _Key;
    String _Line;
    String _ID;
    const char* _LineStatus;
    int _LineSeverity;
    const char* _LineReason;
};

#endif

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
