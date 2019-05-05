#include <avr/io.h>
#include <avr/interrupt.h> /* for sei() */
#include <util/delay.h>    /* for _delay_ms() */
#include <avr/pgmspace.h>  /* required by usbdrv.h */

#include "peri.h"
#include "usbdrv.h"

#define RQ_SET_LED 0
#define RQ_GET_RESET 1
#define RQ_GET_SWITCH 2
#define RQ_GET_LIGHT 3
#define RQ_GET_DATA 4

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */
usbMsgLen_t usbFunctionSetup(uint8_t data[8])
{
    usbRequest_t *rq = (void *)data;

    /* declared as static so they stay valid when usbFunctionSetup returns */
    static uint8_t switch_reset;
    static uint8_t switch_state;
    static uint16_t light;
    static uint8_t rec_data;

    if (rq->bRequest == RQ_SET_LED)
    {
        uint8_t led_state = rq->wValue.bytes[0];
        uint8_t led_no = rq->wIndex.bytes[0];
        set_led(led_no, led_state);
        return 0;
    }

    else if (rq->bRequest == RQ_GET_RESET)
    {
        switch_reset = SWITCH_PRESSED(PIND, 1);
        usbMsgPtr = &switch_reset;
        return 1;
    }
    else if (rq->bRequest == RQ_GET_SWITCH)
    {
        switch (rq->wIndex.bytes[0])
        {
        case 0:
            switch_state = SWITCH_PRESSED(PINB, 0);
            break;
        case 1:
            switch_state = SWITCH_PRESSED(PINB, 1);
            break;
        case 2:
            switch_state = SWITCH_PRESSED(PINB, 2);
            break;
        case 3:
            switch_state = SWITCH_PRESSED(PINB, 3);
            break;
        case 4:
            switch_state = SWITCH_PRESSED(PINB, 4);
            break;
        case 5:
            switch_state = SWITCH_PRESSED(PINB, 5);
            break;
        }
        usbMsgPtr = &switch_state;
        return 1;
    }
    else if (rq->bRequest == RQ_GET_LIGHT)
    {
        uint8_t index = rq->wIndex.bytes[0];
        if (index == 0)
            index = PC0;
        else if (index == 2)
            index = PC2;
        else if (index == 4)
            index = PC4;
        light = read_adc(index);
        if(light <850)
            light = 1;
        usbMsgPtr = &light;

        return 1;
    }

    else if (rq->bRequest == RQ_GET_DATA)
    {
        uint8_t index = rq->wIndex.bytes[0];
        // if(PB1==1)
        if (index == 0)
            index = PD0;
        else if (index == 1)
            index = PD1;
        else if (index == 5)
            index = PD5;
        rec_data = (PIND & (1 << index));
        usbMsgPtr = &rec_data;

        return 1;
    }

    /* default for not implemented requests: return no data back to host */
    return 0;
}

/* ------------------------------------------------------------------------- */
int main(void)
{
    init_peri();

    usbInit();

    /* enforce re-enumeration, do this while interrupts are disabled! */
    usbDeviceDisconnect();
    _delay_ms(300);
    usbDeviceConnect();

    /* enable global interrupts */
    sei();

    /* main event loop */
    for (;;)
    {
        usbPoll();
    }

    return 0;
}

/* ------------------------------------------------------------------------- */
