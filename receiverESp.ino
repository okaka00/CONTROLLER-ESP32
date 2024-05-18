#include "WiFi.h"
#include "esp_now.h"
#include <ESP32Servo.h> 

#define m1 27
#define m2 26
#define m3 14
#define m4 12
#define enA 13
#define enB 25
#define servo 4


#define vrx_init 2560
#define vry_init 2630
#define UP_THRESHOLD    1000
#define DOWN_THRESHOLD  4000


Servo myservo;
//make sure all the variable here same with sender code
typedef struct struct_message {
  int redValue;
  int whiteValue;
  int valueX;
  int valueY;
  int swValue;
}struct_message;

//reading data message
struct_message readingData;

int vrx = 0;
int vry = 0; 
int turn_left;
int turn_right;
int val;
int c = 0;

//function that receive incomming data from the sender esp. 
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len){
  memcpy(&readingData, incomingData, sizeof(readingData));
  
  vry = readingData.valueY;
  vrx = readingData.valueX;
  turn_left = readingData.whiteValue;
  turn_right = readingData.redValue;
  val = readingData.swValue;
   digitalWrite(m1, LOW);
    digitalWrite(m2, LOW);
    digitalWrite(m3, LOW);
    digitalWrite(m4, LOW);

    // depends on what condition you want for to control you motor direction, and depends on your joystick value

  if(vrx < UP_THRESHOLD ){
     Serial.print("foward");

    digitalWrite(m1, HIGH);
    digitalWrite(m2, LOW);
    digitalWrite(m3, HIGH);
    digitalWrite(m4, LOW);
    analogWrite(enA, 100);
    analogWrite(enB, 100);
  }
  if(vrx > DOWN_THRESHOLD){
    Serial.print("down");

    digitalWrite(m1, LOW);
    digitalWrite(m2, HIGH);
    digitalWrite(m3, LOW);
    digitalWrite(m4, HIGH);
    analogWrite(enA, 100);
    analogWrite(enB, 100);
  }

if(turn_left == 1){
    digitalWrite(m1, HIGH);
    digitalWrite(m2, LOW);
    digitalWrite(m3, LOW);
    digitalWrite(m4, HIGH);
    analogWrite(enA, 100);
    analogWrite(enB, 80);
}
  if(turn_right == 1){
    digitalWrite(m1, LOW);
    digitalWrite(m2, HIGH);
    digitalWrite(m3, HIGH);
    digitalWrite(m4, LOW);
    analogWrite(enA, 80);
    analogWrite(enB, 100);
}
if(val == 0){
  if(c == 0){
    myservo.write(30);
    c=1;
  } 
  if(c==1){
    myservo.write(0);
    c=0;
  }
}
  
    
}

void setup() {
  //setup all the mode here
  Serial.begin(115200);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(m4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  myservo.attach(servo);
  
  myservo.write(0);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // the main loop will be empty since all the receive data loopiing at the sender esp
}
