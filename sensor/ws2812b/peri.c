#include <avr/io.h>
#include "peri.h"

void init_peri()
{
    DDRC |= ((1 << PD5) | (1 << PD6));
    // DDRC &= ~(0 << PC4);

    //enable pull up register on pc3
    // PORTC |= (1<< PC3);
}

void set_led_value(uint8_t value)
{
    PORTC &= ~(0b111);
    PORTC |= (value & 0b111);
}
