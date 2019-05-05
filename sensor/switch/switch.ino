#include <Bounce2.h>

#define BUTTON_PIN PIN_PC2
#define LED_PIN PIN_PC0

Bounce debouncer = Bounce();

void setup() {
   debouncer.attach(BUTTON_PIN, INPUT_PULLUP);
   debouncer.interval(25); 
   pinMode(LED_PIN, OUTPUT);
   digitalWrite(LED_PIN, LOW);
}

void loop() {
   debouncer.update();   
   if ( debouncer.fell() ) { 
      digitalWrite(LED_PIN, HIGH);
      delay(100);
   }
   else
      digitalWrite(LED_PIN, LOW);
   
}

