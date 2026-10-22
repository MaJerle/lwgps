/**
 * \file            test_parse_standard.c
 * \author          Tilen MAJERLE <tilen@majerle.eu>
 * \brief           
 * \version         0.1
 * \date            2026-03-30
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include <stdio.h>
#include <string.h>
#include "lwgps/lwgps.h"
#include "test.h"

/**
 * \brief           Dummy data from GPS receiver
 */
const char gps_rx_data[] = ""
                           "$GPRMC,183729,A,3907.356,N,12102.482,W,000.0,360.0,080301,015.5,E*6F\r\n"
                           "$GPGGA,183730,3907.356,N,12102.482,W,1,05,1.6,646.4,M,-24.1,M,,*75\r\n"
                           "$GPGSA,A,3,02,,,07,,09,24,26,,,,,1.6,1.6,1.0*3D\r\n"
                           "$GPGSV,2,1,08,02,43,088,38,04,42,145,00,05,11,291,00,07,60,043,35*71\r\n"
                           "$GPGSV,2,2,08,08,02,145,00,09,46,303,47,24,16,178,32,26,18,231,43*77\r\n"
                           "$GPTXT,01,01,01,ANTENNA OK*35\r\n"
                           "";

/**
 * \brief           Run the test of raw input data
 */
int
test_run(void) {
    lwgps_t hgps;
    lwgps_init(&hgps);

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
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[1], 7));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[2], 9));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[3], 24));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[4], 26));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[5], 0));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[6], 0));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[7], 0));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[8], 0));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[9], 0));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[10], 0));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[11], 0));

    /* Test the satellites in view  */
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view, 8));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[0].num, 2));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[0].elevation, 43));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[0].azimuth, 88));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[0].snr, 38));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[1].num, 4));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[1].elevation, 42));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[1].azimuth, 145));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[1].snr, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[2].num, 5));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[2].elevation, 11));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[2].azimuth, 291));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[2].snr, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[3].num, 7));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[3].elevation, 60));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[3].azimuth, 43));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[3].snr, 35));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[4].num, 8));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[4].elevation, 2));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[4].azimuth, 145));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[4].snr, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[5].num, 9));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[5].elevation, 46));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[5].azimuth, 303));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[5].snr, 47));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[6].num, 24));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[6].elevation, 16));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[6].azimuth, 178));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[6].snr, 32));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[7].num, 26));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[7].elevation, 18));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[7].azimuth, 231));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[7].snr, 43));

    RUN_TEST(INT_IS_EQUAL(hgps.time_valid, 1));
    RUN_TEST(INT_IS_EQUAL(hgps.date_valid, 1));
    RUN_TEST(INT_IS_EQUAL(hgps.date, 8));
    RUN_TEST(INT_IS_EQUAL(hgps.month, 3));
    RUN_TEST(INT_IS_EQUAL(hgps.year, 1));
    RUN_TEST(INT_IS_EQUAL(hgps.hours, 18));
    RUN_TEST(INT_IS_EQUAL(hgps.minutes, 37));
    RUN_TEST(INT_IS_EQUAL(hgps.seconds, 30));

    return 0;
}
