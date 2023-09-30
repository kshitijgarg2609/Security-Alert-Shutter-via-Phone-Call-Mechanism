#include <SoftwareSerial.h>

void initVSerial(uint16_t,int);
boolean fetchVSerData();
void resetVSerPointer();
void enterVSerByte(uint8_t);
String decodeVSer();
void sendVSer(String);

uint8_t *vser_q;
uint16_t rsv_q_max;
uint16_t vser_pnt=0;

boolean car_flg=false;

SoftwareSerial vserial(D1,D2);

void initVSerial(uint16_t r,int baud)
{
  rsv_q_max=r;
  vser_q=(uint8_t*)calloc(rsv_q_max,sizeof(uint8_t));
  vserial.begin(baud);
}

boolean fetchVSerData()
{
  while(vserial.available())
  {
    uint8_t d=vserial.read();
    if(d=='\n')
    {
      if(car_flg)
      {
        car_flg=false;
        return true;
      }
      else
      {
        resetVSerPointer();
        break;
      }
    }
    else if(d=='\r')
    {
      if(car_flg)
      {
        resetVSerPointer();
      }
      else
      {
        car_flg=true;
      }
      break;
    }
    else
    {
      enterVSerByte(d);
    }
  }
  return false;
}

void resetVSerPointer()
{
  vser_pnt=0;
  car_flg=false;
}

void enterVSerByte(uint8_t d)
{
  if(vser_pnt<rsv_q_max)
  {
    vser_q[vser_pnt++]=d;
  }
  else
  {
    resetVSerPointer();
  }
}

String decodeVSer()
{
  if(vser_pnt==0)
  {
    return "";
  }
  String tmp="";
  tmp.reserve(vser_pnt);
  for(int i=0;i<vser_pnt;i++)
  {
    tmp+=(char)(vser_q[i]);
  }
  resetVSerPointer();
  return tmp;
}

void sendVSer(String a)
{
  vserial.println(a);
}
