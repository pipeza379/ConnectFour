#include <avr/io.h>
#include <avr/interrupt.h> /* for sei() */
#include <util/delay.h>    /* for _delay_ms() */
#include <avr/pgmspace.h>  /* required by usbdrv.h */

#include "usbdrv.h"
// #include <stddef.h> // for NULL

#include "peri.h"
// #include "timer.h"

#define RQ_SET_LED 0
#define RQ_GET_LIGHT 3
#define RQ_SEND_DATA 2

// void *checkLight_state;
// void *sendData_state;

// uint16_t light;

// void checkLight()
// {
//     static uint32_t ts;

//     if (checkLight_state != NULL)
//         goto *checkLight_state;

//     for (;;)
//     {
//         light = get_light(PC4);

//         if (light < 500)
//             set_led_value(0b001);
//         else if (light < 700)
//             set_led_value(0b010);
//         else
//             set_led_value(0b100);
//         // สามบรรทัดนี้ทำงานเทียบเท่ากับการใช้ _delay_ms(1000) โดยไม่หยุดรอ
//         ts = timer_millis();
//     ON:
//         checkLight_state = &&ON; // จดจำบรรทัดปัจจุบัน
//         if (timer_millis() - ts < 1000)
//             return;

//         set_led_value(0b000);

//         // สามบรรทัดนี้ทำงานเทียบเท่ากับการใช้ _delay_ms(500) โดยไม่หยุดรอ
//         ts = timer_millis();
//     OFF:
//         checkLight_state = &&OFF; // จดจำบรรทัดปัจจุบัน
//         if (timer_millis() - ts < 500)
//             return;
//     }
// }

// void sendData()
// {
//     static uint32_t ts;

//     if (sendData_state != NULL)
//         goto *sendData_state;

//     for (;;)
//     {
//         send_data(0);
//         ts = timer_millis();
//     ON:
//         sendData_state = &&ON; // จดจำบรรทัดปัจจุบัน
//         if (timer_millis() - ts < 1000)
//             return;

//         send_data(1);
//         ts = timer_millis();
//     OFF:
//         sendData_state = &&OFF; // จดจำบรรทัดปัจจุบัน
//         if (timer_millis() - ts < 500)
//             return;
//     }
// }
/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */
usbMsgLen_t usbFunctionSetup(uint8_t data[8])
{
    usbRequest_t *rq = (void *)data;

    /* declared as static so they stay valid when usbFunctionSetup returns */
    static uint16_t light;

    if (rq->bRequest == RQ_SET_LED)
    {
        uint8_t led_state = rq->wValue.bytes[0];
        uint8_t led_no = rq->wIndex.bytes[0];
        set_led(led_no, led_state);
        return 0;
    }
    else if (rq->bRequest == RQ_GET_LIGHT)
    {
        light = get_light(PC4);
        usbMsgPtr = &light;

        return 2;
    }
    else if (rq->bRequest == RQ_SEND_DATA)
    {
        uint16_t data = rq->wValue.bytes[0];
        send_data(data);

        return 0;
    }
    /* default for not implemented requests: return no data back to host */
    return 0;
}

/* ------------------------------------------------------------------------- */
int main(void)
{
    uint16_t light;
    init_peri();

    // usbInit();

    /* enforce re-enumeration, do this while interrupts are disabled! */
    // usbDeviceDisconnect();
    // _delay_ms(300);
    // usbDeviceConnect();

    /* enable global interrupts */
    // init_peripheral();
    // timer_init();
    sei();

    /* main event loop */

    for (;;)
    {
        light = get_light(PC4);
        if (light < 500)
            set_led_value(0b001);
        else if (light < 700)
            set_led_value(0b010);
        else
            set_led_value(0b100);
        // checkLight_state = NULL;
        // sendData_state = NULL;
        // checkLight();
        // sendData();
        // usbPoll();
        send_data(0b001);
    }

    return 0;
}

/* ------------------------------------------------------------------------- */
