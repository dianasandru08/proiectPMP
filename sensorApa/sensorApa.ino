int resval = 0;  // holds the value
int respin = A0; // sensor pin used
int led = 8;
  
void setup() { 
 
  // start the serial console
  Serial.begin(9600);
  pinMode(led, OUTPUT);
} 
  
void loop() { 
   
  resval = analogRead(respin); //Read data from analog pin and store it to resval variable
   
  if (resval<=100){ 
    Serial.println("Water Level: Empty");
    digitalWrite(led, HIGH);}
  else if (resval>100 && resval<=300){
    Serial.println("Water Level: Low"); 
    digitalWrite(led, LOW);} 
  else if (resval>300 && resval<=350){ 
    Serial.println("Water Level: Medium");
    digitalWrite(led, LOW);}
  else if (resval>350){ 
    Serial.println("Water Level: High");
    digitalWrite(led, LOW); 
  }
  delay(1000); 
}
