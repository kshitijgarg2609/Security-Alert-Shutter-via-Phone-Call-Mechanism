#include "dial_conf.h"
#include "vserial_io.h"

int trig_pin=D5;

boolean trig_flg=false;

void setup()
{
  pinMode(trig_pin,INPUT_PULLUP);
  initConfProcess(true);
  initVSerial(128,115200);
  Serial.begin(115200);
  Serial.println("START");
}

void loop()
{
  confProcess();
  processVSerial();
  if(!trig_flg && (digitalRead(trig_pin)==LOW)&&(digitalRead(trig_pin)==LOW))
  {
    dial();
    Serial.println("DIALING ...");
    trig_flg=true;
  }
  if(trig_flg && (digitalRead(trig_pin)==HIGH)&&(digitalRead(trig_pin)==HIGH))
  {
    trig_flg=false;
  }
}

void checkFunction()
{
  sendVSer("AT");
}

void processVSerial()
{
  if(fetchVSerData())
  {
    String dec=decodeVSer();
    Serial.println(dec);
    cmdVSer(dec);
  }
}

void cmdVSer(String a)
{
  if(a.equals("RING"))
  {
    hangPhone();
  }
}

void dial()
{
  if(stat_actv)
  {
    dn_str=loadDNumber();
    if(!dn_str.equals(""))
    {
      hangPhone();
      sendVSer("ATD"+dn_str+";");
    }
  }
}

void hangPhone()
{
  sendVSer("ATH");
}
