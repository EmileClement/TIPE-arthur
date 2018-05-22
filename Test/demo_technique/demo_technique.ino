#include <Servo.h>
Servo servoMain; 
int analogPinX = 0;
int analogPinY = 1;
float val = 0;

void setup() {
servoMain.attach(2);
Serial.begin(9600)

}

void loop() {
  val = val-0.01*(analogRead(analogPinX)-325);
  if (val>180)
  {
    val = 180;
  }
  if( val <0){
    val = 0;
  }
  servoMain.write(val);
  Serial.println(String(analogRead(analogPinX))+" , "+String(analogRead(analogPinY)) +","+String(val));
  delay(5);
}
