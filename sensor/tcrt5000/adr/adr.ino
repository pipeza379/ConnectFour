void setup()
{
  pinMode(PIN_PB0, OUTPUT); //LIGHT3
  pinMode(PIN_PB2, OUTPUT); //LIGHT4
  pinMode(PIN_PB4, OUTPUT); //LIGHT5
  pinMode(PIN_PC0, INPUT); //LIGHT3
  pinMode(PIN_PC2, INPUT); //LIGHT4
  pinMode(PIN_PC4, INPUT); //LIGHT5
  pinMode(PIN_PD3, OUTPUT);

  digitalWrite(PIN_PB0,LOW);
  digitalWrite(PIN_PB2,LOW);
  digitalWrite(PIN_PB4,LOW);
  digitalWrite(PIN_PD3,HIGH);
}
int sent;

void setZero(){
  digitalWrite(PIN_PB0,LOW);
  digitalWrite(PIN_PB2,LOW);
  digitalWrite(PIN_PB4,LOW);
  digitalWrite(PIN_PD3,HIGH);
}

void getLight(int pin){
    setZero();
    if (analogRead(pin) <850){
      digitalWrite(sent,HIGH);
      digitalWrite(PIN_PD3,LOW);
    }
}

void loop()
{
  
  sent = PIN_PB0;
  getLight(PIN_PC0);
  delay(100);
  
  sent = PIN_PB2;
  getLight(PIN_PC2); 
  delay(100);
  
  sent = PIN_PB4;
  getLight(PIN_PC4);
  delay(100);

}
