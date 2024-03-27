#include <BluetoothSerial.h>
#include <Arduino.h>
#include <Preferences.h>


//**************************************************************************************
// ---- Macro-definitions ----

// -- ADC Pins --
#define THUMB_ADC 4              //Digital pin 34
#define POINTER_ADC 2           //Digital pin 35
#define MIDDLE_ADC 15         //Digital pin 32
#define RING_ADC 13            //Digital pin 33
#define PINKY_ADC 12           //Digital pin 25

// -- Input Pins --
#define CALIBRATE_BTN 26  //Digital pin 26

//-- Output pins --
#define LED_CALIBRATE 32

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
int inputBT;               //Bluetooth Variable
BluetoothSerial SerialBT;  //Bluetooth serial functionality variable

// -- Non-Volatile Memoery --
Preferences preferences; 

// -- State Variables --
int state = POWER_ON_STATE;  //Setting state to default ON-state (extern)
bool connected_flag = 0;        //Default as low  (extern)
bool calibrate_flag = 0;        //Default as low  (extern)
bool transmit_flag = 0;         //Default as low  (extern)

// -- Calibration Variables --
int  thumb_min = 4095;    //Min ADC value
int  thumb_max = 0;       //Max ADC value
int  pointer_min = 4095;  //Min ADC value
int  pointer_max = 0;     //Max ADC value
int  middle_min = 4095;   //Min ADC value
int  middle_max = 0;      //Max ADC value
int  ring_min = 4095;     //Min ADC value
int  ring_max = 0;        //Max ADC value
int  pinky_min = 4095;    //Min ADC value
int  pinky_max = 0;       //Max ADC value


  //Values for taking average during calibration
int sample_count = 0;
unsigned long  thumb_avg = 0;      
unsigned long  pointer_avg = 0;    
unsigned long  middle_avg = 0;      
unsigned long  ring_avg = 0;        
unsigned long  pinky_avg = 0;   

int closed_thumb_value = 2600;
int closed_pointer_value = 2600;
int closed_middle_value = 2050;
int closed_ring_value = 2250;
int closed_pinky_value = 2950;

int open_thumb_value = 3200;
int open_pointer_value = 3300;
int open_middle_value = 2500;
int open_ring_value = 2900;
int open_pinky_value = 3350;

//-- Timer flag --
bool timer_flag = 0;
//**************************************************************************************
// ---- Data types ----

// // -- Receive structure --
// esp_now_peer_info_t receiver_info;      //Creating "receiver_info" as esp_now_peer_info data-type (extern)

// -- Transmit Data --
//Data type for transmission
typedef struct struct_message_t {
  char thumb;
  char pointer;
  char middle;
  char ring;
  char pinky;
} struct_message_t;

struct_message_t finger_data;  //Creating "finger_data" as struct-type to be transmitted (extern)

// -- Timer --
hw_timer_t *My_timer = NULL;

//**************************************************************************************
// ---- Interrupt ----

// -- Calibrate button Interrupt --
//  -Using polling function until can figure out why I am getting an error
void calibration_btn_polling() {
  if (!digitalRead(CALIBRATE_BTN)) {
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
void IRAM_ATTR onTimer() {
  timer_flag = 1;
}

//**************************************************************************************
//  ---- GPIO Functions  ----

//Read ADC values from the felx-sensors
void read_ADC_values() {
  thumb_value = analogRead(THUMB_ADC);
  pointer_value = analogRead(POINTER_ADC);
  middle_value = analogRead(MIDDLE_ADC);
  ring_value = analogRead(RING_ADC);
  pinky_value = analogRead(PINKY_ADC);
}

void floor_ceiling_struct() {
  thumb_value = max(thumb_value, thumb_min);
  thumb_value= min(thumb_value, thumb_max);
  pointer_value = max(pointer_value, pointer_min);
  pointer_value = min(pointer_value, pointer_max);
  middle_value = max(middle_value, middle_min);
  middle_value = min(middle_value, middle_max);
  ring_value = max(ring_value, ring_min);
  ring_value = min(ring_value, ring_max);
  pinky_value = max(pinky_value, pinky_min);
  pinky_value = min(pinky_value, pinky_max);
}

//Set the transmission structer variables
void populate_structure() {
  //Set data to ADC values (May be replaced with different value in the future if the actions are set on this side)
  read_ADC_values();
  floor_ceiling_struct();
  finger_data.thumb = 100 - map(thumb_value, thumb_min, thumb_max, 0, 100);
  finger_data.pointer = 100 - map(pointer_value, pointer_min, pointer_max, 0, 100);
  finger_data.middle = 100 - map(middle_value, middle_min, middle_max, 0, 100);
  finger_data.ring = 100 - map(ring_value, ring_min, ring_max, 0, 100);
  finger_data.pinky = 100 - map(pinky_value, pinky_min, pinky_max, 0, 100);
}

//Check ADC values if they are the same, and set transmit data
void check_ADC_values() {
  //If values are not the same
  populate_structure();  //Set transmit structire to new values
  transmit_flag = 1;     //Set transmit flag
}

void GPIO_init() {
  pinMode(CALIBRATE_BTN, INPUT_PULLUP);
  pinMode(LED_CALIBRATE, OUTPUT);
  digitalWrite(LED_CALIBRATE, LOW);
}

//**************************************************************************************
//  ---- Bluetooth Functions ----

//Initlization of the BT
void BT_init() {
  SerialBT.begin("Smart Glove V2");  //Device name
}

//Checks if the ESP32 is connected to the computer
void check_connection() {
  if (SerialBT.available()) {
    connected_flag = 1;                 //If bluetooth is connected set connected_flag high
    digitalWrite(LED_CALIBRATE, HIGH);  //Write LED high if connected
  } else {
    connected_flag = 0;                //If bluetooth is not connected set connected_flag low
    digitalWrite(LED_CALIBRATE, LOW);  //Write LED low if not connected
  }
}

//Selects which data to transmit based off value recived from computer
void BT_data_select(int inputBT) {
  if (inputBT == 126) {
    uint8_t buffer[sizeof(struct_message_t)];
    memcpy(buffer, &finger_data, sizeof(struct_message_t));
    SerialBT.write(buffer, sizeof(buffer));
  }
  
}

//Transmits the finger values via BT
void BT_Transmit() {
  //Check connection of ESP to device
  check_connection();

  //Based off value recived from computer send coressponding finger data
  while (!SerialBT.available()) {
  }
  inputBT = SerialBT.read();
  BT_data_select(inputBT);

  //Check if last value requested is sent
  BT_end_of_Tx_check();
}

//Checks if the last value to be sent is transmited
void BT_end_of_Tx_check() {
  if (inputBT == 82)
    transmit_flag = 0;
}
//**************************************************************************************
//  ---- Calibrate Functions  ----

//Timer initilization function
void TIMER_init() {
  My_timer = timerBegin(0, 240, true);             //Setting hardware timer(0), pre-scaler(80), count-up(true)
  timerAttachInterrupt(My_timer, &onTimer, true);  //Connecting "onTimer" function for timer interrupt
  timerAlarmWrite(My_timer, 5000000, true);        //Setting 5s timer
}

// //Find the max and min values for calibration
// void find_max_min_values() {
//   timerAlarmEnable(My_timer);  //Enable timer (5s till interrupt for calibrate)
//   reset_min_max();
//   while (timer_flag == 0) {
//     find_max_min();
//   }
//   timerAlarmDisable(My_timer);  //Disable timer (5s till interrupt for calibrate)
//   timer_flag = 0;
//   Serial.print("Leave Find\n");
// }

void reset_min_max() {
  thumb_min, pointer_min, middle_min, ring_min, pinky_min = 4095;
  thumb_max, pointer_max, middle_max, ring_max, pinky_max = 0;
}


//---------------------------

void reset_avg_values(){
  thumb_avg = 0;
  pointer_avg = 0;
  middle_avg = 0;
  ring_avg = 0;  
  pinky_avg = 0;
}

void find_max_values(){
  reset_avg_values();
  sample_count = 0;  //Reset sample count for average

  //Start of sampling to user
  digitalWrite(LED_CALIBRATE, HIGH);  

  //Sample each finger 100 times
  while(sample_count < 1000){
    thumb_avg = thumb_avg + analogRead(THUMB_ADC);
    pointer_avg = pointer_avg + analogRead(POINTER_ADC);
    middle_avg = middle_avg + analogRead(MIDDLE_ADC);
    ring_avg = ring_avg + analogRead(RING_ADC);
    pinky_avg = pinky_avg + analogRead(PINKY_ADC);
    sample_count++;
  }

  //Take average
  thumb_min = thumb_avg / 1000;
  pointer_min = pointer_avg / 1000;
  middle_min = middle_avg / 1000;
  ring_min = ring_avg / 1000;
  pinky_min = pinky_avg / 1000;

  Serial.print("Thumb max:");
  Serial.println(thumb_max);
}

void find_min_values(){
  reset_avg_values();
  sample_count = 0;  //Reset sample count for average

  //Sample each finger 100 times
  while(sample_count < 1000){
    thumb_avg = thumb_avg + analogRead(THUMB_ADC);
    pointer_avg = pointer_avg + analogRead(POINTER_ADC);
    middle_avg = middle_avg + analogRead(MIDDLE_ADC);
    ring_avg = ring_avg + analogRead(RING_ADC);
    pinky_avg = pinky_avg + analogRead(PINKY_ADC);
    sample_count ++;
  }

  //Take average
  thumb_max = thumb_avg / 1000;
  pointer_max = pointer_avg / 1000;
  middle_max = middle_avg / 1000;
  ring_max = ring_avg / 1000;
  pinky_max = pinky_avg / 1000;

  digitalWrite(LED_CALIBRATE, HIGH); 
  Serial.print("Thumb min:");
  Serial.println(thumb_min);
}

void find_values(){
  digitalWrite(LED_CALIBRATE, HIGH); 
  delay(3000);        //Delay for 3s to allow time for the user
  Serial.print("Enter");

  while(1){
    //Check that all values are below the open hand value -> delay 2 seconds -> check again (if true go to find min values)
    if (check_open()){
      Serial.println("Open Enter");
      blink();
      
      //Second check for if statement
      if (check_open()) {
        Serial.println("Open Enter 2");
        digitalWrite(LED_CALIBRATE, HIGH); 
        find_min_values();
        
        while(1){
          if(check_closed()){
            Serial.println("Closed Enter3");
            blink();
            find_max_values();
            calibrate_flag = 0;
            return;
          }
        }
      }    
    }

    //Check that all values are below the open hand value -> delay 2 seconds -> check again (if true go to find min values)
    if (check_closed()){
      Serial.println("Closed Enter");
      blink();
      
      //Second check for if statement
      if (check_closed()) {
        Serial.println("Closed Enter2");
        digitalWrite(LED_CALIBRATE, HIGH); 
        find_max_values();
        while(1){
          if(check_open()){
            Serial.println("Open Enter3");
            blink();
            find_min_values();
            calibrate_flag = 0;
            return;
          }
        }
      }    
    }

    Serial.println("loop");
  }
}

//Check if hand is closed
int check_open(){
  read_ADC_values();  //Read ADC
  if (thumb_value > open_thumb_value &&
      pointer_value > open_pointer_value &&
      //middle_value < open_middle_value &&
      ring_value > open_ring_value &&
      pinky_value > open_pinky_value)
      return 1;
  else
    return 0;
}

//Check if hand is closed
int check_closed(){
  read_ADC_values();
  if (thumb_value < closed_thumb_value &&
      pointer_value < closed_pointer_value &&
      //middle_value > closed_middle_value &&
      ring_value < closed_ring_value &&
      pinky_value < closed_pinky_value)
      return 1;
  else 
    return 0;
}

//Blink LED in 0.5s intervals for 5 loops
void blink(){
  for(int i = 0; i < 3; i++){
      digitalWrite(LED_CALIBRATE, HIGH);  
      delay(500);
      digitalWrite(LED_CALIBRATE, LOW);  
      delay(500);
  }
}

void print_adc_values(){
  Serial.print("thumb:");
  Serial.println(thumb_value);
  Serial.print("pointer:");
  Serial.println(pointer_value);
  Serial.print("middle:");
  Serial.println(middle_value);
  Serial.print("ring:");
  Serial.println(ring_value);
  Serial.print("pinky:");
  Serial.println(pinky_value);
}
//**************************************************************************************
//  ---- Memory Functions  ----

//Write the maximum and minimum values to NVS
void write_max_min_to_NVS(){
  preferences.begin("my-app", false);   //Initlize NVS for writing

  //-- Write Values --
  //Thumb
  preferences.putUInt("thumb_min", thumb_min);      // Write value to NVS with key "thumb_min"
  preferences.putUInt("thumb_max", thumb_max);      // Write value to NVS with key "thumb_max"
  //Pointer
  preferences.putUInt("pointer_min", pointer_min);  // Write value to NVS with key "pointer_min"
  preferences.putUInt("pointer_max", pointer_max);  // Write value to NVS with key "pointer_max"
  //Middle
  preferences.putUInt("middle_min", middle_min);    // Write value to NVS with key "middle_min"
  preferences.putUInt("middle_max", middle_max);    // Write value to NVS with key "middle_max"
  //Ring
  preferences.putUInt("ring_min", ring_min);        // Write value to NVS with key "ring_min"
  preferences.putUInt("ring_max", ring_max);        // Write value to NVS with key "ring_max"
  //Pinky
  preferences.putUInt("pinky_min", pinky_min);      // Write value to NVS with key "pinky_min"
  preferences.putUInt("pinky_max", pinky_max);      // Write value to NVS with key "pinky_max"

  preferences.end();      //End access to NVS
}

void read_max_min_from_NVS(){
  preferences.begin("my-app", true); // Namespace "my-app", read-only mode enabled

  thumb_min = preferences.getUInt("thumb_min", 0);      //Read from NVS with key "thumb_min" (0 is default value if key not found)
  thumb_max =  preferences.getUInt("thumb_max", 4095);     //Read from NVS with key "thumb_min" (0 is default value if key not found)
  pointer_min = preferences.getUInt("pointer_min", 0);  //Read from NVS with key "pointer_min" (0 is default value if key not found)
  pointer_max = preferences.getUInt("pointer_max", 4095);  //Read from NVS with key "pointer_max" (0 is default value if key not found)   
  middle_min = preferences.getUInt("middle_min", 0);    //Read from NVS with key "middle_min" (0 is default value if key not found)
  middle_max = preferences.getUInt("middle_max", 4095);    //Read from NVS with key "middle_max" (0 is default value if key not found)
  ring_min = preferences.getUInt("ring_min", 0);        //Read from NVS with key "ring_min" (0 is default value if key not found)
  ring_max = preferences.getUInt("ring_max", 4095);        //Read from NVS with key "ring_max" (0 is default value if key not found)   
  pinky_min = preferences.getUInt("pinky_min", 0);      //Read from NVS with key "pinky_min" (0 is default value if key not found)    
  pinky_max = preferences.getUInt("pinky_max", 4095);      //Read from NVS with key "pinky_max" (0 is default value if key not found)  
}

//**************************************************************************************
//  ---- State-Machine Functions  ----

void power_on_state() {
  // //Debugging
  Serial.print("power_on_state\n");

  //State loop
  while (state == POWER_ON_STATE) {
    // -- State Checking --
    if (connected_flag == 1) {  //Device successfully connected -> "Data read" state (flag set in "OnDataSent" function -> in Wirelss_COMS file )
      state = DATA_READ_STATE;
    }

    //-- Check Connection --
    check_connection();  //Call "check_connection" function -> Wirelss_COMS files
    calibration_btn_polling();

    //Debug
    // read_ADC_values();
    // print_adc_values();
    // Serial.print("\n");
    // delay(2000);
  }
}

void data_read_state() {
  ////Debugging
  Serial.print("data_read_state\n");
  //delay(2000);
  //-----------

  //State loop
  while (state == DATA_READ_STATE) {
    // -- State Checking --
    if (connected_flag == 0)
      state = POWER_ON_STATE;
    else if (calibrate_flag == 1)
      state = CALIBRATION_STATE;
    else if (transmit_flag == 1)
      state = TRANSMIT_STATE;
      
    check_ADC_values();  //Check and set the transmit structure data
    calibration_btn_polling();
  }
}

void calibration_state() {
  ////Debugging
  Serial.print("calibration_state\n");
  ///---------------

  //State loop
  while (state == CALIBRATION_STATE) {
    //-- Call calibration function ---
    Serial.print("Max & Min\n");
    digitalWrite(LED_CALIBRATE, LOW);
    find_values();
    // find_max_values();
    // find_min_values();
    write_max_min_to_NVS();
    check_connection();

    // -- State Checking --
    if (connected_flag == 0)
      state = POWER_ON_STATE;
    else if (calibrate_flag == 0)
      state = DATA_READ_STATE;

  }
  digitalWrite(LED_CALIBRATE, HIGH);
}

void transmission_state() {
  ////Debugging
  //Serial.print("transmission_state\n");
  //delay(2000);

  //State loop
  while (state == TRANSMIT_STATE) {
    // -- State Checking --
    if (connected_flag == 0)  //Device disconnects -> "power on" state (flag set in "OnDataSent" function -> in Wirelss_COMS file )
      state = POWER_ON_STATE;
    else if (transmit_flag == 0)  //Successful transmission -> "Data read" state (flag set in "OnDataSent" function -> in Wirelss_COMS file )
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
  Serial.begin(9600);       
  BT_init();                //Bluetooth initilization
  GPIO_init();              //GPIO initilization
  TIMER_init();             //Timer initilization
  read_max_min_from_NVS();  //Read values for finger max and min values from NVS
}

//**************************************************************************************
//  ---- Main Loop  ----

void loop() {
  switch (state) {
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
