
#include<Wire.h>
#include <SPI.h> // Not actually used but needed to compile
#include <SoftwareSerial.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
using namespace std;

SoftwareSerial mySerial(0, 1);  // RX, TX pins for UART communication
const int MPU_addr=0x68;        // SPI adress for Gyrosscope
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;  // Gyro data
int forward = 8;

double scandata[3];             // data recieved from lidar stored here

/*Define Vibromotor pins*/
// All must be PWM pins
#define THUMB   5 
#define INDEX   9
#define MIDDLE  10
#define RING    6
#define LITTLE  3




void setup() {
  Serial.begin(9600);   // start Serial port for sending control flags
  while (!Serial);      // wait untill done
  mySerial.begin(9600); // start Serial port for reciveing lidar data
  
  /*Setup communication between arduino board and gyroscope*/
  Wire.begin(); 
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); 
  Wire.write(0);     
  Wire.endTransmission(true);
  /*END Setup*/

  /*Setup pinmodes of vibromotors*/
  pinMode(RING, OUTPUT);
  pinMode(THUMB, OUTPUT);
  pinMode(INDEX, OUTPUT);
  pinMode(MIDDLE, OUTPUT);
  pinMode(LITTLE, OUTPUT);
}

void loop() {
  gyro(); 
  delay(100);     //Small delay
}
/**
 * Function used to split string to the array of strings 
 * which are formed by dividing original string by the given delimeter('|').
 */
void split(String data)
{
    int pos = 0;
    String token;

    for(int i=0; i<3; i++)
    {
        pos = data.indexOf('|');
        token = data.substring(0, pos);
        scandata[i] = token.toDouble();
        data.remove(0, pos + 1);
    }
}


  

void gyro()
{
  /*Start reading gyro value*/
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  
  AcX = Wire.read()<<8|Wire.read();     
  AcY = Wire.read()<<8|Wire.read();  
  AcZ = Wire.read()<<8|Wire.read();  
  Tmp = Wire.read()<<8|Wire.read();  
  GyX = Wire.read()<<8|Wire.read(); 
  GyY = Wire.read()<<8|Wire.read();  
  GyZ = Wire.read()<<8|Wire.read();  
  /*Gyro read done*/
  
  /*Proccess gyro data to control flags*/
  if(AcX<-6000 && -11000)     // if hand is tilt 'FORWARD'
    Serial.println("w");  
  else if(AcX>4000 && 6000)   // if hand is tilt 'BACKWARD'
    Serial.println("x");
  else
    Serial.println("s");      // if hand is in equilibrium by x-axis

  if(AcY<-7000 && -5000)      // if hand is tilt 'LEFT'
    Serial.println("a");
  else if(AcY>5000 && 12000)
    Serial.println("d");      // if hand is tilt 'RIGHT'
  else
    Serial.println("r");      // if hand is in equilibrium by y-axis
  /*Proccessing done and data sent*/

  if(mySerial.available()){
    split(mySerial.readStringUntil('#'));
    vibro();
  }else{
    scandata[0] = 100;
    scandata[1] = 100;
    scandata[2] = 100;
  }
 


}

void vibro()
{
  /* 
    ---LEFT---
    THUMB   5
    INDEX   4
    MIDDLE  3
    RING    2
    LITTLE  1
    ---RIGHT---
  */
  
  
  
  int finger1 = filter(scandata[0]); // soldaki
  int finger3 = filter(scandata[1]); // ortdaki
  int finger5 = filter(scandata[2]); // sagdaki
  int finger2 = (finger1 + finger3)/2; // aver
  int finger4 = (finger3 + finger5)/2; // aver

  analogWrite(THUMB, finger5);
  analogWrite(INDEX, finger4);
  analogWrite(MIDDLE, finger3);
  analogWrite(RING, finger2);
  analogWrite(LITTLE, finger1);
}

int filter(double value)
{
  double max_dist = 1.0; // start vibrating if closer than 1m
  if(value > max_dist)
    return 0;
    
  return 128 - map(value * 100, 0, max_dist * 100, 0, 128); // 255 - value = to reverse the polarity
}
