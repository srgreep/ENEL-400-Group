#include "SerialTransfer.h"
#include "BluetoothSerial.h"
#define RXp2 16
#define TXp2 17
#define POINTER_ADC 32//35    //Digital pin 35
#define THUMB_ADC 33          //Digital pin 33

int thumb_value = 0;  //stores ADC variable
int pointer_value = 0;

SerialTransfer myTransfer;

struct __attribute__((packed)) STRUCT {
  int16_t thumb;
  int16_t pointer;
  int16_t middle;
  int16_t ring;
  int16_t pinky;
} testStruct;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Smart Glove RC Car");
  Serial1.begin(115200, SERIAL_8N1, RXp2, TXp2);
  myTransfer.begin(Serial1);

//test values for code...these will be replaced with sensor data
  testStruct.thumb = 5;
  testStruct.pointer = 4;
  testStruct.middle = 3;
  testStruct.ring = 2;
  testStruct.pinky = 1;
}

void loop() {
  // use this variable to keep track of how many
  // bytes we're stuffing in the transmit buffer
  uint16_t sendSize = 0;
  thumb_value  = analogRead(THUMB_ADC);
  pointer_value  = analogRead(POINTER_ADC);
  testStruct.thumb  = thumb_value;
  testStruct.pointer = pointer_value;
  Serial.println(testStruct.thumb);
  Serial.println(" | ");
  Serial.println(testStruct.pointer);
  Serial.println(" | ");
  ///////////////////////////////////////// Stuff buffer with struct
  sendSize = myTransfer.txObj(testStruct, sendSize);

  ///////////////////////////////////////// Send buffer
  myTransfer.sendData(sendSize);
  Serial.println(sizeof(testStruct));   //code from forum,used to check size of testStruct...can be deleated by user if not needed
  Serial.println(" | ");
  delay(500);
}

void getFingerValues(){
  SerialBT.Write("N");
  testStruct.pinky = SerialBT.read();
  SerialBT.Write("O");
  testStruct.pointer = serialPort.read();
  SerialBT.Write("P");
  testStruct.middle = serialPort.read();
  SerialBT.Write("Q");
  testStruct.ring = serialPort.read();
  SerialBT.Write("R");
  testStruct.pinky = serialPort.read();
}

void appConnection(){
  //Stuck here until app connection is confirmed
  while(true){
    if(SerialBT.available()){
      if (SerialBT.read() == "~"){
        Serial.println("Connected with app");
        break;
      }
    }
  }
}