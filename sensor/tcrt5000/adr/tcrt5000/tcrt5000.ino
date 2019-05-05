void setup() {
  pinMode(PIN_PC3, INPUT);
  pinMode(PIN_PD3, OUTPUT);
}

void loop() {
  digitalWrite(PIN_PD3,HIGH);
  if(analogRead(PIN_PC3)<850){
    digitalWrite(PIN_PD3,LOW);
    delay(100);
  }}
