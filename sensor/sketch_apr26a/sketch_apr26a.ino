void setup()
{
  pinMode(PIN_PD3, OUTPUT);
}
 
void loop()
{
  digitalWrite(PIN_PD3, LOW);
  delay(250);
  digitalWrite(PIN_PD3, HIGH);
  delay(250);
}
