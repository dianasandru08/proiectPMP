#include <Servo.h>          //Servo motor library. This is standard library
#include <NewPing.h>  

int en1 = 2;
int in1 = 3;
int in2 = 4;
int in3 = 5;
int in4 = 6;
int en2 = 7;

#define trig_pin A1 //analog input 1
#define echo_pin A2 //analog input 2

#define maximum_distance 200
boolean goesForward = false;
int distance = 100;

NewPing sonar(trig_pin, echo_pin, maximum_distance); //sensor function
Servo servo_motor; //our servo name


void setup() {
  // put your setup code here, to run once:
  pinMode(en1, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(en2, OUTPUT);

   servo_motor.attach(10); //our servo pin

  servo_motor.write(115);
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);

}

void loop() {
  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);

  if (distance <= 35){
    moveStop();
    delay(300);
    moveBackward();
    delay(400);
    moveStop();
    delay(300);
    distanceRight = lookRight();
    delay(300);
    distanceLeft = lookLeft();
    delay(300);

    if (distanceRight >= distanceLeft){
      turnRight();
      moveStop();
    }
    else{
      turnLeft();
      moveStop();
    }
  }
  else{
    moveForward(); 
  }
    distance = readPing();

}


int lookRight(){  
  servo_motor.write(50);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(115);
  return distance;
}

int lookLeft(){
  servo_motor.write(170);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(115);
  return distance;
  delay(100);
}

int readPing(){
  delay(70);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}


void moveStop(){
  digitalWrite(en1, 100);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(en2, 100);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
   
  
}

void moveForward(){

 


  digitalWrite(en1, 100);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(en2, 100);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

   
  
}

void moveBackward(){

 
   digitalWrite(en1, 100);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(en2, 100);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
 
  
}

void turnRight(){
  digitalWrite(en1, 100);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(en2, 100);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  
  delay(2000);

   digitalWrite(en1, 100);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(en2, 100);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
 
  
  
}

void turnLeft(){

   digitalWrite(en1, 100);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(en2, 100);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  delay(2000);

  digitalWrite(en1, 100);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(en2, 100);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
 
}
