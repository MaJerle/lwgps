/**
 * \file            test_parse_rmc.c
 * \brief           Test RMC-only parsing
 */

#include <stdio.h>
#include <string.h>
#include "lwgps/lwgps.h"
#include "test.h"

/**
 * \brief           Run the RMC-only parser test
 */
int
test_run(void) {
    static const char valid[] = "$GPRMC,092750.000,A,5321.6802,N,00630.3372,W,0.02,31.66,280511,,,A*43\r\n";
    static const char short_time[] = "$GPRMC,09,A,5321.6802,N,00630.3372,W,0.02,31.66,280511,,,A*5D\r\n";
    lwgps_t hgps;

    lwgps_init(&hgps);
    lwgps_process(&hgps, valid, strlen(valid));

    RUN_TEST(INT_IS_EQUAL(hgps.time_valid, 1));
    RUN_TEST(INT_IS_EQUAL(hgps.hours, 9));
    RUN_TEST(INT_IS_EQUAL(hgps.minutes, 27));
    RUN_TEST(INT_IS_EQUAL(hgps.seconds, 50));
    RUN_TEST(INT_IS_EQUAL(hgps.date_valid, 1));
    RUN_TEST(INT_IS_EQUAL(hgps.date, 28));
    RUN_TEST(INT_IS_EQUAL(hgps.month, 5));
    RUN_TEST(INT_IS_EQUAL(hgps.year, 11));

    lwgps_process(&hgps, short_time, strlen(short_time));

    RUN_TEST(INT_IS_EQUAL(hgps.time_valid, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.hours, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.minutes, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.seconds, 0));

    return 0;
}
