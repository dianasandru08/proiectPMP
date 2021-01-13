#include <NewPing.h>
#include <Servo.h>   

#define TRIGGER_PIN  A1  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     A2  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
int distance;

int en1 = 2;
int in1 = 3;
int in2 = 4;
int in3 = 5;
int in4 = 6;
int en2 = 7;

int resval = 0;  // holds the value
int respin = A0; // sensor pin used
int led = 8;

char incomingValue = 0;
char incomingValue1 = 0;
char incomingValue2 = 0;
char incomingValue3 = 0;
char incomingValue4 = 0;
int ok = 0;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
Servo servo_motor; //our servo name;

void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
  servo_motor.attach(10); //our servo pin

  pinMode(led, OUTPUT);
  
  pinMode(en1, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(en2, OUTPUT);

  servo_motor.write(115);
  delay(2000);
  distance = readPing();
  delay(2000);
  distance = readPing();
  delay(2000);
  distance = readPing();
  delay(2000);
  distance = readPing();
  delay(3000);
  //Serial.println("Ma uita la dreapta:");
  //lookRight();
  //delay(3000);
  //Serial.println("Ma uita la stanga:");
  //lookLeft();
  //delay(3000);

  servo_motor.write(50);
  delay(1000); //1000
  servo_motor.write(115);

}

void loop() {

  if(Serial.available() > 0){
    Serial.print("ajung in if serial");
    incomingValue = Serial.read();
    if(incomingValue == '1'){
      
      moveStop();
      delay(500);
    }

     else if(incomingValue == '2'){
        moveForward();
        delay(500);
    }
     else if(incomingValue == '3'){
        moveBackward();
        delay(500);
      }
     else if(incomingValue == '4'){
        turnLeft();
        delay(500);
      }
    else if(incomingValue == '5'){
        turnRight();
        delay(500);
      }
       delay(500);
    }

  else {
    int distanceRight = 0;
  int distanceLeft = 0;
  //delay(2000);

  if (distance <= 40){
    moveStop();
    delay(300); //3000
    moveBackward();
    delay(300); //3000
    moveStop();
    delay(300); //3000
    distanceRight = lookRight();
    delay(300); //3000
    distanceLeft = lookLeft();
    delay(300); //3000

    if (distanceRight >= distanceLeft){
      turnRight();
      delay(300);
      moveStop();
    }
    else{
      turnLeft();
      delay(300);
      //delay(1000)
      moveStop();
    }
  }
  else{ 
    moveForward(); 
  }
    distance = readPing();
  }


  // /////////////////////////////////////////////////////////////////////// 
  resval = analogRead(respin); //Read data from analog pin and store it to resval variable
   
   Serial.print("|");
   Serial.println(resval);
  if (resval<=100){ 
   // Serial.println("Water Level: Empty");
    digitalWrite(led, HIGH);}
  else if (resval>100 && resval<=300){
   // Serial.println("Water Level: Low"); 
    digitalWrite(led, LOW);} 
  else if (resval>300 && resval<=350){ 
    //Serial.println("Water Level: Medium");
    digitalWrite(led, LOW);}
  else if (resval>350){ 
   // Serial.println("Water Level: High");
    digitalWrite(led, LOW); 
  }
  delay(200); 
  // ////////////////////////////////////////////////////////////////////////

    
  
}

int readPing(){
  delay(70);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}

int lookRight(){  
  servo_motor.write(50);
  delay(500); //1000
  int distance = readPing();
  //delay(500); //1000
  servo_motor.write(115);
  return distance;
}

int lookLeft(){
  servo_motor.write(170);
  delay(500); //1000
  int distance = readPing();
  //delay(500); //1000
  servo_motor.write(115);
  return distance;
  delay(100);
}




void moveStop(){
  digitalWrite(en1, 50);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(en2, 50);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
   
  
}

void moveForward(){

  digitalWrite(en1, 50);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(en2, 50);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

   

}

void moveBackward(){

   digitalWrite(en1, 50);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(en2, 50);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
 
  
}

void turnRight(){
  digitalWrite(en1, 50);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(en2, 50);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  
  delay(300); //2000

   digitalWrite(en1, 50);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(en2, 50);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
 
  
  
}

void turnLeft(){

   digitalWrite(en1, 50);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(en2, 50);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  delay(300); //2000

  digitalWrite(en1, 50);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(en2, 50);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
 
}
