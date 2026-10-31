/**
 * \file            test_parse_framing.c
 * \brief           Test statement framing: `$` start, `*` checksum and `\r`/`\n` end
 */

#include <stdio.h>
#include <string.h>
#include "lwgps/lwgps.h"
#include "test.h"

static lwgps_statement_t events[16];
static size_t events_cnt;

/**
 * \brief           Record every reported statement
 */
static void
prv_evt_fn(lwgps_statement_t res) {
    if (events_cnt < sizeof(events) / sizeof(events[0])) {
        events[events_cnt] = res;
    }
    ++events_cnt;
}

/**
 * \brief           Reset recorded events and process the string
 */
static void
prv_process(lwgps_t* hgps, const char* str) {
    events_cnt = 0;
    lwgps_process(hgps, str, strlen(str), prv_evt_fn);
}

/**
 * \brief           Run the framing test
 */
int
test_run(void) {
    lwgps_t hgps;

    lwgps_init(&hgps);

    /* Junk before the first `$`, including line endings and a star, must be ignored */
    prv_process(&hgps, "garbage\r\nmore*12\r\n\r\n");
    RUN_TEST(INT_IS_EQUAL(events_cnt, 0));

    /* Valid statement */
    prv_process(&hgps, "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76\r\n");
    RUN_TEST(INT_IS_EQUAL(events_cnt, 1));
    RUN_TEST(INT_IS_EQUAL(events[0], STAT_GGA));
    RUN_TEST(FLT_IS_EQUAL(hgps.altitude, 61.7));

    /* Repeated end of line must process the statement only once */
    prv_process(&hgps, "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76\r\r\n\n");
    RUN_TEST(INT_IS_EQUAL(events_cnt, 1));
    RUN_TEST(INT_IS_EQUAL(events[0], STAT_GGA));

    /* Repeated end of line on GSV must not accumulate the same series twice */
    prv_process(&hgps, "$GPGSV,1,1,02,10,63,137,17,07,61,098,15*79\r\r\n"
                       "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76\r\n");
    RUN_TEST(INT_IS_EQUAL(events_cnt, 2));
    RUN_TEST(INT_IS_EQUAL(events[0], STAT_GSV));
    RUN_TEST(INT_IS_EQUAL(events[1], STAT_GGA));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view, 2));

    /* `\n` only line ending */
    prv_process(&hgps, "$GPGGA,092751.000,5321.6802,N,00630.3372,W,1,8,1.03,99.9,M,55.2,M,,*7E\n");
    RUN_TEST(INT_IS_EQUAL(events_cnt, 1));
    RUN_TEST(INT_IS_EQUAL(events[0], STAT_GGA));
    RUN_TEST(FLT_IS_EQUAL(hgps.altitude, 99.9));

    /* Lowercase hex checksum */
    prv_process(&hgps, "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76\r\n");
    prv_process(&hgps, "$GPGGA,092751.000,5321.6802,N,00630.3372,W,1,8,1.03,99.9,M,55.2,M,,*7e\r\n");
    RUN_TEST(INT_IS_EQUAL(events_cnt, 1));
    RUN_TEST(INT_IS_EQUAL(events[0], STAT_GGA));
    RUN_TEST(FLT_IS_EQUAL(hgps.altitude, 99.9));

    /* Missing or malformed checksum fails and leaves user data untouched */
    prv_process(&hgps, "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,\r\n");
    prv_process(&hgps, "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,7\r\n");
    RUN_TEST(INT_IS_EQUAL(events_cnt, 1));
    RUN_TEST(INT_IS_EQUAL(events[0], STAT_CHECKSUM_FAIL));
    prv_process(&hgps, "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*\r\n"
                       "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*7\r\n"
                       "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*767\r\n"
                       "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*7G\r\n"
                       "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76,\r\n"
                       "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*7*6\r\n"
                       "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*77\r\n");
    RUN_TEST(INT_IS_EQUAL(events_cnt, 7));
    for (size_t i = 0; i < events_cnt; ++i) {
        RUN_TEST(INT_IS_EQUAL(events[i], STAT_CHECKSUM_FAIL));
    }
    RUN_TEST(FLT_IS_EQUAL(hgps.altitude, 99.9));

    /* Truncated statement, restarted by `$`, is silently dropped */
    prv_process(&hgps, "$GPGGA,092750.000,5321.68"
                       "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76\r\n");
    RUN_TEST(INT_IS_EQUAL(events_cnt, 1));
    RUN_TEST(INT_IS_EQUAL(events[0], STAT_GGA));
    RUN_TEST(FLT_IS_EQUAL(hgps.altitude, 61.7));

    /* Statement split over several process calls */
    events_cnt = 0;
    lwgps_process(&hgps, "$GPGGA,092751.000,5321.6802,N,0063", strlen("$GPGGA,092751.000,5321.6802,N,0063"),
                  prv_evt_fn);
    lwgps_process(&hgps, "0.3372,W,1,8,1.03,99.9,M,55.2,M,,*", strlen("0.3372,W,1,8,1.03,99.9,M,55.2,M,,*"),
                  prv_evt_fn);
    lwgps_process(&hgps, "7E\r", strlen("7E\r"), prv_evt_fn);
    lwgps_process(&hgps, "\n", strlen("\n"), prv_evt_fn);
    RUN_TEST(INT_IS_EQUAL(events_cnt, 1));
    RUN_TEST(INT_IS_EQUAL(events[0], STAT_GGA));
    RUN_TEST(FLT_IS_EQUAL(hgps.altitude, 99.9));

    /* Invalid term with valid checksum is reported as invalid data */
    prv_process(&hgps, "$GPGSV,1,0,02,10,63,137,17,07,61,098,15*78\r\n");
    RUN_TEST(INT_IS_EQUAL(events_cnt, 1));
    RUN_TEST(INT_IS_EQUAL(events[0], STAT_INVALID_DATA));

    /* Invalid term with invalid checksum reports the checksum failure */
    prv_process(&hgps, "$GPGSV,1,0,02,10,63,137,17,07,61,098,15*79\r\n");
    RUN_TEST(INT_IS_EQUAL(events_cnt, 1));
    RUN_TEST(INT_IS_EQUAL(events[0], STAT_CHECKSUM_FAIL));

    return 0;
}
