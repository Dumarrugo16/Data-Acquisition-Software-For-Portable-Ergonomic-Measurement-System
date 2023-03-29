#include <Wire.h>
#include <SoftwareSerial.h>
#include "UbidotsMicroESP8266.h"

//INTERNET CONNECTION
#define TOKEN "BBFF-wPIQu4VAQrgQnUyZ3JmqSDEpzy9ZHH" // Your Ubidots TOKEN
//#define WIFISSID "UNE_6254"//WIFI NETWORK NAME
//#define PASSWORD "15043301" // Your Wifi Pass*/
#define WIFISSID "DESKTOP-6DVSBLV 9373"//WIFI NETWORK NAME
#define PASSWORD "633u04P{" // Your Wifi Pass*/
#define DEVICE  "382b78043128"  // Put here your Ubidots device label
#define VARIABLE  "var_led"  // Put here your Ubidots variable label


/*#define WIFISSID "DESKTOP-6DVSBLV 9373"//WIFI NETWORK NAME
#define PASSWORD "633u04P{" // Your Wifi Pass*/
Ubidots Client(TOKEN);
int PWMcar;
// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;

// Select SDA and SCL pins for I2C communication 
const uint8_t scl = D6;
const uint8_t sda = D7;

const byte  Pin_led = D8; 


SoftwareSerial mySerial(D2, D5); //Tx, Rx

// sensitivity scale factor respective to full scale setting provided in datasheet 
const uint16_t AccelScaleFactor = 16384;
const uint16_t GyroScaleFactor = 131;

// MPU6050 few configuration register addresses
const uint8_t MPU6050_REGISTER_SMPLRT_DIV   =  0x19;
const uint8_t MPU6050_REGISTER_USER_CTRL    =  0x6A;
const uint8_t MPU6050_REGISTER_PWR_MGMT_1   =  0x6B;
const uint8_t MPU6050_REGISTER_PWR_MGMT_2   =  0x6C;
const uint8_t MPU6050_REGISTER_CONFIG       =  0x1A;
const uint8_t MPU6050_REGISTER_GYRO_CONFIG  =  0x1B;
const uint8_t MPU6050_REGISTER_ACCEL_CONFIG =  0x1C;
const uint8_t MPU6050_REGISTER_FIFO_EN      =  0x23;
const uint8_t MPU6050_REGISTER_INT_ENABLE   =  0x38;
const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;
const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x68;

int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;
unsigned lastMillis=0;

#define RAD_A_DEG = 57.295779

double Ax, Ay, Az, T, Gx, Gy, Gz, Acc_1, Acc_2, Pitch,Roll,Pitch_prev,Roll_prev;

double  girosc_ang_x_prev,girosc_ang_x,girosc_ang_y_prev,girosc_ang_y;
  long tiempo_prev, dt;

int PWMt;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Client.wifiConnection(WIFISSID, PASSWORD);
  Wire.begin(sda, scl);
  MPU6050_Init();
  pinMode(Pin_led, OUTPUT);
  digitalWrite(Pin_led, LOW);

}

void loop() {
  

  Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
  if(millis()-lastMillis>1000){
  //divide each with their sensitivity scale factor
  Ax = (double)AccelX/AccelScaleFactor;
  Ay = (double)AccelY/AccelScaleFactor;
  Az = (double)AccelZ/AccelScaleFactor;
  T = (double)Temperature/340+36.53; //temperature formula
  Gx = (double)GyroX/GyroScaleFactor;
  Gy = (double)GyroY/GyroScaleFactor;
  Gz = (double)GyroZ/GyroScaleFactor;

  Acc_1 = atan(-1*(Ax)/sqrt(pow((Ay),2) + pow((Az),2)))*RAD_TO_DEG;
  Acc_2 = atan((Ay)/sqrt(pow((Ax),2) + pow((Az),2)))*RAD_TO_DEG;

  dt = (millis() - tiempo_prev)/1000.0;
  tiempo_prev = millis();

  Pitch = 0.98*(Pitch_prev+Gx*dt) + 0.02*Acc_1;
  Roll = 0.98*(Roll_prev+Gy*dt) + 0.02*Acc_2;
  Pitch_prev = Pitch;
  Roll_prev = Roll;

 /*
    Serial.print("Ax: "); Serial.print(Ax);
  Serial.print(" Ay: "); Serial.print(Ay);
  Serial.print(" Az: "); Serial.print(Az);
  Serial.print(" T: "); Serial.print(T);
  Serial.print(" Gx: "); Serial.print(Gx);
  Serial.print(" Gy: "); Serial.print(Gy);
  Serial.print(" Gz: "); Serial.println(Gz);
  lastMillis = millis();
*/
 float Valor_Led = Client.getValueWithDevice(DEVICE, VARIABLE);
    
    if (Valor_Led != ERROR_VALUE){
      Serial.print(F(">>>>>>>>> VALOR OBTENIDO POR EL LED: "));
      Serial.println(Valor_Led);
      
      if (Valor_Led==1){digitalWrite(Pin_led, HIGH);}
      else{digitalWrite(Pin_led, LOW);}
      
    }else{
      Serial.println(F("Error getting Valor_Led"));
    }
  //ADELANTE

   if (Valor_Led==1){
  if(Acc_1>=3 && Acc_1<=90){
    PWMcar = round(map(Acc_1, 3, 90,0, 127));
    mySerial.write(PWMcar);
    Serial.println(PWMcar);  
    }
    /*
  if(Acc_2>15 && Acc_2<=30){
    mySerial.write(200);
    Serial.println('b');
    PWMcar = 100;
    
    }
  if(Acc_2>30 && Acc_2<=45){
    mySerial.write('c');
    Serial.println('c');
    PWMcar = 120;
    
    }
  if(Acc_2>45 && Acc_2<=60){
    mySerial.write(200);
    Serial.println('d');
    PWMcar = 150;
    
    }
  if(Acc_2>60 && Acc_2<=80){
    mySerial.write(200);
    Serial.println('e');
    PWMcar = 190;
    
    }
   if(Acc_2>80 && Acc_2<=90){
    mySerial.write(200);
    Serial.println('f');
    PWMcar = 210;
    
    }*/
    
  //ATRAS
  if(Acc_1<=-3 && Acc_1>=-90){
    PWMcar = round(map((Acc_1), -3, -90, 128, 255));
    mySerial.write(PWMcar);
    Serial.println(PWMcar); 
    
    }
    /*
  if(Acc_2<-15 && Acc_2>=-30){
    mySerial.write(200);
    Serial.println('2');
    PWMcar = 100;
    
    }
  if(Acc_2<-30 && Acc_2>=-45){
    mySerial.write(200);
    Serial.println('3');
    PWMcar = 120;
    
    }
  if(Acc_2<-45 && Acc_2>=-60){
    mySerial.write('4');
    Serial.println('4');
    PWMcar = 150;
    
    }
  if(Acc_2<-60 && Acc_2>=-80){
    mySerial.write('5');
    Serial.println('5');
    PWMcar = 190;
   
    }
   if(Acc_2<-80 && Acc_2>=-90){
    mySerial.write('6');
    Serial.println('6');
    PWMcar = 210;
    
    }*/

  
  //DERECHA
  if(Acc_2>=3 && Acc_2<=90){
    mySerial.write('2');
    Serial.println('2');
    PWMcar = 80;
    }
  //IZQUIERDA
  if(Acc_2<-3 && Acc_2>-90){
    mySerial.write('1');
    Serial.println('1');
    PWMcar = 80;
    }
  //PARE
  if(Acc_2>=-3 && Acc_2<3  && Acc_1>-3 && Acc_1<3){
    mySerial.write('0');
    Serial.println('0');
    PWMcar = 0;
    }
   
  
   
    
  Client.add("pwm",PWMcar);
  Client.add("ix", Acc_1); //Insert your variable Labels and the value to be sent PITCH
  Client.add("iy", Acc_2); //Insert your variable Labels and the value to be sent ROLL

  Client.sendAll(true);
   }
 
  }
  
} 




void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}

// read all 14 register
void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, (uint8_t)14);
  AccelX = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelY = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelZ = (((int16_t)Wire.read()<<8) | Wire.read());
  Temperature = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroX = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroY = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroZ = (((int16_t)Wire.read()<<8) | Wire.read());
}

//configure MPU6050
void MPU6050_Init(){
  delay(150);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_CONFIG, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_GYRO_CONFIG, 0x00);//set +/-250 degree/second full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);
}
