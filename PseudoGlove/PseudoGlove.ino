int pinkyPin = A0;
int ringPin = A1;
int middlePin = A2;
int pointerPin = A3;
int thumbPin = A4;


void setup() {
  Serial.begin(9600);           //  setup serial
}

void loop() {
  int gloveValues[5];
  gloveValues[0] = analogRead(pinkyPin);
  gloveValues[1] = analogRead(ringPin);
  gloveValues[2] = analogRead(middlePin);
  gloveValues[3] = analogRead(pointerPin);
  gloveValues[4] = analogRead(thumbPin);

  for (int i = 0; i<5; i++){
    char high = (gloveValues[i] >>8) & 0b11;
    char low = gloveValues[i] & 0b11111111;
    Serial.write((i<<5) | high);
    Serial.write(gloveValues[i]);
    delay(250);
  }
}