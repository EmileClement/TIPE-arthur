
void setup() {
Serial.begin(9600);
}

void loop() {
 Serial.println(String(analogRead(analogPinX))+" , "+String(analogRead(analogPinY)));
 delay(10);

}
