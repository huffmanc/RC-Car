#include <SoftwareSerial.h>  

#define gas A0
#define brake A1
#define left 6
#define right 5
#define mode 13
#define start 12

int rightstate = 0;
int leftstate = 0;
int Gval = 0;
int Bval = 0;
//int Mval = 0;
//int Sval = 0;

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup()
{
  
  Serial.begin(9600);  // Begin the serial monitor at 9600bps

  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$$$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600

// Automatically connect
  bluetooth.print("$$$");
  Serial.print("Trying command \n");
  delay(2000);
  bluetooth.print("connected");
  if ((char)Serial.read() != "YES");
{
    
//  Entering command mode
  if ((char)Serial.read() != "CMD");
  {  
  bluetooth.print("$$$");
  Serial.print("Trying command \n");
  delay(2000);
  }

  // Searching for connection
  if ((char)Serial.read() == "CMD");
    {
    Serial.print("Trying search \n"); 
    delay(1000);
    bluetooth.println("I,7");
    delay(12000);
    }
    
  // Trying to connect
  if ((char)Serial.read() == "Inquiry Done");
     { 
     Serial.print("Trying connect \n");
     delay(1000);
     bluetooth.println("C,201709290552");
     delay(5000);
     }
}
  if ((char)Serial.read() == "YES");
  {
    bluetooth.println("---");
  }

pinMode(gas, INPUT);
pinMode(brake, INPUT);
pinMode(left, INPUT);
pinMode(right, INPUT); 
pinMode(mode, INPUT);
pinMode(start, INPUT);  
bluetooth.println("p");
}

void loop()
{ 
 if(bluetooth.available())  // If the bluetooth sent any characters
  {
    // Send any characters the bluetooth prints to the serial monitor
    Serial.print((char)bluetooth.read());  
  }

  if(Serial.available())  // If stuff was typed in the serial monitor
  {
    // Send any characters the Serial monitor prints to the bluetooth
    bluetooth.print((char)Serial.read());
  }
  // loops bluetooth and serial tramission!

  Gval = analogRead(gas);
  Bval = analogRead(brake);

  
//if(digitalRead(start) == HIGH){
//  bluetooth.print('p');
//  bluetooth.print('\n');
//}
//
//if(digitalRead(mode) == LOW){
//  bluetooth.print('m');
//  bluetooth.print('\n');
//}

if(Gval > 200)
  {
  bluetooth.print('w');
  bluetooth.print('\n');
  }

else if(Bval > 200)
  {   
  bluetooth.print('s');
  bluetooth.print('\n');
  }

if (Gval <= 200 and Bval <=200)
  {
   bluetooth.print('f');
   bluetooth.print('\n');
  }

leftstate = digitalRead(left);
  if (leftstate == LOW) {
   bluetooth.print('a');
   bluetooth.print('\n');
  }

rightstate = digitalRead(right);
  if (rightstate == LOW) {
   bluetooth.print('d');
   bluetooth.print('\n');
   delay(10);
  }

else if (rightstate && leftstate == HIGH) {
   bluetooth.print('v');
   bluetooth.print('\n');
   delay(10);
}
}

