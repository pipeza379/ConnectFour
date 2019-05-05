#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include "./FastLED.h"

#define NUM_LEDS 30
#define PIN PD5
CRGB leds[NUM_LEDS];

int main()
{
    CRGB leds[NUM_LEDS];
    FastLED.addLeds<WS2812B, PIN>(leds, NUM_LEDS);
    uint16_t light;
    init_peri();
    while (1)
    {
        leds[0] = CRGB::White;
        FastLED.show();
        sleep(0.5);
        leds[0] = CRGB::Black;
        FastLED.show();
        sleep(0.5);
    }
}
