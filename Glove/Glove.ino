#include <TM1638.h>
#include <RH_ASK.h>
#include <SPI.h> 

RH_ASK driver;

TM1638 module(8, 9, 10); // TM1638 module pins
#define TX

void setup() {
  Serial.begin(9600);    // Debugging only
    if (!driver.init())
         Serial.println("init failed");
    else
         Serial.println("good to go");  
  module.clearDisplay();
  module.setupDisplay(true, 3); // where 7 is intensity (from 0~7)
  int a = 0;
  module.setDisplayToDecNumber(a,0,true);
}

void loop() {
    recieve();
   
   
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
void actionManager(){
  int input = module.getButtons();

  for(int i = 0; i < 8; i++)
  {
    int nbit = nthBit(input, i);
//    disp+= nbit * power(10, i);
    if(nbit == 1)
     switch(i)
      {
        case 0:
        buttonAction0();
        break;
        case 1:
        buttonAction1();
        break;
        case 2:
        buttonAction2();
        break;
        case 3:
        buttonAction3();
        break;
        case 4:
        buttonAction4();
        break;
        case 5:
        buttonAction5();
        break;
        case 6:
        buttonAction6();
        break;
        case 7:
        buttonAction7();
        break;
      }
    
  }
  return;
  
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
