#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <SoftwareSerial.h>
#include <Servo.h> 
// Bluetooth Connection
SoftwareSerial BTserial(13, 11); // RX, TX
char phone;
// Motor Shield
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Servo myservo;
Adafruit_DCMotor *leftMotor = AFMS.getMotor(2);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);

// Pins for Ultrasonic Sensor
const int trigPin = 5;
const int echoPin = 6;
//--------------------------
const int minDistance = 40;
const int rightDistance = 40;
const int leftDistance = 40;
//-------------------------
bool off = false,estop=false;

//-------------------------
// defines variables
long duration;
int distance;

void setup(){
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  Serial.begin(9600); // Starts the serial communication
  BTserial.begin(9600);     
  AFMS.begin();
  myservo.attach(10);  // attaches the servo on pin 10 (SERVO_1 on the Motor Drive Shield to the servo object 
  myservo.write(95); // tells the servo to position at 90-degrees ie. facing forward.
  delay(1000); // delay for one seconds
}

void forward (){
  leftMotor->setSpeed(200);
  leftMotor->run(FORWARD);
  rightMotor->setSpeed(200);
  rightMotor->run(FORWARD);
}
void backward(){
  leftMotor->setSpeed(200);
  leftMotor->run(BACKWARD);
  rightMotor->setSpeed(200);
  rightMotor->run(BACKWARD);
}
void left (){
  leftMotor->setSpeed(200);
  leftMotor->run(BACKWARD);
  rightMotor->setSpeed(200);
  rightMotor->run(FORWARD);
}
void right (){
  leftMotor->setSpeed(200);
  leftMotor->run(FORWARD);
  rightMotor->setSpeed(200);
  rightMotor->run(BACKWARD);
}
void stopall (){
  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
}

void scanMid(){   // ULtrasonic sensor scans when servo is in it's initial position
   // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH, 50000);
  // Calculating the distance
  distance= duration / 29 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.println(duration );
}

void scanRight(){ // Sensor scans when servo is facing right
   // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH, 50000);
  // Calculating the distance
  distance= duration / 29 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.println(duration );
}

void scanLeft(){  // Sensor scans when servo is facing left
   // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH, 50000);
  // Calculating the distance
  distance= duration / 29 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.println(duration );
}
void phoneinput()               //retrieves an input from the phone using bluetooth
  {
  if (BTserial.available())     //BTserial checks if serial is available
    {   
    phone=' ';  
    phone = BTserial.read();    //takes the input from the bluetoth and puts it into [hone buffer
    
    //Serial.println(phone);
    Serial.write(phone);        //outputs the input from the phone into serial monitor
    }    
  }
  
void loop()
{ 
  //phoneinput();
  phone= ' ';
  phoneinput();
  if (phone== 'B')  //B turns on entire car functions
    estop = false; 

  while (estop== false){
     scanMid();
     myservo.write(95);
     delay(500);
     if (distance<minDistance){
      stopall();
      delay(2500);
      left();
      delay(750);
      stopall();
      delay(2500);
      forward();
     }
     scanRight();
     myservo.write(36);
     delay(500);
     if (distance < rightDistance){
      stopall();
      delay(2500);
      left();
      delay(750);
      stopall();
      delay(2500);
      forward();
     }
     scanLeft();
     myservo.write(144);
     delay(500);
     stopall();
     if (distance < leftDistance){
      stopall;
      delay(2500);
      right();
      delay(750);
      stopall();
      delay(2500);
      forward();
     }
     
    forward();
    estop =false;
    phoneinput();
    if (phone == 'A'){ //A turns off entire car's 
      estop = true;
      stopall();
    }

  }
  }
