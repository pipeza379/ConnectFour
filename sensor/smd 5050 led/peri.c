#include <avr/io.h>
#include "peri.h"

#define RED 0
#define GREEN 1
#define BLUE 2

void init_peri()
{
    //output
    DDRC |= (1 << PC2) | (1 << PC1) | (1 << PC0);
}

void set_led(char color)
{
    if (color = 'R')
        PORTC |= (0b001);
    else if (color = 'G')
        PORTC |= (0b010);
    else if (color = 'B')
        PORTC |= (0b100);
    else if (color = '0')
        PORTC |= (0b000);
}

void set_led_value(uint8_t value)
{
    PORTC &= ~(0b111);
    PORTC |= (value & 0b111);
}
