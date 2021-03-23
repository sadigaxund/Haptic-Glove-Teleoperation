#include <SPI.h>
#include <RH_ASK.h>

/*Pins Declaration*/
#define echoPinBack     22  // data pin for ultrasonic sensor at the back
#define echoPinRight    24  // data pin for ultrasonic sensor on the right
#define echoPinLeft     26  // data pin for ultrasonic sensor on the left
#define echoPinFront    28  // data pin for ultrasonic sensor in front
#define dataPinTrnsm    30  // data pin for transmitter module, 433Mhz
#define dataPinRcvr     32  // data pin for reciever module, 433 Mhz
#define trigPin         34  // Universal trigger pin for all ultrasonic sensors
#define wheelInL1       36  // Control pin 1 for the left motor
#define wheelInL2       38  // Control pin 2 for the left motor
#define wheelInR1       40  // Control pin 1 for the right motor
#define wheelInR2       42  // Control pin 2 for the right motor

RH_ASK driver;

void setup()
{
  /*****Define mode for each pin*****/

  /*INPUTS*/
  pinMode(echoPinBack, INPUT);       
  pinMode(echoPinRight, INPUT);
  pinMode(echoPinLeft, INPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(dataPinRcvr, INPUT);
  
  /*OUTPUTS*/
  pinMode(dataPinTrnsm, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(wheelInL1, OUTPUT);
  pinMode(wheelInL2, OUTPUT);
  pinMode(wheelInR1, OUTPUT);
  pinMode(wheelInR2, OUTPUT);

  Serial.begin(9600);    // Debugging only
  if (!driver.init())
    Serial.println("init failed");
}


void loop()
{

  
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

/** Function for getting the reading of ultrasonic sensor */
float getEchoOutput(int echoPin)
{
  long duration;
 
  digitalWrite(trigPin, LOW);             // Clear the trigPin
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);              // Set the trigPin on HIGH state for 10 micro seconds
  delayMicroseconds(10);
  
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);          // Get the sound wave travel time in microseconds
  return duration * 0.034 / 2;              // Calculate the distance and return
}


void setSpeed(int speed)
{
  //speed = (speed > 255)? 255: speed;
  //analogWrite(ctrlSpeedPin, speed);
}

/** Function used for setting the output pins from motor controller IC. Controls Right motor*/
void setLeftMotor(int pinOut1, int pinOut2)
{
  digitalWrite(wheelInL1, pinOut1);
  digitalWrite(wheelInL2, pinOut2);
}
/** Function used for setting the output pins from motor controller IC. Controls Left motor*/
void setRightMotor(int pinOut1, int pinOut2)
{
  digitalWrite(wheelInL1, pinOut1);
  digitalWrite(wheelInL2, pinOut2);
}


/** Function used to stop the motor by setting the input voltages of motor to LOW*/
void stopMotor()
{
 
}

/** Function used to make motor move forwards*/
void forwards()
{

}

/** Function used to make motor move backwards*/
void backwards()
{

}
