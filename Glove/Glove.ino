#include <RH_ASK.h>
#include <SPI.h> 
#include <math.h>

#define RING    2
#define THUMB   3
#define INDEX   4
#define MIDDLE  5
#define LITTLE  6

#define INT_MAX 2147483647

RH_ASK driver;
double average=55;
long sum = analogRead(A0);
int cases = 1;



void setup() {
  Serial.begin(9600);
  pinMode(RING, OUTPUT);
  pinMode(THUMB, OUTPUT);
  pinMode(INDEX, OUTPUT);
  pinMode(MIDDLE, OUTPUT);
  pinMode(LITTLE, OUTPUT);
  for(int i = 0; i < 100; i++)
  {
    processFlexData(55);
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

void loop() {
  int value = analogRead(A0);         //Read and save analog value from potentiometer
  Serial.print("average=");    
  Serial.print(average);
  Serial.print(" | value=");    
  Serial.print(value);
  Serial.print(" | diff=");    
  Serial.println(average - value);
  int diff = average - value;
  
  if(abs(diff) > 1.5 && abs(diff) < 6){
   processFlexData(value);
   if(average - value > 0)
    allON();
   else
    allOFF();
  }
  delay(50);     
 
                       //Small delay
 
}

void processFlexData(int newValue)
{
  if(cases == 1000){ // Random threshold to clear the variables, after 500 cases we reset sum and case amount value
    cases = 0;
    sum = 0;
  }
  
  sum += newValue; 
  cases++;
  average = sum * 1.0 / cases;
}

void transmit()
{
    const char *msg = "Hello World!";
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    delay(1000);
}
void recieve()
{
    uint8_t buf[12];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
      int i;
      // Message with a good checksum received, dump it.
      Serial.print("Message: ");
      Serial.println((char*)buf);         
    }
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
