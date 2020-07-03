#include "lwgps/lwgps.h"
#include "lwrb/lwrb.h"
#include <string.h>

/* GPS handle */
lwgps_t hgps;

/* GPS buffer */
lwrb_t hgps_buff;
uint8_t hgps_buff_data[12];

/**
 * \brief           Dummy data from GPS receiver
 * \note            This data are used to fake UART receive event on microcontroller
 */
const char
gps_rx_data[] = ""
                "$GPRMC,183729,A,3907.356,N,12102.482,W,000.0,360.0,080301,015.5,E*6F\r\n"
                "$GPRMB,A,,,,,,,,,,,,V*71\r\n"
                "$GPGGA,183730,3907.356,N,12102.482,W,1,05,1.6,646.4,M,-24.1,M,,*75\r\n"
                "$GPGSA,A,3,02,,,07,,09,24,26,,,,,1.6,1.6,1.0*3D\r\n"
                "$GPGSV,2,1,08,02,43,088,38,04,42,145,00,05,11,291,00,07,60,043,35*71\r\n"
                "$GPGSV,2,2,08,08,02,145,00,09,46,303,47,24,16,178,32,26,18,231,43*77\r\n"
                "$PGRME,22.0,M,52.9,M,51.0,M*14\r\n"
                "$GPGLL,3907.360,N,12102.481,W,183730,A*33\r\n"
                "$PGRMZ,2062,f,3*2D\r\n"
                "$PGRMM,WGS84*06\r\n"
                "$GPBOD,,T,,M,,*47\r\n"
                "$GPRTE,1,1,c,0*07\r\n"
                "$GPRMC,183731,A,3907.482,N,12102.436,W,000.0,360.0,080301,015.5,E*67\r\n"
                "$GPRMB,A,,,,,,,,,,,,V*71\r\n";
static size_t write_ptr;
static void uart_irqhandler(void);

int
main() {
    uint8_t rx;

    /* Init GPS */
    lwgps_init(&hgps);

    /* Create buffer for received data */
    lwrb_init(&hgps_buff, hgps_buff_data, sizeof(hgps_buff_data));

    while (1) {
        /* Add new character to buffer */
        /* Fake UART interrupt handler on host microcontroller */
        uart_irqhandler();

        /* Process all input data */
        /* Read from buffer byte-by-byte and call processing function */
        if (lwrb_get_full(&hgps_buff)) {        /* Check if anything in buffer now */
            while (lwrb_read(&hgps_buff, &rx, 1) == 1) {
                lwgps_process(&hgps, &rx, 1);   /* Process byte-by-byte */
            }
        } else {
            /* Print all data after successful processing */
            printf("Latitude: %f degrees\r\n", hgps.latitude);
            printf("Longitude: %f degrees\r\n", hgps.longitude);
            printf("Altitude: %f meters\r\n", hgps.altitude);
            break;
        }
    }

    return 0;
}

/**
 * \brief           Interrupt handler routing for UART received character
 * \note            This is not real MCU, it is software method, called from main
 */
static void
uart_irqhandler(void) {
    /* Make interrupt handler as fast as possible */
    /* Only write to received buffer and process later */
    if (write_ptr < strlen(gps_rx_data)) {
        /* Write to buffer only */
        lwrb_write(&hgps_buff, &gps_rx_data[write_ptr], 1);
        ++write_ptr;
    }
}
