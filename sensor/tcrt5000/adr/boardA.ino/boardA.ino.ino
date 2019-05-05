void setup() {
  pinMode(PIN_PD0, INPUT);
  pinMode(PIN_PD1, INPUT);
  pinMode(PIN_PD2, INPUT);
  pinMode(PIN_PD3, OUTPUT);
}
int ldr;
void loop() {
  if(digitalRead(PIN_PD1))
    digitalWrite(PIN_PD3,LOW);
  else
    digitalWrite(PIN_PD3,HIGH  delay(250);
  }
