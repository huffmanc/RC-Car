// connect motor controller pins to Arduino digital pins

// motor one and two
int rearMtEne = 6;
int rearMtBw = 9;
int rearMtFw = 8;
// motor three
int frontMtEne = 1;
int frontMtLeft = 7;
int frontMtRight = 2;

//sensor
int trig = 5;
int echo = 3;

char command = 0;
char state = 0;

int turnOn = false;
int reckless = false;

// BT Module
#include <SoftwareSerial.h>
SoftwareSerial BT1(10, 11); // El pin 10 es Rx y el pin 11 es Tx

void setup()
{
  
  Serial.begin(9600);
  BT1.begin(9600);
  // set all the motor control pins to outputs
  pinMode(rearMtEne, OUTPUT);
  pinMode(frontMtEne, OUTPUT);
  pinMode(rearMtFw, OUTPUT);
  pinMode(rearMtBw, OUTPUT);
  pinMode(frontMtLeft, OUTPUT);
  pinMode(frontMtRight, OUTPUT);
  
  //sensor
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  //Motor Drive
  # define MAX_SPEED 220 // 70% of top speed ==> 256
  # define MIN_SPEED 70 

  const int ON = 1;
  const int OFF = 0;
  const int RIGHT = 1;
  const int LEFT = 0;
  const int FW = 1;
  const int BW = 0;
  
  moveStop(); 

  BT1.print("MJRoBot AutoBot BT Remote Control");
  BT1.println('\n');
  
}

void checkBTcmd()  // verify if a command is received from BT remote control
 { 
    if (BT1.available()) 
    { 
      command = BT1.read();
      BT1.flush();
    }
 }

void receiveCmd ()
{
  switch (command)
  {
    case 'connected':
      command = 0;
      BT1.print("YES");
      BT1.println('\n');
      delay(200);
      break;
    
    case 'p': 
      Serial.println("command ==> p"); 
      turnOn = !turnOn;
      command = 0;
      BT1.print(" COMMAND ON/OFF");
      BT1.println('\n');
      delay(200); //Delay to call attention to mode change
      break;
      
   case 'm': //not used here
      reckless = !reckless;
      command = 0;
      BT1.print(" reckless ON/OFF");
      BT1.println('\n');
      delay(200);
      break;
  }   
}

void manualCmd()
{
  switch (command)
  {
    
    case 'f': 
      moveStop(); //turn off both motors
      state = command;
      break;

    case 'w':  
      moveForward(); 
      state = command;
      break;

    case 'd':     
      moveRight();
      state = command;
      break;

   case 'a': 
      moveLeft();
      state = command;
      break;
    
    case 's':  
      moveBackward();
      state = command;
      break;

    case 'v':
      turnStop();
      state = command;
      break;

  //if (!reckless){
//    long duration, distance;
//    digitalWrite(trig, LOW);
//    delayMicroseconds(2);
//    digitalWrite(trig, HIGH);
//    delayMicroseconds(10);
//    digitalWrite(trig, LOW);
//  
//    duration = pulseIn(echo, HIGH);
//    distance = (duration/2)/ 29.1;
//
//    Serial.println(distance);
//
//    delay(20);
//
//    if (distance< 15) {
//      moveStop();
//      turnStop();
//      sensorRegister();
//    }
//  }
  }
}

void moveStop() //turn off all motors
{ 
  analogWrite(rearMtEne, LOW);
  digitalWrite(rearMtFw, LOW);
  digitalWrite(rearMtBw, LOW);
  
  delay(5);
}

void turnStop() //revert steering to straight.
{
  digitalWrite(frontMtEne, LOW);
  digitalWrite(frontMtLeft, LOW);
  digitalWrite(frontMtRight, LOW);

  delay(5);
}

//******************************************************************************//  

void moveForward() // rear motor FW
{ 
  analogWrite(rearMtEne, 200);
  digitalWrite(rearMtFw, HIGH);
  digitalWrite(rearMtBw, LOW);
  digitalWrite(frontMtEne, LOW);

  delay(5);

}

//******************************************************************************//  

void moveBackward() // rear motor BW
{ 
  analogWrite(rearMtEne, 200);
  digitalWrite(rearMtFw, LOW);
  digitalWrite(rearMtBw, HIGH);
  digitalWrite(frontMtEne, LOW);

  delay(5);
  
}

//******************************************************************************//  

void moveLeft() // front motor left
{ 
  digitalWrite(frontMtEne, HIGH);
  digitalWrite(frontMtLeft, HIGH);
  digitalWrite(frontMtRight, LOW);

  delay(5);
}

//******************************************************************************//  

void moveRight() // front motor right
{ 
  digitalWrite(frontMtEne, HIGH);
  digitalWrite(frontMtLeft, LOW);
  digitalWrite(frontMtRight, HIGH);

  delay(5);
  
}

void sensorRegister()
{
  moveBackward();
  moveRight();
  delay(1000);
  moveStop();
  turnStop();
}

//******************************************************************************//  

void stopRobot ()
{  
  state = 0;
  moveStop(); //turn off both motors
}


void loop() 
{  
  checkBTcmd();  // verify if a comand is received from BT remote control
  receiveCmd ();

if (turnOn) manualCmd ();
  else stopRobot ();
}



