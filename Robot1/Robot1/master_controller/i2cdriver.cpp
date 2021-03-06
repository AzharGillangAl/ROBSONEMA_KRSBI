#ifndef i2cdriver_cpp
#define i2cdriver_cpp

#include <Arduino.h>
#include "i2cdriver.h"
#include "interface.h"

//F = maju, B = mundur, R = kanan, L = kiri, A = putar kiri, D = putar kanan
//Q = serong maju kiri, E = serong maju kanan, Z = serong mundur kiri, C = serong mundur kanan

volatile byte slave_address[4];
volatile byte pinen=8;
volatile int enc[3]={0,0,0};
volatile bool enc_lock=true;

void initslave(char part, unsigned char address){
  switch(part){
    case 'L': slave_address[0]=address;break;
    case 'M': slave_address[1]=address;break;
    case 'R': slave_address[2]=address;break;
    case 'D': slave_address[3]=address;break;
  }
}

void transmitData(int lpwm, int bpwm, int rpwm){
  byte cl[3]={0,0,0};
  if(lpwm>0)cl[0]=1;//clockwise
  else if(lpwm<0)cl[0]=2;//anticlockwise
  else cl[0]=0;
  
  if(bpwm>0)cl[1]=1;//clockwise
  else if(bpwm<0)cl[1]=2;//anticlockwise
  else cl[1]=0;
  
  if(rpwm>0)cl[2]=1;//clockwise
  else if(rpwm<0)cl[2]=2;//anticlockwise
  else cl[2]=0;
  //===========kirim ke motor kiri===========
  Wire.beginTransmission(slave_address[0]);
  Wire.write((byte)(1));
  Wire.write((byte)(cl[0]));
  Wire.write((byte)(abs(lpwm)>>8));
  Wire.write((byte)(abs(lpwm)));
  Wire.endTransmission();
  //===========kirim ke motor belakang=======
  Wire.beginTransmission(slave_address[1]);
  Wire.write((byte)(1));
  Wire.write((byte)(cl[1]));
  Wire.write((byte)(abs(bpwm)>>8));
  Wire.write((byte)(abs(bpwm)));
  Wire.endTransmission();
  //===========kirim ke motor kanan==========
  Wire.beginTransmission(slave_address[2]);
  Wire.write((byte)(1));
  Wire.write((byte)(cl[2]));
  Wire.write((byte)(abs(rpwm)>>8));
  Wire.write((byte)(abs(rpwm)));
  Wire.endTransmission();
}

void transmitDataDribbler(char mov, byte drbVal, byte target){
  //===========kirim ke dribbler===========
  Wire.beginTransmission(slave_address[3]);
  Wire.write((char)(mov));
  Wire.write((byte)(drbVal)); 
  Wire.write((byte)(target));
  Wire.endTransmission();
}

void motor_enable(){
  digitalWrite(pinen,HIGH);
}

void motor_disable(){
  digitalWrite(pinen,LOW);  
}

void reset_encoder(unsigned char slave_addres){
  Wire.beginTransmission(slave_addres);
  Wire.write((byte)(0x02));
  Wire.endTransmission();
}

void update_encoder(){
  Wire.requestFrom(2,2);//kiri
  enc[0] =0;
  enc[0] |= Wire.read();
  enc[0] = enc[0] << 8;
  enc[0] |= Wire.read();
  Wire.requestFrom(3,2);//tengah
  enc[1] =0;
  enc[1] |= Wire.read();
  enc[1] = enc[1] << 8;
  enc[1] |= Wire.read();
  Wire.requestFrom(4,2);//kanan
  enc[2] =0;
  enc[2] |= Wire.read();
  enc[2] = enc[2] << 8;
  enc[2] |= Wire.read();
}

#endif
