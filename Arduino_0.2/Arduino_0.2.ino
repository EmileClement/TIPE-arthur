#include<Wire.h>
#include <Servo.h>
Servo servoZ;
Servo servoY;
int s_pinZ = 2;
int s_pinY = 2;
int analogPinX = 0;
int analogPinY = 1;
int analogPinZ = 2;
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
int accer_Mode = 0, position_Mode = 1;
String Message;
float CX, CY;
float IntegrZ = 0 ;
float IntegrY = 0 ;
float  DerriveZ = 0 ;
float  DerriveY = 0 ;
float  LastErreurZ = 0 ;
float  LastErreurY = 0 ;
float  LastIntration = 0;
float PZ = 0.1;
float DZ = 0.01 ;
float IZ = 0.05;
float PY = 1 ;
float DY = 0 ;
float IY = 0;
float cibleZ = 0, cibleY = 0;
int Pas = 10;

void setup() {
  servoZ.attach ( s_pinZ );
  servoY.attach ( s_pinY );

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  
  Serial.begin(9600);
}

void Mesure_MPU() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

}

float ErreurZ() { //renvoi l'erreur selon Z
 return analogRead(analogPinX) - 325 - cibleZ;
}

float ErreurY() { // renvoi l'erreur selon Y
return analogRead(analogPinZ) - 325 - cibleY;
}

void envoie_mesure() {
Serial.print(String(analogRead(analogPinX)) + "," + String(analogRead(analogPinY)) + "," + String(analogRead(analogPinZ)));
}

void OrdreZ(float ordre) {
  servoZ.write(ordre);
}
void OrdreY(float ordre) {
  servoY.write( ordre);
}

void Communication() {  // gere la reception des messages depuis python
  if (Serial.available() > 0) {
    Message += Serial.read();
    if ( Message.endsWith(";")) {
      if (Message.startsWith("M")) {  // mode de fonctionement
//        accer_Mode = int( Message.substring(1));
//        if (Mode == 1) {
//          LastIntration = millis();
//        }
      }
      else if (Message.startsWith("P")) { // deffinition des parrametre d'asservissement

      }
      else if (Message.startsWith("C")) { // deffinition des consigne

      }
      else if (Message.startsWith("M")) { // demande de mesure
        envoie_mesure();
      }
      Message = "";
    }
  }
}
void accer_PID() {
  if (millis() - LastIntration<Pas){
  IntegrZ += ErreurZ() * (millis() - LastIntration);
  IntegrY += ErreurY() * (millis() - LastIntration);
  DerriveZ = (ErreurZ() - LastErreurZ) / (millis() - LastIntration);
  DerriveY = (ErreurY() - LastErreurY) / (millis() - LastIntration);
  LastIntration = millis();
  LastErreurZ = ErreurZ();
  LastErreurY = ErreurY();
  OrdreZ( PZ * ErreurZ() + IZ * IntegrZ + DZ * DerriveZ);
  OrdreY( PY * ErreurY() + IY * IntegrY + DY * DerriveY);
  LastIntration = millis();
  }
}

void loop() {
  Communication();
  if (accer_Mode == 1) {
    accer_PID();
  }
}
