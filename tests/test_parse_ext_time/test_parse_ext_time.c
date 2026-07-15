/**
 * \file            test_parse_ext_time.c
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

#if !LWGPS_CFG_STATEMENT_PUBX_TIME
#error "this test must be compiled with -DLWGPS_CFG_STATEMENT_PUBX_TIME=1"
#endif /* !LWGPS_CFG_STATEMENT_PUBX_TIME */

/* GPS handle */
static lwgps_t hgps;

/**
 * \brief           Dummy data from GPS receiver
 */
const char gps_rx_data_A[] = ""
                             "$PUBX,04*37\r\n"
                             "$PUBX,04,073731.00,091202,113851.00,1196,15D,1930035,-2660.664,43*71\r\n"
                             "";
const char gps_rx_data_B[] = ""
                             "$PUBX,04,200714.00,230320,158834.00,2098,18,536057,257.043,16*12\r\n"
                             "";
const char gps_rx_data_C[] = ""
                             "$PUBX,04,1,2,0,0,3,0,0,0*37\r\n"
                             "";

/**
 * \brief           Run the test of raw input data
 */
int
test_run(void) {
    lwgps_init(&hgps);

    /* Process and test block A */
    lwgps_process(&hgps, gps_rx_data_A, strlen(gps_rx_data_A));

    RUN_TEST(INT_IS_EQUAL(hgps.hours, 7));
    RUN_TEST(INT_IS_EQUAL(hgps.minutes, 37));
    RUN_TEST(INT_IS_EQUAL(hgps.seconds, 31));
    RUN_TEST(INT_IS_EQUAL(hgps.date, 9));
    RUN_TEST(INT_IS_EQUAL(hgps.month, 12));
    RUN_TEST(INT_IS_EQUAL(hgps.year, 2));
    RUN_TEST(FLT_IS_EQUAL(hgps.utc_tow, 113851.00));
    RUN_TEST(INT_IS_EQUAL(hgps.utc_wk, 1196));
    RUN_TEST(INT_IS_EQUAL(hgps.leap_sec, 15));
    RUN_TEST(INT_IS_EQUAL(hgps.clk_bias, 1930035));
    RUN_TEST(FLT_IS_EQUAL(hgps.clk_drift, -2660.664));
    RUN_TEST(INT_IS_EQUAL(hgps.tp_gran, 43));

    /* Process and test block B */
    lwgps_process(&hgps, gps_rx_data_B, strlen(gps_rx_data_B));

    RUN_TEST(INT_IS_EQUAL(hgps.hours, 20));
    RUN_TEST(INT_IS_EQUAL(hgps.minutes, 7));
    RUN_TEST(INT_IS_EQUAL(hgps.seconds, 14));
    RUN_TEST(INT_IS_EQUAL(hgps.date, 23));
    RUN_TEST(INT_IS_EQUAL(hgps.month, 3));
    RUN_TEST(INT_IS_EQUAL(hgps.year, 20));
    RUN_TEST(FLT_IS_EQUAL(hgps.utc_tow, 158834.00));
    RUN_TEST(INT_IS_EQUAL(hgps.utc_wk, 2098));
    RUN_TEST(INT_IS_EQUAL(hgps.leap_sec, 18));
    RUN_TEST(INT_IS_EQUAL(hgps.clk_bias, 536057));
    RUN_TEST(FLT_IS_EQUAL(hgps.clk_drift, 257.043));
    RUN_TEST(INT_IS_EQUAL(hgps.tp_gran, 16));

    /* Process and test truncated time, date, and leap-second fields */
    lwgps_process(&hgps, gps_rx_data_C, strlen(gps_rx_data_C));

    RUN_TEST(INT_IS_EQUAL(hgps.hours, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.minutes, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.seconds, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.date, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.month, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.year, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.leap_sec, 0));

    return 0;
}
