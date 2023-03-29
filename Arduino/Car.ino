#include "Servo.h"


int DATOS =0;
//MOTOR
const int IN1 = 6;
const int IN2 = 7;
const int IN3 = 8;
const int IN4 = 9;
int LED = 3;
const int ENA = 5;
const int ENB = 10;
int a=0;
Servo servo1; 

void setup() {
 Serial.begin(9600);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);
  analogWrite(IN1, 0);
  analogWrite(IN2, 0); 
  analogWrite(IN3, 0);
  analogWrite(IN4, 0); 
  pinMode (LED, OUTPUT);
  servo1.attach(5); 
}

void loop() {
  // Serial.println(DATOS);
  // put your main code here, to run repeatedly:
if(Serial.available()>0)
{DATOS = Serial.read();
//ADELANTE
if(DATOS > 0 && DATOS<=100)
{int a = round(map((DATOS), 0, 100, 100, 255));
digitalWrite(LED,LOW);
  Serial.println(a);
  analogWrite(ENA, a);
  analogWrite(ENB, a); 

  digitalWrite(IN2, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(IN3, HIGH);
}


//ATRAS

if(DATOS >= 101 && DATOS<=201)
{int b = round(map((DATOS), 101, 201, 80, 255));
Serial.println(b);
digitalWrite(LED,HIGH);
  analogWrite(ENA, b);
  analogWrite(ENB, b); 

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}


//DERECHA
if(DATOS=='2')
{analogWrite(ENA, 90);
  analogWrite(ENB, 90); 

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(IN3, LOW);
}
//IZQUIERDA
if(DATOS=='1')
{analogWrite(ENA, 90);
  analogWrite(ENB, 90); 

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(IN3, HIGH);
}

//PARAR
if(DATOS >=201&& DATOS<=255)
{ analogWrite(ENA, 0);
  analogWrite(ENB, 0); 

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(IN3, LOW);
  int angle=map(DATOS, 201,255, 0, 70);
  // Repassa o angulo ao ServoWrite
  servo1.write(angle); 
}
}
}
