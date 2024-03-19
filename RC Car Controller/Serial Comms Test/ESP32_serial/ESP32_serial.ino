/**
 * Install SerialTransfer and BluetoothSerial Libraries
 * Connect pins ####
 * 
 * This code connects with the Windows Form App via bluetooth.
 * Recieves flex values from glove
 * Commands RC to move accordingly
*/

#include "SerialTransfer.h"
#include "BluetoothSerial.h"
#define RXp2 16
#define TXp2 17
#define POINTER_ADC 32//35    //Digital pin 35
#define THUMB_ADC 33          //Digital pin 33

int thumb_value = 0;  //stores ADC variable
int pointer_value = 0;

SerialTransfer myTransfer;
BluetoothSerial SerialBT;
struct __attribute__((packed)) STRUCT {
  int8_t thumb;
  int8_t pointer;
  int8_t middle;
  int8_t ring;
  int8_t pinky;
} testStruct;

void setup() {
  Serial.begin(9600);
  SerialBT.begin("Smart Glove RC Car");
  Serial1.begin(9600, SERIAL_8N1, RXp2, TXp2);
  myTransfer.begin(Serial1);
}

void loop() {
  // use this variable to keep track of how many
  // bytes we're stuffing in the transmit buffer
  uint16_t sendSize = 0;
  getFingerValues();
  Serial.print(testStruct.thumb);
  Serial.print(" | ");
  Serial.print(testStruct.pointer);
  Serial.print(" | ");
  Serial.print(testStruct.middle);
  Serial.print(" | ");
  Serial.print(testStruct.ring);
  Serial.print(" | ");
  Serial.println(testStruct.pinky);
  ///////////////////////////////////////// Stuff buffer with struct
  sendSize = myTransfer.txObj(testStruct, 5);

  ///////////////////////////////////////// Send buffer
  myTransfer.sendData(sendSize);
  //Serial.println(sizeof(testStruct));   //code from forum,used to check size of testStruct...can be deleated by user if not needed
  //Serial.println(" | ");
  //delay(500);
}

void getFingerValues(){
  appConnection(); //wait for connection
  char input[5];
  if(SerialBT.available()){
    SerialBT.readBytes(input, 5);
  }
  
  testStruct.thumb = (uint8_t)input[0];
  testStruct.pointer = (uint8_t)input[1];
  testStruct.middle = (uint8_t)input[2];
  testStruct.ring = (uint8_t)input[3];
  testStruct.pinky = (uint8_t)input[4];
}
//Waits for a prompt from app to tell it is ready to send values
void appConnection(){
  //Stuck here until app connection is confirmed
  while(true){
    if(SerialBT.available()){
      if (SerialBT.read() == 126){
        Serial.println("Connected with app");
        break;
      }
    }
  }
}