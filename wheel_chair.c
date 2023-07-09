#include <Servo.h>
Servo myservo; 
// function declaration
void motor_forward();
void motor_back();
void motor_left();
void motor_right();
void motor_stop();

//ultrasonic snesor declaration
const int  trigpin=8;
const int echopin=7;
long duration;
int distance;
int distance_left;
int distance_right;
int minimum_safe_distance=20;
// motor pins declaration
const int motor_right_in1=3; // h bridge motor right
const int motor_right_in2=5;
const int motor_left_in1=6;// h bridge motor left
const int motor_left_in2=11;

// data from blue tooth
char command= '0';
int speed_factor=150;
void setup() {
  
// begin serial communication

myservo.attach(9); 
myservo.write(90);
delay(1000);
myservo.write(0);
delay(1000);
myservo.write(180);
delay(1000);
myservo.write(90);
delay(1000);
Serial.begin(9600);
// motor pin declaration
pinMode(motor_right_in1,OUTPUT);
pinMode(motor_right_in2,OUTPUT);
pinMode(motor_left_in1,OUTPUT);
pinMode(motor_left_in2,OUTPUT);

// ultrasonic pin declaration
pinMode(trigpin,OUTPUT);
pinMode(echopin,INPUT);

// set all motors off condition
analogWrite(motor_right_in1, 0);
analogWrite(motor_right_in2, 0);
analogWrite(motor_left_in1, 0);
analogWrite(motor_left_in2, 0);
speed_factor=0;


}

void loop() {


  //check for serial data from bluetooth module
  if(Serial.available()>0)
  {
    // copy data to local variable
    command=Serial.read();
   // Serial.println(command);
  }

// measure distance from ultrasonic sensor
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);
  duration=pulseIn(echopin,HIGH);
  distance=duration*0.034/2;
 // Serial.println("distance is:");
 // Serial.println(distance);0
 // Serial.println();
 // Serial.println();

 
  if(command=='F'){

    if(distance>=minimum_safe_distance){
      motor_forward();

     // Serial.println("forward command detected");
      
    }
    else{
      motor_stop();
      // move servo to left
      myservo.write(180);
      delay(1000);
      // measure distance left
      digitalWrite(trigpin,HIGH);
      delayMicroseconds(10);
      digitalWrite(trigpin,LOW);
      duration=pulseIn(echopin,HIGH);
      distance_left=duration*0.034/2;
      Serial.println(distance_left);
      delay(100); // give some time for measuring
      
      // move servo to right
      myservo.write(0);
      delay(1000);
      // measure distance right
      digitalWrite(trigpin,HIGH);
      delayMicroseconds(10);
      digitalWrite(trigpin,LOW);
      duration=pulseIn(echopin,HIGH);
      distance_right=duration*0.034/2;
      Serial.println(distance_right);
      delay(100); // give some time for measuring
      
      //return servo to face forward
         myservo.write(90);
      
      if((distance_left<minimum_safe_distance)&&(distance_right<minimum_safe_distance)){
         command='S';
         
         // if bothe left and right distance is less than minimum_safe_distance the stop.
         // in future add reverse logic here so that robot comes back a bit and then tries to scan again.
      }
      else{
      
        if(distance_left>minimum_safe_distance){
          Serial.println("turning left");
          // left side gap is more.
          motor_left(); // move left for 2 sesconds
          delay(1000);
          //motor_forward(); // the move forward
        }
        else if(distance_right>minimum_safe_distance){
          Serial.println("turning right");
          // right side gap is more.
          motor_right(); // move right for 2 sesconds
          delay(1000);
          //motor_forward(); // the move forward
        }
      }
      
      
    }
    
  }
  else if(command=='B'){motor_back();}
  else if(command=='L'){motor_left();}
  else if(command=='R'){motor_right();}
  else if(command=='S'){motor_stop();}
  else if(command=='0'){speed_factor=96;}
  else if(command=='1'){speed_factor=110;}
  else if(command=='2'){speed_factor=140;}
  else if(command=='3'){speed_factor=160;}
  else if(command=='4'){speed_factor=200;}
 // Serial.println("Speed setting is:");
 // Serial.println(speed_factor);
  //delay(1000);
}


void motor_forward(){
  analogWrite(motor_right_in1,speed_factor);
  analogWrite(motor_right_in2,0);
  analogWrite(motor_left_in1,speed_factor);
  analogWrite(motor_left_in2,0);
  }

void motor_back(){
  analogWrite(motor_right_in1,0);
  analogWrite(motor_right_in2,speed_factor);
  analogWrite(motor_left_in1,0);
  analogWrite(motor_left_in2,speed_factor);

  }

void motor_left(){
 analogWrite(motor_right_in1,speed_factor);
  analogWrite(motor_right_in2,0);
  analogWrite(motor_left_in1,0);
  analogWrite(motor_left_in2,speed_factor);
  }

void motor_right(){
analogWrite(motor_right_in1,0);
  analogWrite(motor_right_in2,speed_factor);
  analogWrite(motor_left_in1,speed_factor);
  analogWrite(motor_left_in2,0);
  }

void motor_stop(){
 analogWrite(motor_right_in1,0);
 analogWrite(motor_right_in2,0);
 analogWrite(motor_left_in1,0);
 analogWrite(motor_left_in2,0);}
