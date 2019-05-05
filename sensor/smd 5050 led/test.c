#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include "peri.h"

int main()
{
    init_peri();
    uint16_t count = 0;
    while (1)
    {
        count++;
        set_led_value(count);
        _delay_ms(150);
        // else if (light < 700)
        // set_led_value(count);
        // _delay_ms(150);
        // else
        // set_led_value(count);
        // _delay_ms(150);
    }
}