/**
 * This example tests the callback functionality of lwgps_process()
 * when the LWGPS_CFG_STATUS flag is set.
 */
#include <string.h>
#include <stdio.h>
#include "lwgps/lwgps.h"

#if !LWGPS_CFG_STATUS
#error "this test must be compiled with -DLWGPS_CFG_STATUS=1"
#endif /* !LWGPS_CFG_STATUS */

/* GPS handle */
lwgps_t hgps;

/**
 * \brief           Dummy data from GPS receiver
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

const lwgps_statement_t expected[] = {
    STAT_RMC,
    STAT_UNKNOWN,
    STAT_GGA,
    STAT_GSA,
    STAT_GSV,
    STAT_GSV,
    STAT_UNKNOWN,
    STAT_UNKNOWN,
    STAT_UNKNOWN,
    STAT_CHECKSUM_FAIL,
    STAT_UNKNOWN,
    STAT_UNKNOWN,
    STAT_RMC,
    STAT_UNKNOWN
};

static int err_cnt;

void
callback(lwgps_statement_t res) {
    static int i;

    if (res != expected[i]) {
        printf("failed i %d, expected res %d but received %d\n",
               i, expected[i], res);
        ++err_cnt;
    }

    ++i;
}

int
main() {
    /* Init GPS */
    lwgps_init(&hgps);

    /* Process all input data */
    lwgps_process(&hgps, gps_rx_data, strlen(gps_rx_data), callback);

    return err_cnt;
}
