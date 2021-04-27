// TRANSMITTER CODE

#include<Wire.h>
#include <VirtualWire.h>
const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int forward = 8;
void setup()
{
    Serial.begin(9600);    
    Serial.println("setup");
    vw_setup(2000); 
    vw_set_tx_pin(12); 
    Wire.begin();
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B); 
    Wire.write(0);     
    Wire.endTransmission(true);
}

void loop()
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  
  AcX=Wire.read()<<8|Wire.read();     
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  Tmp=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read(); 
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();  
//Serial.print("AcX = "); Serial.print(AcX);
   if(AcX<-6000&&-11000)
    {
    char *msg2 = "a";
    
    Serial.println("a");
    }
  
   else if(AcX>4000&&6000)
    {
    char *msg2 = "b";
    
    Serial.println("b");
    }
   else if(AcY<-7000&&-5000)
  {
    char *msg2 = "c";
  
    Serial.println("c");
  }
   else if(AcY>5000&&12000)
{
    char *msg2 = "d";
    
    Serial.println("d"); 
}
else
  {
     char *msg2 = "e";
  
    Serial.println("e"); 
  }
  
} 
  
