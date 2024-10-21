#include <Servo.h>

#include <avr/wdt.h>
#include "DeviceDriverSet_xxx0.h"

#define PIN_Motor_PWMA 5
#define PIN_Motor_PWMB 6

#define PIN_Motor_BIN_1 7
#define PIN_Motor_AIN_1 8
#define PIN_Motor_STBY 3

#define PIN_ITR20001xxxL A2
#define PIN_ITR20001xxxM A1
#define PIN_ITR20001xxxR A0

#define TrackingDetection_S 40
#define TrackingDetection_E 500
#define TrackingDetection_V 950

#define PIN_Servo_z 10


Servo myservo;

DeviceDriverSet_ULTRASONIC myUltrasonic;


uint8_t rpm_left = 50;
uint8_t rpm_right = 50;

uint8_t rpm_normal = 50;
uint8_t rpm_turn = 40;

//line detetion sensors
uint8_t sensor_tresh = 200;

float l1_distance;
float l2_distance;
float r1_distance;
float r2_distance;
float m_distance;

float turn_delay_big = 1000;
float turn_delay_small = 500;

//function to move forward
void forward() {
  digitalWrite(PIN_Motor_STBY, HIGH);
  digitalWrite(PIN_Motor_AIN_1, HIGH);
  analogWrite(PIN_Motor_PWMA, 50);
  digitalWrite(PIN_Motor_BIN_1, HIGH);
  analogWrite(PIN_Motor_PWMB, 50);
}

void rotate_right(uint8_t right, uint8_t left){
    digitalWrite(PIN_Motor_AIN_1, HIGH);
    digitalWrite(PIN_Motor_BIN_1, LOW);
    analogWrite(PIN_Motor_PWMB, right);
    analogWrite(PIN_Motor_PWMA, left);
    digitalWrite(PIN_Motor_STBY, HIGH);
}

void rotate_left(uint8_t right, uint8_t left){
  digitalWrite(PIN_Motor_AIN_1, LOW);
  digitalWrite(PIN_Motor_BIN_1, HIGH);
  analogWrite(PIN_Motor_PWMB, left);
  analogWrite(PIN_Motor_PWMA, right);
  digitalWrite(PIN_Motor_STBY, HIGH);
}
void stopMov(){
  analogWrite(PIN_Motor_PWMA, 0);
  analogWrite(PIN_Motor_PWMB, 0);
  digitalWrite(PIN_Motor_STBY, LOW);
  digitalWrite(PIN_Motor_AIN_1, LOW);
  digitalWrite(PIN_Motor_BIN_1, LOW);
}

void init_servo()
{
  myservo.attach(PIN_Servo_z, 500, 2400); //500: 0 degree  2400: 180 degree
  myservo.attach(PIN_Servo_z);
  myservo.write(90); //sets the servo position according to the 90（middle）
  delay(500);
}

static boolean comparison(long L, long M, long R, uint8_t tresh){
  if (L > tresh || M > tresh || R > tresh)
    return true;
  else
    return false;
}

int move_face(int angle){
  //code to move servo and get distance
  
  myservo.attach(PIN_Servo_z);
  myservo.write(angle);
}
  

int L_R(){
  move_face(45);//look left 
  l1_distance = myUltrasonic.DeviceDriverSet_ULTRASONIC_Test();
  delay(360);
  move_face(135);//look left 
  r1_distance = myUltrasonic.DeviceDriverSet_ULTRASONIC_Test();

  if (l1_distance == r1_distance){
    return 0;
  }
  else if (l1_distance > r1_distance){
    return 1;
  }
  else {
    return 2;
  }
}

int get_turn_dist(){
  move_face(20);//look left 
  l1_distance = myUltrasonic.DeviceDriverSet_ULTRASONIC_Test();
  delay(360);
  move_face(45);//look left 
  l2_distance = myUltrasonic.DeviceDriverSet_ULTRASONIC_Test();
  delay(360);
  move_face(135);//look right
  r1_distance = myUltrasonic.DeviceDriverSet_ULTRASONIC_Test();
  delay(360);
  move_face(160);//look right
  r2_distance = myUltrasonic.DeviceDriverSet_ULTRASONIC_Test();
  delay(360);
  move_face(90);//look straight
  m_distance = myUltrasonic.DeviceDriverSet_ULTRASONIC_Test();
  delay(360);
}

boolean isObj(int l1, int l2, int r1, int r2, int m){
    get_turn_dist();

    if(l1<=15){
        return true;    
    }
    else if(l2<=15){
        return true;    
    }
    else if(r1<=15){
        return true;    
    }
    else if(r2<=15){
        return true;
    }
    else if(m<=15){
        return true;
    }
    else{
        return false;
    }
}

void exec_turn(){
  if (l1_distance <= 15 & r1_distance <= 15 & l2_distance <= 15 & r2_distance <= 15){
       rotate_left(rpm_right, rpm_left);
       delay(1500);
       stopMov();
   }
   else if(l1_distance > r1_distance){
       if(l1_distance >= l2_distance){
           rotate_left(rpm_right, rpm_left);
           delay(turn_delay_small);
           stopMov();
       }
       else{
        rotate_left(rpm_right, rpm_left);
        delay(turn_delay_big);
        stopMov();
       }
   }
   else if(l1_distance<r1_distance){
       if(r1_distance>=r2_distance){
           rotate_right(rpm_right, rpm_left);
           delay(turn_delay_small);
           stopMov();
       }
       else{
           rotate_right(rpm_right, rpm_left);
           delay(turn_delay_big);
           stopMov();
       }
   }  
}

int lineCount = 0;

void setup() {
  pinMode(PIN_ITR20001xxxL, INPUT);
  pinMode(PIN_ITR20001xxxM, INPUT);
  pinMode(PIN_ITR20001xxxR, INPUT);
  pinMode(PIN_Motor_PWMA, OUTPUT);
  pinMode(PIN_Motor_PWMB, OUTPUT);
  pinMode(PIN_Motor_AIN_1, OUTPUT);
  pinMode(PIN_Motor_BIN_1, OUTPUT);
  pinMode(PIN_Motor_STBY, OUTPUT);

  Serial.begin(9600);

  myUltrasonic.DeviceDriverSet_ULTRASONIC_Init();

  init_servo();//init servo and set angle to stright (90)

 
}


void loop() {
 int lr = L_R();

  while (lineCount < 1){
      if(lr == 0){
          while (isObj(l1_distance, l2_distance, r1_distance, r2_distance, m_distance)){
              forward();
              delay(200); 
          }
          stopMov();
          get_turn_dist();
          exec_turn();
      }
      else if (lr = 1){//left
          exec_turn();
          while (isObj(l1_distance, l2_distance, r1_distance, r2_distance, m_distance)){
              forward();
              delay(200); 
          }
          stopMov();
          get_turn_dist();
          exec_turn();
      }
      else{//right
          exec_turn();
          while (isObj(l1_distance, l2_distance, r1_distance, r2_distance, m_distance)){
              forward();
              delay(200); 
          }
          stopMov();
          get_turn_dist();
          exec_turn();
      } 
   }
}
