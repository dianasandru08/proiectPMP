#include <NewPing.h>
#include <Servo.h>   

#define TRIGGER_PIN  A1  //  trigger pin al senzorului de distanta (senzorului ultrasonic)
#define ECHO_PIN     A2  // echo pin al senzorului de distanta (senzorului ultrasonic)
#define MAX_DISTANCE 200 // distanta maxima la care putem percepe distanta dintre obiecte si robotel.
   
                          // senzorul de distanta "suporta" aproximativ 500 cm distanta 
int distance;  // variabila in care se stocheaza distanta masurata de sezorul de distanta dintre el si cel mai apropiat obiect care ii sta in cale

//pinii de pe placuta asociati motoraselor
int en1 = 2;  //enable al motorului1 + motorului3 (motoarele de pe aceeasi parte a robotelului) 
int in1 = 3;
int in2 = 4;
int in3 = 5;
int in4 = 6;
int en2 = 7; //enable al motorului2 + motorului4 (motoarele de pe aceeasi parte a robotelului) 

int resval = 0;  // valoarea masurata de senzorul de apa
int respin = A0; // pinul folosit pentru senzorul de nivel apa
int led = 8;

char incomingValue = 0; //caracterul primit din aplicatia mobila care transmite comenzi de miscare catre robotel. Comunicatia este seriala (serial.begin())

int ok = 0;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // setup-ul pentru NewPing (pinii + distanta maxima).
Servo servo_motor; // servomotorul

void setup() {
  Serial.begin(9600); // anuntam comunicarea seriala dintre placuta arduino (montaj) si aplicatia mobila
  servo_motor.attach(10); //servo pin

  pinMode(led, OUTPUT); //led pin care se aprinde doar atunci cand rezervorul de apa este gol

  //instantiam iesirile din puntea H L298N (care vor controla motoarele)
  pinMode(en1, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(en2, OUTPUT);

  servo_motor.write(115); //setam ca servomotorul sa fie intr-o pozitie valida (robotelul sa fie indreptat cu senzorul de distanta in fata) 
  delay(2000);
  distance = readPing(); //calculez distanta dintre robot(senzor de distanta) si primul obiect care intervine in raza robotului prin functia readPing()
  delay(2000);
  distance = readPing(); //repetam aceasta citire de distanta de 4 ori
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

  servo_motor.write(50); //robotelul va incepe deplasarea si se va uita in partea dreapta ca dovada a inc eperii cu succes a programului
  delay(1000); //1000
  servo_motor.write(115); //robotelul revine cu senzorul de distanta la starea initiala (indreptata in fata)

}

void loop() {

 ///////////////////////////////////CONTROLUL ROBOTULUI DIN APLICATIE//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if(Serial.available() > 0){  // daca primim date serial (de la aplicatia mobila)
    Serial.print("ajung in if serial"); //afisez un mesaj in monitorul serial
    incomingValue = Serial.read(); //citesc caracterul care se trimite din aplicatie
    if(incomingValue == '1'){ //daca acesta este '1' atunci robotelul se opreste pentru o jumatate de secunda minim sau pentru perioada de timp cat se mentine butonul respectiv din aplicatie apasat
      
      moveStop();
      delay(500);
    }

     else if(incomingValue == '2'){ //daca acesta este '2' atunci robotelul se indreapta in fata pentru o jumatate de secunda minim sau pentru perioada de timp cat se mentine butonul respectiv din aplicatie apasat
        moveForward();
        delay(500);
    }
     else if(incomingValue == '3'){//daca acesta este '3' atunci robotelul merge in spate pentru o jumatate de secunda minim sau pentru perioada de timp cat se mentine butonul respectiv din aplicatie apasat
        moveBackward();
        delay(500);
      }
     else if(incomingValue == '4'){ //daca acesta este '4' atunci robotelul o ia la stanga pentru o jumatate de secunda minim sau pentru periada de timp cat se mentine butonul respectiv din aplicatie apasat
        turnLeft();
        delay(500);
      }
    else if(incomingValue == '5'){//daca acesta este '1' atunci robotelul o ia la dreapta pentru o jumatate de secunda minim sau pentru periada de timp cat se mentine butonul respectiv din aplicatie apasat
        turnRight();
        delay(500);
      }
       delay(500);
    }
////////////////////////////////////////////////////////////////////DEPLASAREA ROBOTULUI AUTOMATA PRIN EVITAREA OBSTACOLELOR////////////////////////////////////////////////////////////////////////////////////
  else { //daca nu se primesc date serial, robotelul urmeaza "traseul" clasic: evita obstacolele
  int distanceRight = 0; //variabila in care se stocheaza distanta dintre robotel si cel mai apropiat obiect de la dreapta sa
  int distanceLeft = 0; //variabila in care se stocheaza distanta dintre robotel si cel mai apropiat obiect de la stanga sa
  //delay(2000);

  if (distance <= 40){ //daca distanta (pentru primul loop este calculata in setup) este mai mica de 40, atunci vom lua in considerare faptul ca suntem aproape de un obiect si e posibila ciocnirea cu acesta
    moveStop(); //robotelul se opreste
    delay(300); //3000
    moveBackward(); //merge in spate pentru a putea vizualiza teritoriul si a vedea posibilele directii in care o poate lua
    delay(300); //3000
    moveStop(); //se opreste din nou pentru a vizualiza
    delay(300); //3000
    distanceRight = lookRight(); //calculeaza distanta dintre robotel si cel mai apropiat obiect de la dreapta sa
    delay(300); //3000
    distanceLeft = lookLeft(); //calculeaza distanta dintre robotel si cel mai apropiat obiect de la stanga sa
    delay(300); //3000

    if (distanceRight >= distanceLeft){ //compara distantele intre ele pentru a o lua intr-o directie: stanga sau dreapta(directia optima)
      turnRight(); //daca distanta la dreapat e mai mica, robotul o va lua in dreapta
      delay(300);
      moveStop(); //robotul se opreste
    }
    else{
      turnLeft(); //daca distanta la dreapat e mai mica, robotul o va lua in stanga
      delay(300);
      //delay(1000)
      moveStop(); //robotul se opreste
    }
  }
  else{ 
    moveForward(); //daca robotul nu detecteaza nimic in fata lui (la o distanta mai mica de 40 cm, atunci o va lua tot inainte)
  }
    distance = readPing(); //se calculeaza din nou distanat dintre robot si cel mai apropiat obiect din calea lui.
  }


  // ///////////////////////////////////////////////////SENZORUL DE NIVEL DE APA//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  resval = analogRead(respin); // citim analog valoarea furnizata de senzorul de apa si o stocam in variabila resval
   
   Serial.print("|"); //concatenam  mesajul initial transmis serial cu caracterul "|" 
   Serial.println(resval); // concatenam mesajul transmis serial cu valoarea din resval, aceste lucru este efcetuat pentru o buna functionare a transmiterii aceste valori citita de la senzor in aplicatie
  if (resval<=100){  //daca valoarea e mai mica decat 100, rezervorul este gol, iar ledul aprins.
   // Serial.println("Water Level: Empty");
    digitalWrite(led, HIGH);}
  else if (resval>100 && resval<=300){ //daca valoarea este mai mare decat 100, avem diferite nivele de apa in rezervor, deci rezervorul nu e gol si ledul ramane stins
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

//functia de readPing foloseste functia specifica librariei newPing (sonar.ping_cm()) care calculeaza distanta dintre senzorul de distanta si cel mai apropiat obiect 
int readPing(){
  delay(70);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}

//functie care implica activitatea servomotorului care va misca senzorul de distanta la dreapta pentru a calcula distanta dintre el si cel mai apropiat obicet din dreapta lui dupa care va reveni la starea initiala (indreptat in fata)
int lookRight(){  
  servo_motor.write(50);
  delay(500); //1000
  int distance = readPing();
  //delay(500); //1000
  servo_motor.write(115);
  return distance;
}
 //functie care implica activitatea servomotorului care va misca senzorul de distanta la stanga pentru a calcula distanta dintre el si cel mai apropiat obicet din stanga lui dupa care va reveni la starea initiala (indreptat in fata)
int lookLeft(){
  servo_motor.write(170);
  delay(500); //1000
  int distance = readPing();
  //delay(500); //1000
  servo_motor.write(115);
  return distance;
  delay(100);
}



//functie care va opri rotirea rotilor
void moveStop(){
  digitalWrite(en1, 50); //enable pentru motorul 1 si 3
  digitalWrite(in1, LOW); //atat vcc cat si gnd acestor motoare sunt puse pe LOW
  digitalWrite(in2, LOW);
  digitalWrite(en2, 50); //enable pentru motorul 2 si 4
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
   
  
}

//functie care va determina miscarea rotilor in fata 
void moveForward(){

  digitalWrite(en1, 50); //enable
  digitalWrite(in1, HIGH); //vcc este pus pe HIGH
  digitalWrite(in2, LOW); //gnd este pus pe LOW, deci motorul va roti roata in fata
  digitalWrite(en2, 50);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

   

}

//functie care va determina miscarea rotilor in spate
void moveBackward(){

   digitalWrite(en1, 50); //enable
  digitalWrite(in1, LOW); //vcc este pus pe LOW
  digitalWrite(in2, HIGH); //gnd este pus pe HIGH, deci motorul va roti roata in sens invers (in spate)
  digitalWrite(en2, 50);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
 
  
}

//functie care va determina miscarea rotilor la dreapta
void turnRight(){
  digitalWrite(en1, 50);
  digitalWrite(in1, HIGH); //motoarele vor comanda rotile de pe partea stanga sa o ia in fata
  digitalWrite(in2, LOW);
  digitalWrite(en2, 50);
  digitalWrite(in3, LOW); //motoarele vor comanda rotile de pe partea dreapta sa o ia in spate
  digitalWrite(in4, HIGH);
  //astfel se creeaza directionarea spre dreapta a robotului pentru 0,3 sec
  
  delay(300); //2000

//motoarele vor misca rotile in fata
   digitalWrite(en1, 50);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(en2, 50);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
 
  
  
}
//functie care va determina miscarea rotilor la stanga
void turnLeft(){

   digitalWrite(en1, 50);
  digitalWrite(in1, LOW); //rotile de pa partea stanga o iau in spate
  digitalWrite(in2, HIGH);
  digitalWrite(en2, 50);
  digitalWrite(in3, HIGH); //rotile de pe partea dreapta o iau in fata
  digitalWrite(in4, LOW);
  //se produce directionarea robotelului spre stanga
  delay(300); //2000
 //in cele din urma robotelul se indreapta in fata
  digitalWrite(en1, 50);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(en2, 50);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
 
}
