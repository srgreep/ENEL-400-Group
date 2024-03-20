#include "BluetoothSerial.h"
#include <Arduino.h>


//**************************************************************************************
// ---- Macro-definitions ----

// -- ADC Pins --
#define THUMB_ADC 32//34      //Digital pin 34
#define POINTER_ADC 35//35    //Digital pin 35
#define MIDDLE_ADC 34//32     //Digital pin 32
#define RING_ADC 39//33       //Digital pin 33
#define PINKY_ADC 36//25      //Digital pin 25

// -- Input Pins --
#define CALIBRATE_BTN 26  //Digital pin 26

//-- Output pins --
#define LED_CALIBRATE 27 

//  -- State Macros --
#define POWER_ON_STATE 0
#define DATA_READ_STATE 1
#define CALIBRATION_STATE 2
#define TRANSMIT_STATE 3

//**************************************************************************************
// ---- Variables ----

// -- ADC Values --
int thumb_value = 0;    //stores ADC variable
int pointer_value = 0;  //stores ADC variable
int middle_value = 0;   //stores ADC variable
int ring_value = 0;     //stores ADC variable
int pinky_value = 0;    //stores ADC variable

// // -- Wireless COMS --
int inputBT;                //Bluetooth Variable
BluetoothSerial SerialBT;   //Bluetooth serial functionality variable


// -- State Variables --
int state = CALIBRATION_STATE;        //Setting state to default ON-state (extern)
bool connected_flag = 0;              //Default as low  (extern)
bool calibrate_flag = 0;              //Default as low  (extern)
bool transmit_flag = 0;               //Default as low  (extern)

// -- Calibration Variables --
int thumb_min = 4095;   //Min ADC value 
int thumb_max = 0;      //Max ADC value

int pointer_min = 4095; //Min ADC value
int pointer_max = 0;    //Max ADC value

int middle_min = 4095;  //Min ADC value
int middle_max = 0;     //Max ADC value

int ring_min = 4095;    //Min ADC value
int ring_max = 0;       //Max ADC value

int pinky_min = 4095;   //Min ADC value
int pinky_max = 0;      //Max ADC value

//-- Timer flag --
bool timer_flag = 0;
//**************************************************************************************
// ---- Data types ----

// // -- Receive structure --
// esp_now_peer_info_t receiver_info;      //Creating "receiver_info" as esp_now_peer_info data-type (extern)

// -- Transmit Data --
//Data type for transmission
typedef struct struct_message_t {      
  int thumb;
  int pointer;
  int middle;
  int ring;
  int pinky;
} struct_message_t;

struct_message_t finger_data;          //Creating "finger_data" as struct-type to be transmitted (extern)

// -- Timer --
hw_timer_t *My_timer = NULL;

//**************************************************************************************
// ---- Interrupt ----

// -- Calibrate button Interrupt --
//  -Using polling function until can figure out why I am getting an error
void calibration_btn_polling(){
  if(!digitalRead(CALIBRATE_BTN)){
    calibrate_flag = 1;
    //digitalWrite(LED_CALIBRATE, HIGH);
    Serial.print("CAl flag\n");
  }
}

// void calibrate_ISR(){
//   calibrate_flag = 1;   //Set "calibrate" flag -> enter "Calibrate" state
// }

// attachInterrupt(CALIBRATE_BTN, calibrate_ISR, LOW); //Set pin "CALIBRATE_BTN" to have an interrupt on input "LOW" -> calls "calibrate_ISR" function

// -- Timer Interrupt --
void IRAM_ATTR onTimer(){
  timer_flag = 1;
} 

//**************************************************************************************
//  ---- GPIO Functions  ----

//Read ADC values from the felx-sensors
void read_ADC_values(){
  thumb_value = analogRead(THUMB_ADC);
  pointer_value = analogRead(POINTER_ADC);
  middle_value = analogRead(MIDDLE_ADC);
  ring_value = analogRead(RING_ADC);  
  pinky_value = analogRead(PINKY_ADC);
}

//Set the transmission structer variables
void set_transmit_data(){
  //Set data to ADC values (May be replaced with different value in the future if the actions are set on this side)
  finger_data.thumb = abs(map(thumb_value, thumb_min, thumb_max, 0, 100));             //calibration_scale(thumb_value, thumb_min, thumb_max);
  finger_data.pointer = abs(map(pointer_value, pointer_min, pointer_max , 0, 100));    //calibration_scale(pointer_value, pointer_min, pointer_max);
  finger_data.middle = abs(map(middle_value, middle_min, middle_max, 0, 100));         //calibration_scale(middle_value, middle_min, middle_max);
  finger_data.ring = abs(map(ring_value, ring_min, ring_max, 0, 100));                 //calibration_scale(ring_value, ring_min, ring_max);
  finger_data.pinky = abs(map(pinky_value, pinky_min, pinky_max, 0, 100));             //calibration_scale(pinky_value, pinky_min, pinky_max);     //thumb = 0 ? <0 : thumb;  
}

//Check ADC values if they are the same, and set transmit data
void check_ADC_values(){
  //Check if values are the same (does not have tolerance yets)
  if(thumb_value == finger_data.thumb &
     pointer_value == finger_data.pointer &
     middle_value == finger_data.middle &
     ring_value == finger_data.ring &
     middle_value == finger_data.pinky)
    return;

  //If values are not the same
  set_transmit_data();        //Set transmit structire to new values
  transmit_flag = 1;          //Set transmit flag
}

void GPIO_init(){
  pinMode(CALIBRATE_BTN, INPUT_PULLUP);
  pinMode(LED_CALIBRATE, OUTPUT);
  digitalWrite(LED_CALIBRATE, LOW);
}

//**************************************************************************************
//  ---- Bluetooth Functions ----

//Initlization of the BT
void BT_init(){
  SerialBT.begin("Smart Glove");    //Device name
}

//Checks if the ESP32 is connected to the computer
void check_connection(){
  if (SerialBT.available()) {
    connected_flag = 1;                 //If bluetooth is connected set connected_flag high
    digitalWrite(LED_CALIBRATE, HIGH);  //Write LED high if connected
  }
  else{
    connected_flag = 0;                //If bluetooth is not connected set connected_flag low
    digitalWrite(LED_CALIBRATE, LOW);  //Write LED low if not connected
  }
}

//Selects which data to transmit based off value recived from computer
void BT_data_select(int intputBT){
  switch (inputBT){
        case 78:
          SerialBT.println(finger_data.pinky);
          break;
        case 79:
          SerialBT.println(finger_data.ring);
        break;
        case 80:
          SerialBT.println(finger_data.middle);
        break;
        case 81:
          SerialBT.println(finger_data.pointer);
        break;
        case 82:
          SerialBT.println(finger_data.thumb);
        break;
        default:
        break;
      }
}

//Transmits the finger values via BT
void BT_Transmit(){
  //Check connection of ESP to device
  check_connection();

  //Based off value recived from computer send coressponding finger data
  inputBT = SerialBT.read();
  BT_data_select(inputBT);

  //Check if last value requested is sent
  BT_end_of_Tx_check();
}

//Checks if the last value to be sent is transmited
void BT_end_of_Tx_check(){
  if(inputBT == 82)
    transmit_flag = 0;
}
//**************************************************************************************
//  ---- Calibrate Functions  ----

//Timer initilization function
void TIMER_init(){
  My_timer = timerBegin(0, 240, true);             //Setting hardware timer(0), pre-scaler(80), count-up(true)
  timerAttachInterrupt(My_timer, &onTimer, true); //Connecting "onTimer" function for timer interrupt
  timerAlarmWrite(My_timer, 5000000, true);       //Setting 5s timer
}

//Find the max and min values for calibration
void find_max_min_values(){
  Serial.print("Enter Find\n"); 
  reset_min_max();              //Reset min & max variables

  timerAlarmEnable(My_timer);   //Enable timer (5s till interrupt for calibrate)

  while(timer_flag == 0){
    read_ADC_values();          //Read ADC values of flex sensors
    check_above_or_below();     //Check if values are above of below max & min value
  }

  timerAlarmDisable(My_timer);  //Disable timer (5s till interrupt for calibrate)
  timer_flag = 0;
  
  Serial.print("Leave Find\n");
}

//Check and set the max and min values after each ADC read for the calibrate function
void check_above_or_below(){
  //Thumb - Check if ADC value is above or below min
  if(thumb_value < thumb_min)
    thumb_min = thumb_value;
  if(thumb_value > thumb_max)
    thumb_max = thumb_value;

  //Pointer - Check if ADC value is above or below min
  if(pointer_value < pointer_min)
    pointer_min = pointer_value;
  if(pointer_value > pointer_max)
    pointer_max = pointer_value;
    
  //Middle - Check if ADC value is above or below min
  if(middle_value < middle_min)
    middle_min = middle_value;
  if(middle_value > middle_max)
    middle_max = middle_value;

  //Ring - Check if ADC value is above or below min
  if(ring_value < ring_min)
    ring_min = ring_value;
  if(ring_value > ring_max)
    ring_max = ring_value;

  //Pinky - Check if ADC value is above or below min
  if(pinky_value < pinky_min)
    pinky_min = pinky_value;
  if(pinky_value > pinky_max)
    pinky_max = pinky_value;
}

// //returns value of 0-100 for range given by max and min calibrate function
// int calibration_scale(int ADC_value, int min_value, int max_value){
//     return ((ADC_value - min_value)/(max_value - min_value)) * 100;
// }

//Resets the max and min values for re-calibration
void reset_min_max(){
  int thumb_min = 4095;   //Min ADC value 
  int thumb_max = 0;      //Max ADC value

  int pointer_min = 4095; //Min ADC value
  int pointer_max = 0;    //Max ADC value

  int middle_min = 4095;  //Min ADC value
  int middle_max = 0;     //Max ADC value

  int ring_min = 4095;    //Min ADC value
  int ring_max = 0;       //Max ADC value

  int pinky_min = 4095;   //Min ADC value
  int pinky_max = 0;      //Max ADC value
}

//**************************************************************************************
//  ---- State-Machine Functions  ----

void power_on_state(){  
  // //Debugging
  // Serial.print("power_on_state\n");

  //State loop
  while(state == POWER_ON_STATE){
    // -- State Checking --
    if(connected_flag == 1){    //Device successfully connected -> "Data read" state (flag set in "OnDataSent" function -> in Wirelss_COMS file )
      state = DATA_READ_STATE;
    }

    //-- Check Connection --
    check_connection();         //Call "check_connection" function -> Wirelss_COMS files
    calibration_btn_polling();
  }
}

void data_read_state(){
  ////Debugging
  //Serial.print("data_read_state\n");
  //delay(2000);
  //-----------

  //State loop
  while(state == DATA_READ_STATE){
    // -- State Checking --
    if (connected_flag == 0)
      state = POWER_ON_STATE;
    else if(calibrate_flag == 1)
      state = CALIBRATION_STATE;
    else if (transmit_flag == 1)
      state = TRANSMIT_STATE;

    // -- ADC Data --
    read_ADC_values();    //Read ADC values
    check_ADC_values();   //Check and set the transmit structure data
    calibration_btn_polling();
  }
}

void calibration_state(){
  ////Debugging
  //Serial.print("calibration_state\n");
  ///---------------

  //State loop
  while(state == CALIBRATION_STATE){
    // -- State Checking --
    if (connected_flag == 0)
      state = POWER_ON_STATE;
    else if(calibrate_flag == 0)
      state = DATA_READ_STATE;

    //-- Call calibration function ---
    Serial.print("Max & Min\n");
    digitalWrite(LED_CALIBRATE, LOW);
    find_max_min_values();
    calibrate_flag = 0;
  }
  digitalWrite(LED_CALIBRATE, HIGH);
}

void transmission_state(){
  ////Debugging
  //Serial.print("transmission_state\n");
  //delay(2000);

  //State loop
  while(state == TRANSMIT_STATE){
    // -- State Checking --
    if (connected_flag == 0)      //Device disconnects -> "power on" state (flag set in "OnDataSent" function -> in Wirelss_COMS file )
      state = POWER_ON_STATE;
    else if(transmit_flag == 0)   //Successful transmission -> "Data read" state (flag set in "OnDataSent" function -> in Wirelss_COMS file )
      state = DATA_READ_STATE;

    // -- Sending Data --
    BT_Transmit();
    
    //--Calibrate--
    calibration_btn_polling();
  }

}

//**************************************************************************************
//  ---- Setup Function  ----

void setup() {
  Serial.begin(9600); //FOR DEBUGGING
  BT_init();          //Bluetooth initilization
  GPIO_init();       //GPIO initilization 
  TIMER_init();      //Timer initilization

}

//**************************************************************************************
//  ---- Main Loop  ----

void loop() {
  switch(state){
    case POWER_ON_STATE:
      power_on_state();
      break;

    case DATA_READ_STATE:
      data_read_state();
      break;

    case CALIBRATION_STATE:
      calibration_state();
      break;

    case TRANSMIT_STATE:
      transmission_state();
      break;

  }

}


