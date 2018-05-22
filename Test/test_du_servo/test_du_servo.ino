#include <Servo.h>
Servo servoMain; 
int analogPinX = 0;
int analogPinY = 1;
float val = 0;
int T = 1000;

void setup() {
servoMain.attach(2);
Serial.begin(9600);

}

void loop() {
  servoMain.write(90);
  delay(T);
  servoMain.write(90);
  delay(T);
  Serial.println(String(analogRead(analogPinX))+" , "+String(analogRead(analogPinY)) +","+String(val));
  delay(10);
}
 
