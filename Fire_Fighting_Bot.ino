


/*------ Arduino Fire Fighting Robot ---- */
#include <SoftwareSerial.h>   //include SoftwareSerial.h library
#include <Servo.h>            //include servo.h library  

Servo myservo;
 
int pos = 0;    
boolean fire = false;

int bt_data; // variable to receive data from the serial port

int mode=1;


#define rxPinBluetooth 2
#define txPinBluetooth 3

#define GAS_SENSOR 11    //Gas sensor

#define Left 10      // left sensor
#define Right 8    // right sensor
#define Forward 9   //front sensor

#define RM1 4       // left motor
#define RM2 5       // left motor
#define LM1 6       // right motor
#define LM2 7       // right motor
#define pump 12

SoftwareSerial BT_Serial(rxPinBluetooth,txPinBluetooth);

void setup()
{

  BT_Serial.begin(9600);
  Serial.begin(9600);

  
  pinMode(Left, INPUT);
  pinMode(Right, INPUT);
  pinMode(Forward, INPUT);
  pinMode(GAS_SENSOR, INPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(pump, OUTPUT);
 
  myservo.attach(13);
  myservo.write(90); 

  digitalWrite(pump, HIGH);
  
}
 
 
void loop()
{
  
  myservo.write(90); //Sweep_Servo();  

// Serial.println(BT_Serial.available());
  if(BT_Serial.available() > 0) //if some date is sent, reads it and saves in state  
  {     
    bt_data = BT_Serial.read();  
  }

  if(bt_data == 8)  //Auto
  {
    mode=1;
  }
  else if(bt_data == 9)    //Manual 
  {
    mode=0;
  } 

  if(mode==0){     
//===============================================================================
//                          Key Control
//=============================================================================== 
  if(bt_data == 1){ forward(); }  // if the bt_data is '1' the DC motor will go forward
  else if(bt_data == 2){ backward(); }  // if the bt_data is '2' the motor will Reverse
  else if(bt_data == 3){ turnLeft(); }  // if the bt_data is '3' the motor will turn left
  else if(bt_data == 4){ turnRight(); } // if the bt_data is '4' the motor will turn right
  else if(bt_data == 5){ Stop(); }
  else if(bt_data == 6){put_off_fire();}
  delay(10);
  }
//===============================================================================
//                          Auto Control
//=============================================================================== 
  else{
    if (digitalRead(Left) == 1 && digitalRead(Right)==1 && digitalRead(Forward) == 1) //không  phát hiện cháy
    {
      delay(500);
      Stop();
      delay(500);
    }
    else if (digitalRead(Forward) == 0) // phát hiện cháy ở phía trước
    {
      forward();
      fire = true;
    }
      
    else if (digitalRead(Left) == 0){ turnLeft(); }  //phát hiện cháy ở phía bên trái
      
    else if (digitalRead(Right) == 0){ turnRight(); }  //phát hiện cháy ở phía bên phải
    delay(200);//change this value to change the distance
      
    if(digitalRead(GAS_SENSOR) == 0)
    {
      Serial.println("Gas is Detected.");
      put_off_fire();
    }
    
    while (fire == true)
    {
      put_off_fire();
      Serial.println("Fire Detected.");
    }
  }
}



void put_off_fire()
{
  Stop();    
  digitalWrite(pump,LOW);
  delay(300);
 
  for (pos = 50; pos <= 130; pos += 1) 
  { 
    myservo.write(pos); 
    delay(10);  
  }
  for (pos = 130; pos >= 50; pos -= 1) { 
    myservo.write(pos); 
    delay(10);
  }
  digitalWrite(pump,HIGH);
  myservo.write(90); 
  fire=false;
}

void forward()  //forward
{ 
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, LOW);
  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, LOW);
}

void backward() //backward
{ 
  digitalWrite(RM1, LOW);  //Right Motor forward Pin 
  digitalWrite(RM2, HIGH); //Right Motor backward Pin 
  digitalWrite(LM1, LOW); //Left Motor backward Pin 
  digitalWrite(LM2, HIGH);  //Left Motor forward Pin 
}

void turnRight()  //turnRight
{
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, HIGH);
  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, LOW);
}

void turnLeft()  //turnLeft
{
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, LOW);
  digitalWrite(LM1, LOW);
  digitalWrite(LM2, HIGH);
}

void Stop()  //stop
{
  digitalWrite(RM1, LOW); //Right Motor forword Pin 
  digitalWrite(RM2, LOW); //Right Motor backword Pin 
  digitalWrite(LM1, LOW); //Left Motor backword Pin 
  digitalWrite(LM2, LOW); //Left Motor forword Pin 
}
