#include "SerialTransfer.h"

SerialTransfer myTransfer;


// Motor A
int motorPin1 = 2; // Connect to the input 1 of the L298N
int motorPin2 = 4; // Connect to the input 2 of the L298N
int enablePinA = 5; // Connect to the enable A of the L298N

// Motor B
int motorPin3 = 7; // Connect to the input 3 of the L298N
int motorPin4 = 8; // Connect to the input 4 of the L298N
int enablePinB = 6; // Connect to the enable B of the L298N

int joystickThreshold = 50; // Adjust this threshold based on your joystick's sensitivity

unsigned long lastCommunicationTime = 0;
unsigned long communicationTimeout = 1000; // Set the timeout value in milliseconds

struct __attribute__((packed)) STRUCT {
  int8_t thumb;
  int8_t pointer;
  int8_t middle;
  int8_t ring;
  int8_t pinky;
} testStruct;

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePinA, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(enablePinB, OUTPUT);

  // Set the motor speed to 0 initially
  analogWrite(enablePinA, 0);
  analogWrite(enablePinB, 0);

  Serial.begin(9600);
  myTransfer.begin(Serial);
}

void loop() {
  if (myTransfer.available()) {
    // Reset the timeout timer
    lastCommunicationTime = millis();
    //Serial.print(lastCommunicationTime);
    //Serial.print(" | ");
    // use this variable to keep track of how many
    // bytes we've processed from the receive buffer
    uint8_t recSize = 0;
    recSize = myTransfer.rxObj(testStruct, 5);
    Serial.print(testStruct.thumb);
    Serial.print(" | ");
    Serial.print(testStruct.pointer);
    Serial.print(" | ");
    Serial.print(testStruct.middle);
    Serial.print(" | ");
    Serial.print(testStruct.ring);
    Serial.print(" | ");
    Serial.println(testStruct.pinky);    
  }
  control();
}

void control() {
// Read joystick values
  int Throttle_Value = testStruct.thumb;  // Throttle
  int Right_Value = testStruct.pointer;  // RIGHT
  int Left_Value = testStruct.middle;   // Left
  int Rev_Enable_Value = testStruct.ring;    // Reverse Command when threshold met
  int Fwd_Enable_Value = testStruct.pinky; // Forward Command when threshold met (also overwrites Rev Command)



  // Map joystick values to speed range (0 to 255)
  int speedA = map(Throttle_Value, 0, 100, 0, 255);  // Expecting 0 to 100 value
  int speedB = map(Throttle_Value, 0, 100, 0, 255);



  // Limit speed based on X-axis position for turning
  speedA -= map(Left_Value, 0, 100, 0, 100);  // Adjust most right value for fine adjustments
  speedB -= map(Right_Value, 0, 100, 0, 100);

  // Constrain speed values to valid range
  speedA = constrain(speedA, 0, 255);
  speedB = constrain(speedB, 0, 255);

    // Serial.print(speedA);
    // Serial.print(" | ");
    // Serial.print(speedB);
    // Serial.print(" | ");

  // Set motor speeds and directions
  analogWrite(enablePinA, abs(speedA));
  analogWrite(enablePinB, abs(speedB));

    //Insure threshold of 10 is surpassed before forward or reverse movement,  Testing with 10.
  if (Fwd_Enable_Value > 10) { 
    // Joystick moved forward
    moveForward();
  } else if (Rev_Enable_Value > 10)  {
    // Joystick moved backward
    moveBackward();
  } else {
    // Joystick is centered in Y direction
    // Stop the motors
    stopMotors();
  }
  if (millis() - lastCommunicationTime > communicationTimeout) {
  // Timeout, stop the motors
    stopMotors();
  }
}

void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void moveBackward() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}

void stopMotors() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

