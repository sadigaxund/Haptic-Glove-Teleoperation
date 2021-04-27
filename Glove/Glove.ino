#include <math.h>
#include<Wire.h>
#include <VirtualWire.h>
const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int forward = 8;

#define RING    2
#define THUMB   3
#define INDEX   4
#define MIDDLE  5
#define LITTLE  6
#define TX_PIN 12
#define RX_PIN 11


#define INT_MAX 2147483647

double average=55;
long sum = analogRead(A0);
int cases = 1;



void setup() {
  Serial.begin(9600);    
  Serial.println("setup");
  // Initialise the IO and ISR
  vw_set_tx_pin(TX_PIN);
  vw_set_rx_pin(RX_PIN);
  vw_setup(2000);  
  vw_rx_start();
  
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); 
  Wire.write(0);     
  Wire.endTransmission(true);
  pinMode(RING, OUTPUT);
  pinMode(THUMB, OUTPUT);
  pinMode(INDEX, OUTPUT);
  pinMode(MIDDLE, OUTPUT);
  pinMode(LITTLE, OUTPUT);
  for(int i = 0; i < 100; i++)
  {
    processFlexData(analogRead(A0));
  }
     
  
}

void allON()
{
    //digitalWrite(THUMB, HIGH);
    digitalWrite(RING, HIGH);
    digitalWrite(INDEX, HIGH);
    digitalWrite(MIDDLE, HIGH);
    digitalWrite(LITTLE, HIGH);
}
void allOFF()
{
    digitalWrite(THUMB, LOW);
    digitalWrite(RING, LOW);
    digitalWrite(INDEX, LOW);
    digitalWrite(MIDDLE, LOW);
    digitalWrite(LITTLE, LOW);
}

int prevVal = analogRead(A0);
  
void loop() {
  //flex();
  gyro(); 
 
  delay(50);     //Small delay
 
}
void gyro()
{
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
  char *msg2;
  if(AcX<-6000&&-11000)
    msg2 = "a";
  else if(AcX>4000&&6000)
    msg2 = "b";
  else if(AcY<-7000&&-5000)
    msg2 = "c";
  else if(AcY>5000&&12000)
    msg2 = "d";
  else
    msg2 = "e";
  Serial.println(msg2);
  vw_send((uint8_t *)msg2, 7);
  vw_wait_tx(); // Wait until the whole message is gone
}
void flex()
{
  int value = analogRead(A0);         //Read and save analog value from potentiometer
  Serial.print("average=");    
  Serial.print(average);
  Serial.print(" | value=");    
  Serial.print(value);
  Serial.print(" | diff=");    
  Serial.println(average - value);
  int diff = average - value;
  diff = prevVal - value;
  prevVal = value;
  if(abs(diff) > 5){
   processFlexData(value);
   if(diff > 6)
    resetAverage();
   if(average - value > 0)
    allON();
   else
    allOFF();
  }
}

void resetAverage()
{
  cases = 0;
    sum = 0;
}

void processFlexData(int newValue)
{
  
  if(cases == 1000){ // Random threshold to clear the variables, after 500 cases we reset sum and case amount value
    resetAverage();
  }
  
  sum += newValue; 
  cases++;
  average = sum * 1.0 / cases;
}



void buttonAction0()
{
  
}
void buttonAction1()
{

}
void buttonAction2()
{

}
void buttonAction3()
{
 
}
void buttonAction4()
{

}
void buttonAction5()
{

}
void buttonAction6()
{

}
void buttonAction7()
{

}

int nthBit(int num, int n)
{
    /* Right shift num, n times and perform bitwise AND with 1 */
    return (num >> n) & 1;
}

/* Function to calculate x raised to the power y in O(logn)
    Time Complexity of optimized solution: O(logn)
*/
 long long power (long long x, unsigned  long long y)
{
    long long temp;
    if (y == 0)
        return 1;

    temp = power (x, y / 2);
    if ((y % 2) == 0)
        return temp * temp;
    else
        return x * temp * temp;
}
