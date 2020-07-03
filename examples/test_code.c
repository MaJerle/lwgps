/*
 * This example uses direct processing function,
 * to process dummy NMEA data from GPS receiver
 */
#include <string.h>
#include <stdio.h>
#include "lwgps/lwgps.h"
#include "test_common.h"

/* GPS handle */
lwgps_t hgps;

/**
 * \brief           Dummy data from GPS receiver
 */
const char
gps_rx_data[] = ""
                "$GPRMC,183729,A,3907.356,N,12102.482,W,000.0,360.0,080301,015.5,E*6F\r\n"
                "$GPGGA,183730,3907.356,N,12102.482,W,1,05,1.6,646.4,M,-24.1,M,,*75\r\n"
                "$GPGSA,A,3,02,,,07,,09,24,26,,,,,1.6,1.6,1.0*3D\r\n"
                "$GPGSV,2,1,08,02,43,088,38,04,42,145,00,05,11,291,00,07,60,043,35*71\r\n"
                "$GPGSV,2,2,08,08,02,145,00,09,46,303,47,24,16,178,32,26,18,231,43*77\r\n"
                "";

/**
 * \brief           Run the test of raw input data
 */
void
run_tests() {
    lwgps_init(&hgps);                          /* Init GPS */

    /* Process all input data */
    lwgps_process(&hgps, gps_rx_data, strlen(gps_rx_data));

    /* Run the test */
    RUN_TEST(!INT_IS_EQUAL(hgps.is_valid, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.fix, 1));
    RUN_TEST(INT_IS_EQUAL(hgps.fix_mode, 3));
    RUN_TEST(FLT_IS_EQUAL(hgps.latitude, 39.1226000000));
    RUN_TEST(FLT_IS_EQUAL(hgps.longitude, -121.0413666666));
    RUN_TEST(FLT_IS_EQUAL(hgps.altitude, 646.4000000000));
    RUN_TEST(FLT_IS_EQUAL(hgps.course, 360.0000000000));
    RUN_TEST(INT_IS_EQUAL(hgps.dop_p, 1.6000000000));
    RUN_TEST(INT_IS_EQUAL(hgps.dop_h, 1.6000000000));
    RUN_TEST(INT_IS_EQUAL(hgps.dop_v, 1.0000000000));
    RUN_TEST(FLT_IS_EQUAL(hgps.speed, 0.0000000000));
    RUN_TEST(FLT_IS_EQUAL(hgps.geo_sep, -24.100000000));
    RUN_TEST(FLT_IS_EQUAL(hgps.variation, 15.500000000));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view, 8));

    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_use, 5));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[0], 2));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[1], 0));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[2], 0));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[3], 7));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[4], 0));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[5], 9));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[6], 24));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[7], 26));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[8], 0));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[9], 0));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[10], 0));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[11], 0));

    RUN_TEST(INT_IS_EQUAL(hgps.date, 8));
    RUN_TEST(INT_IS_EQUAL(hgps.month, 3));
    RUN_TEST(INT_IS_EQUAL(hgps.year, 1));
    RUN_TEST(INT_IS_EQUAL(hgps.hours, 18));
    RUN_TEST(INT_IS_EQUAL(hgps.minutes, 37));
    RUN_TEST(INT_IS_EQUAL(hgps.seconds, 30));
}
