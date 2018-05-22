#include <Servo.h>
Servo servoMain; 
int analogPinX = 0;
int analogPinY = 1;
float val = 0;

void setup() {
servoMain.attach(2);
Serial.begin(9600);

}

void loop() {
  servoMain.write(0);
  delay(500);
  servoMain.write(180);
  delay(500);
  Serial.println(String(analogRead(analogPinX))+" , "+String(analogRead(analogPinY)) +","+String(val));
  delay(10);
}
 