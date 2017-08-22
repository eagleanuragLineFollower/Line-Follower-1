//Serene Kamal & Kenny Chan Thursday 10am

#define IN1 5 //D5 
#define IN2 6 //D6
#define IN4 8 //D8
#define IN3 7 //D7

#define PwmL 11 //D11
#define PwmR 2 //D2

#define rightREC A4
#define leftREC A6
#define frontREC A2


//*******sets minimum pwm values for motors to run*********
#define leftMin 190 
#define rightMin 130
//*********************************************************


//******values used for base values during setup()*********
long zeroR, zeroL, zeroF;
int readR, readL, readF, integral, pError, error, dt;
int mapR, mapL, mapF, errorR, errorL, zeroRerror, zeroLerror;
long prevR, prevL = 0;
//*********************************************************


void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(rightREC, INPUT);
  pinMode(leftREC, INPUT);
  pinMode(frontREC, INPUT);

  //**********takes base readings**************
  for(int i = 0; i < 100; i++)
  {
    zeroR += analogRead(rightREC);
    zeroL += analogRead(leftREC);
    zeroF += analogRead(frontREC);
    //Serial.println(zeroR);
  }
  zeroR /= 100;
  zeroL /= 100;
  zeroF /= 100;
  //Serial.println(zeroR);
  //********************************************

  dt = 100;
  integral = 0;
  error = 0;
}


void loop() {
  int KI = 1, KD = 0.05, KP = 2;                 //declare constants and correction on right and left
  int corrR = 0, corrL = 0;

  readValues();                               //used for analogRead and map

  if (mapL < -1000) {                         //detects whether left sensor is on the tape  
    corrR = map(mapL, -1000, -7000, 0, 65);   //map the error between 0 and 65
    
    if(mapL < -7000)                          //incase the sensor reads below -7000                        
      corrR = 65;
     
    //Serial.print("      correction R: ");
    //Serial.println(corrR*KP);
  }
  
  else if (mapR < -800) {                    //detects whether right sensor is on the tape
   // corrL = map(mapR, -1000, -5000, 0, 65);   //map the error between 0 and 65
    corrL = map(mapR, -800, -2000, 0, 65); 
    if(mapL < -5000)                          //incase the sensor reads below -5000
      corrL = 65;
    
    //Serial.print("      correction L: ");
    //Serial.println(corrL*KP);  
  }
  
  analogWrite(PwmR, rightMin + corrL*KP + (mapR - prevR)*KD);     //adjust right motor speed based on PID
  analogWrite(PwmL, leftMin + corrR*KP + (mapL - prevL)*KD);      //adjust left motor speed based on PID
  
  if (corrR > 0 && corrL > 0)                 //if both sensors are on the tape, then just move forward
     forward();

  if(corrR > 0)                               //if right sensor is on the tape, turn left                      
     turnLeft();
  
  else if(corrL > 0)                          //if left sensor is on the tape, turn right
     turnRight();

  else                                        //incase there is an unknown situation, then just move forward
    forward();

  prevR = mapR;
  prevL = mapL;

} 

void displayValues(){                   //used for debugging and printing values
//Serial.print(readR);
//Serial.print(" = RIGHT    ");
//Serial.print(readL);
//Serial.print(" = LEFT     ");
//Serial.print(" FRONT: ");
//Serial.print(mapF);
  Serial.print(" RIGHT: ");
  Serial.print(mapR);
  Serial.print( "    ");
//Serial.print(zeroR);
  Serial.print(" LEFT: ");
  Serial.print(mapL);
  Serial.print("      FRONT: ");
  Serial.println(mapF);
//Serial.println();
}

void readValues(){                      //analogReads values and maps them for left and right sensor
  readR = analogRead(rightREC);
  readL = analogRead(leftREC);
  readF = analogRead(frontREC);

  mapL = map(readL, zeroL, 0, 0, 1023);
  mapR = map(readR, zeroR, 0, 0, 1023);
  mapF = map(readF, zeroF, 0, 0, 1023);
}

void forward(){                         //H-Bridge functions
   digitalWrite(IN1, LOW);
   digitalWrite(IN2, HIGH);
   digitalWrite(IN4, HIGH);
   digitalWrite(IN3, LOW); 
}
void reverse(){
   digitalWrite(IN1, HIGH);
   digitalWrite(IN2, LOW);
   digitalWrite(IN3, HIGH);
   digitalWrite(IN4, LOW); 
}
void turnLeft(){
   digitalWrite(IN1, LOW);
   digitalWrite(IN2, HIGH);
   digitalWrite(IN3, HIGH);
   digitalWrite(IN4, LOW); 
}
void turnRight(){
   digitalWrite(IN1, HIGH);
   digitalWrite(IN2, LOW);
   digitalWrite(IN3, LOW);
   digitalWrite(IN4, HIGH); 
}
void coast(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW); 
}
void brake(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH); 
}
