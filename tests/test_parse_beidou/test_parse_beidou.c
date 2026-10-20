/**
 * \file            test_parse_beidou.c
 * \brief           Test BeiDou constellation NMEA sentence parsing
 * \version         0.1
 * \date            2026-05-11
 */
#include <stdio.h>
#include <string.h>
#include "lwgps/lwgps.h"
#include "test.h"

/**
 * \brief           BeiDou NMEA data from GPS receiver
 *                  BeiDou uses $BD prefix instead of $GP/$GN
 */
const char beidou_rx_data[] = ""
                               "$BDRMC,092750.000,A,5321.6802,N,00630.3372,W,0.02,31.66,280511,,,A*52\r\n"
                               "$BDGGA,092751.000,5321.6802,N,00630.3372,W,1,08,1.03,61.7,M,55.2,M,,*56\r\n"
                               "$BDGSA,A,3,01,02,03,04,05,06,07,08,,,,,1.6,1.0,1.2*2E\r\n"
                               "$BDGSV,2,1,08,01,43,088,38,02,42,145,00,03,11,291,00,04,60,043,35*60\r\n"
                               "$BDGSV,2,2,08,05,02,145,00,06,46,303,47,07,16,178,32,08,18,231,43*69\r\n"
                               "";

/**
 * \brief           Run the test of BeiDou input data
 */
int
test_run(void) {
    lwgps_t hgps;
    lwgps_init(&hgps);

    /* Process all BeiDou input data */
    lwgps_process(&hgps, beidou_rx_data, strlen(beidou_rx_data));

    /* Verify BeiDou data was parsed correctly */
    RUN_TEST(!INT_IS_EQUAL(hgps.is_valid, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.fix, 1));
    RUN_TEST(INT_IS_EQUAL(hgps.fix_mode, 3));
    RUN_TEST(FLT_IS_EQUAL(hgps.latitude, 53.3613366666));
    RUN_TEST(FLT_IS_EQUAL(hgps.longitude, -6.5056200000));
    RUN_TEST(FLT_IS_EQUAL(hgps.altitude, 61.7000000000));
    RUN_TEST(FLT_IS_EQUAL(hgps.course, 31.6600000000));
    RUN_TEST(FLT_IS_EQUAL(hgps.speed, 0.0200000000));
    RUN_TEST(FLT_IS_EQUAL(hgps.geo_sep, 55.2000000000));

    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view, 8));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_use, 8));

    RUN_TEST(INT_IS_EQUAL(hgps.time_valid, 1));
    RUN_TEST(INT_IS_EQUAL(hgps.date_valid, 1));
    RUN_TEST(INT_IS_EQUAL(hgps.date, 28));
    RUN_TEST(INT_IS_EQUAL(hgps.month, 5));
    RUN_TEST(INT_IS_EQUAL(hgps.year, 11));
    RUN_TEST(INT_IS_EQUAL(hgps.hours, 9));
    RUN_TEST(INT_IS_EQUAL(hgps.minutes, 27));
    RUN_TEST(INT_IS_EQUAL(hgps.seconds, 50));

    return 0;
}
