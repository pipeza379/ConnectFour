#include <FastLED.h>

#define LED_PIN     PIN_PD5
#define NUM_LEDS    30
#define BRIGHTNESS  200
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100

CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

void setup() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}


void loop()
{
//    ChangePalettePeriodically();
    
    static uint8_t startIndex = 0;
//    startIndex = startIndex + 1; /* motion speed */

       uint8_t brightness = 255;
 
   for(uint8_t colorIndex = 0; colorIndex < 30; colorIndex++) {
        leds[colorIndex] = ColorFromPalette( RainbowColors_p, colorIndex*3+105, brightness, LINEARBLEND);
        brightness+=50;
//        leds[i+5] = ColorFromPalette( RainbowColors_p, colorIndex, brightness, LINEARBLEND);
//        leds[i+10] = ColorFromPalette( RainbowColors_p, colorIndex, brightness, LINEARBLEND);
//        leds[i+15] = ColorFromPalette( RainbowColors_p, colorIndex, brightness, LINEARBLEND);
//        leds[i+20] = ColorFromPalette( RainbowColors_p, colorIndex, brightness, LINEARBLEND);
//        leds[i+25] = ColorFromPalette( RainbowColors_p, colorIndex, brightness, LINEARBLEND);   
        FastLED.show();
        FastLED.delay(100);
//        colorIndex += 3;
    }

//    FillLEDsFromPaletteColors( startIndex);
    
//    FastLED.show();
    FastLED.delay(500);
//    for(int i=0;i<30;i++)
//      leds[i]=CRGB::Black;
//    FastLED.show();
    FastLED.clear();
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < 5; i++) {
        leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, brightness, LINEARBLEND);
        leds[i+5] = ColorFromPalette( RainbowColors_p, colorIndex, brightness, LINEARBLEND);
        leds[i+10] = ColorFromPalette( RainbowColors_p, colorIndex, brightness, LINEARBLEND);
        leds[i+15] = ColorFromPalette( RainbowColors_p, colorIndex, brightness, LINEARBLEND);
        leds[i+20] = ColorFromPalette( RainbowColors_p, colorIndex, brightness, LINEARBLEND);
        leds[i+25] = ColorFromPalette( RainbowColors_p, colorIndex, brightness, LINEARBLEND);
        
        colorIndex += 3;
    }
}

void ChangePalettePeriodically()
{
    uint8_t secondHand = 0;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
    }    }  
