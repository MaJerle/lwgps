/**
 * \file            test_parse_gps_beidou.c
 * \brief           Test GPS + BeiDou (multi-GNSS) NMEA output of ATGM336H receiver, see issue #49
 * \version         0.1
 * \date            2026-10-22
 */
#include <stdio.h>
#include <string.h>
#include "lwgps/lwgps.h"
#include "test.h"

/**
 * \brief           One NMEA epoch from GPS + BeiDou receiver
 *                  8 satellites in use (7 GPS + 1 BeiDou), 14 in view (10 GPS + 4 BeiDou)
 */
const char gps_beidou_rx_data[] = ""
                                  "$GNGGA,230924.000,3746.55402,N,12223.73182,W,1,08,1.0,-3.6,M,0.0,M,,*40\r\n"
                                  "$GNGLL,3746.55402,N,12223.73182,W,230924.000,A,A*54\r\n"
                                  "$GPGSA,A,3,05,07,08,09,17,21,22,,,,,,2.4,1.0,2.2*33\r\n"
                                  "$BDGSA,A,3,37,,,,,,,,,,,,2.4,1.0,2.2*20\r\n"
                                  "$GPGSV,3,1,10,05,16,282,26,07,55,078,16,08,29,052,20,09,23,159,19*70\r\n"
                                  "$GPGSV,3,2,10,14,,,13,17,25,178,29,20,27,315,16,21,19,248,18*41\r\n"
                                  "$GPGSV,3,3,10,22,47,244,16,30,71,352,09*76\r\n"
                                  "$BDGSV,1,1,04,20,,,20,25,,,19,37,43,281,24,38,,,31*54\r\n"
                                  "$GNRMC,230924.000,A,3746.55402,N,12223.73182,W,0.35,238.88,211026,,,A*6A\r\n"
                                  "$GNVTG,238.88,T,,M,0.35,N,0.65,K,A*2F\r\n"
                                  "$GNZDA,230924.000,21,10,2026,00,00*42\r\n"
                                  "$GPTXT,01,01,01,ANTENNA OK*35\r\n"
                                  "";

/**
 * \brief           Run the test of GPS + BeiDou input data
 */
int
test_run(void) {
    lwgps_t hgps;
    lwgps_init(&hgps);

    /* Process all input data */
    lwgps_process(&hgps, gps_beidou_rx_data, strlen(gps_beidou_rx_data));

    /* Position, fix and DOP */
    RUN_TEST(!INT_IS_EQUAL(hgps.is_valid, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.fix, 1));
    RUN_TEST(INT_IS_EQUAL(hgps.fix_mode, 3));
    RUN_TEST(FLT_IS_EQUAL(hgps.latitude, 37.7759003333));
    RUN_TEST(FLT_IS_EQUAL(hgps.longitude, -122.3955303333));
    RUN_TEST(FLT_IS_EQUAL(hgps.altitude, -3.6000000000));
    RUN_TEST(FLT_IS_EQUAL(hgps.geo_sep, 0.0000000000));
    RUN_TEST(FLT_IS_EQUAL(hgps.dop_p, 2.4000000000));
    RUN_TEST(FLT_IS_EQUAL(hgps.dop_h, 1.0000000000));
    RUN_TEST(FLT_IS_EQUAL(hgps.dop_v, 2.2000000000));
    RUN_TEST(FLT_IS_EQUAL(hgps.speed, 0.3500000000));
    RUN_TEST(FLT_IS_EQUAL(hgps.course, 238.8800000000));
    RUN_TEST(FLT_IS_EQUAL(hgps.variation, 0.0000000000));

    /* Satellites in use, from GPGSA and BDGSA */
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_use, 8));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[0], 5));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[1], 7));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[2], 8));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[3], 9));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[4], 17));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[5], 21));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[6], 22));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[7], 37));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[8], 0));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[9], 0));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[10], 0));
    RUN_TEST(INT_IS_EQUAL(hgps.satellites_ids[11], 0));

    /* Satellites in view, from GPGSV and BDGSV series */
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view, 14));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[0].num, 5));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[0].elevation, 16));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[0].azimuth, 282));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[0].snr, 26));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[1].num, 7));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[1].elevation, 55));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[1].azimuth, 78));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[1].snr, 16));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[2].num, 8));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[2].elevation, 29));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[2].azimuth, 52));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[2].snr, 20));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[3].num, 9));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[3].elevation, 23));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[3].azimuth, 159));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[3].snr, 19));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[4].num, 14));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[4].elevation, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[4].azimuth, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[4].snr, 13));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[5].num, 17));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[5].elevation, 25));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[5].azimuth, 178));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[5].snr, 29));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[6].num, 20));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[6].elevation, 27));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[6].azimuth, 315));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[6].snr, 16));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[7].num, 21));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[7].elevation, 19));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[7].azimuth, 248));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[7].snr, 18));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[8].num, 22));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[8].elevation, 47));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[8].azimuth, 244));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[8].snr, 16));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[9].num, 30));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[9].elevation, 71));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[9].azimuth, 352));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[9].snr, 9));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[10].num, 20));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[10].elevation, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[10].azimuth, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[10].snr, 20));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[11].num, 25));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[11].elevation, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[11].azimuth, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[11].snr, 19));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[12].num, 37));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[12].elevation, 43));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[12].azimuth, 281));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[12].snr, 24));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[13].num, 38));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[13].elevation, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[13].azimuth, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[13].snr, 31));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[14].num, 0));

    /* Date and time */
    RUN_TEST(INT_IS_EQUAL(hgps.time_valid, 1));
    RUN_TEST(INT_IS_EQUAL(hgps.date_valid, 1));
    RUN_TEST(INT_IS_EQUAL(hgps.date, 21));
    RUN_TEST(INT_IS_EQUAL(hgps.month, 10));
    RUN_TEST(INT_IS_EQUAL(hgps.year, 26));
    RUN_TEST(INT_IS_EQUAL(hgps.hours, 23));
    RUN_TEST(INT_IS_EQUAL(hgps.minutes, 9));
    RUN_TEST(INT_IS_EQUAL(hgps.seconds, 24));

    return 0;
}
