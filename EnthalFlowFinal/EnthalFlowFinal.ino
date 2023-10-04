#include <VirtualWire.h>
#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial Blue(1,0); //HC-05 Bluetooth (RXD, TXD)
const int trigPin = 7; //Funny pin stuff
const int echoPin = 6; 
const int MAX = 90; //Maximum angle
bool isConnected = false;
long int data;

long duration; //duration for distance calc
int distance; //distance calc
int pos = 0; //Position 
Servo myservo; //Servo motor

long int turnOn = 1984; //on and off values
long int turnOff = 1917;
int count = 0; //to check if a cycle has been reached
String output = ""; //output to android application
int minDist = 9999;
int count = 1;

bool invert = false;
bool isDanger = false;

void setup() {
  myservo.attach(8); //Servo init
  pinMode(trigPin, OUTPUT); //stuff
  pinMode(echoPin, INPUT);
  Serial.begin(9600); //start stuff
  Blue.begin(9600);
}
void loop() {
  
  if (!isConnected) { //if it's not connected
    if(Blue.available() > 0) { //attempt connection
      data = Blue.parseInt();
    if (data == turnOn) {
      isConnected = true;}
    }
    else {
      isConnected = false;
    }
    }
   else {
     output = ""; minDist = 9999; invert = true;
     
     for (pos = 0; pos <= MAX; pos += 1) //first pass
     {
        if (pos == MAX || pos == MAX/4 || pos == MAX/2 || pos == 3*MAX/4) see(); else delay(5);
        myservo.write(pos); //servo position          
     }
     if (minDist < 50) {
        isDanger = true;
     }
     Blue.write(isDanger + ", " + output);

     output = ""; minDist = 9999; invert = true; count = 5;
     
     for (pos = MAX; pos >= 0; pos -= 1) //reverse pass
      {
        if (pos == 0 || pos == MAX/4 || pos == MAX/2 || pos == 3*MAX/4) see(); else delay(5);
        myservo.write(pos); //servo position
      }
      Blue.write(isDanger + ", " + output);
   }
  
}

void see()
{
  //ultrasonic input
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  
  distance = duration * 0.034 / 2; //Distance calculation

  //Printing information to the console
  Serial.print("Distance: ");
  Serial.println(distance);
  if (distance < minDist) {
    minDist = distance;
    output = count;
  }
  if (invert = true) {
    count++;
  }
  else {
    count--;
  }
}
