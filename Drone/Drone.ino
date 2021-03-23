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
#define interruptPin    2  // 

RH_ASK driver;
int delayTime = 50; // ms

enum CMD{STOP=0, FORWARD=1, BACKWARD=2, LEFT=3, RIGHT=4};
enum CMD droneControl = STOP;


void blink() {
  next();
}

void next()
{
    if(droneControl == STOP)
    droneControl = FORWARD;
  else if(droneControl == FORWARD)
    droneControl = BACKWARD;
  else if(droneControl == BACKWARD)
    droneControl = LEFT;
  else if(droneControl == LEFT)
    droneControl = RIGHT;
 else if(droneControl == RIGHT)
    droneControl = STOP;
}


void setup()
{
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
  
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

  fwd();
  delay(3000);
  fwd_right();
  delay(1000);
  fwd();
  delay(3000);
  fwd_right();
  delay(1000);
  fwd();
  delay(3000);
  fwd_right();
  delay(1000);
  fwd();
  delay(3000);
  fwd_right();
  delay(1000);

}

  //L ->  LOW, HIGH = FORWARD
  //R ->  HIGH, LOW = FORWARD
void loop()
{
  delay(delayTime);
 
 
  while(true)
  {
    switch(droneControl)
    {
      case STOP:
      setRightMotor(LOW, LOW);
      setLeftMotor(LOW, LOW);
      break;
      case FORWARD:
      setRightMotor(LOW, HIGH);
      setLeftMotor(HIGH, LOW);
      break;
      case BACKWARD:
      setRightMotor(HIGH, LOW);
      setLeftMotor(LOW, HIGH);
      break;
      case LEFT:
      setRightMotor(LOW, LOW);
      setLeftMotor(HIGH, LOW);
      break;
      case RIGHT:
      setRightMotor(LOW, HIGH);
      setLeftMotor(LOW, LOW);
      break;
      
    }
  }
  
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
  //LOW, HIGH = FORWARD
  digitalWrite(wheelInL1, pinOut1);
  digitalWrite(wheelInL2, pinOut2);
}
  
/** Function used for setting the output pins from motor controller IC. Controls Left motor*/
void setRightMotor(int pinOut1, int pinOut2)
{
  //HIGH, LOW = FORWARD
  digitalWrite(wheelInR1, pinOut1);
  digitalWrite(wheelInR2, pinOut2);
}


/** Function used to stop the motor by setting the input voltages of motor to LOW*/
void stopMotor()
{
  setRightMotor(LOW, LOW);
  setLeftMotor(LOW, LOW);
}

/** Function used to make motor move forwards*/
void fwd()
{
  setRightMotor(HIGH, LOW);
  setLeftMotor(LOW, HIGH);
}

/** Function used to make motor move backwards*/
void bkw()
{
  setRightMotor(LOW, HIGH);
  setLeftMotor(HIGH, LOW);
}

void fwd_right()
{
  fwd();
  setLeftMotor(LOW, LOW);
}
void fwd_left()
{
  fwd();
  setRightMotor(LOW, LOW);
}

void bkw_right()
{
  bkw();
  setLeftMotor(LOW, LOW);
}

void bkw_left()
{
  bkw();
  setRightMotor(LOW, LOW);
}
