#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include "peri.h"

int main()
{
    uint16_t light;
    init_peri();
    while (1)
    {
        light = get_light(PC4);
        if (light > 0)
            set_led_value(0b001);
        else if (light < 700)
            set_led_value(0b010);
        else
            set_led_value(0b100);
        send_data(light);
    }
}