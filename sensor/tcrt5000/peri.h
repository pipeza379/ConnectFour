#include <avr/io.h>

void init_peri();
void set_led_value(uint8_t value);
uint16_t read_adc(uint8_t channel);
void send_data(uint16_t data);
