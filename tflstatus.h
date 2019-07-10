#ifndef tflstatus_h
#define tflstatus_h

class TFLStatus
{
  public:
    TFLStatus(String L, String K);
    String getStatus();
  private:
    String Key;
    String Line; 
};

#endif
