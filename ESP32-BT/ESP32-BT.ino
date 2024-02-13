#include "BluetoothSerial.h"
#define thumb 32 
#define pointer 35
#define middle 34
#define ring 39
#define pinky 36

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(9600);
  SerialBT.begin("ESP32test");
  delay(1000);
}

void loop() {
  int inputBT;
  if (SerialBT.available()) {
    inputBT = SerialBT.read();
    switch (inputBT){
      case 78:
        SerialBT.println(analogRead(pinky));
        break;
      case 79:
        SerialBT.println(analogRead(ring));
      break;
      case 80:
        SerialBT.println(analogRead(middle));
      break;
      case 81:
        SerialBT.println(analogRead(pointer));
      break;
      case 82:
        SerialBT.println(analogRead(thumb));
      break;
      default:
      break;
    }
  }
}
