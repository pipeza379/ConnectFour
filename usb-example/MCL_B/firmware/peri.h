#include <avr/io.h>

void init_peri();
void set_led(uint8_t pin, uint8_t state);
void send_data(uint16_t data);
void set_led_value(uint8_t value);
uint16_t get_light(uint8_t channel);
