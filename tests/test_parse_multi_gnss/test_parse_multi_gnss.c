/**
 * \file            test_parse_multi_gnss.c
 * \brief           Test satellite data consistency with multi-GNSS (GPS + BeiDou) receiver
 * \version         0.1
 * \date            2026-10-22
 */
#include <stdio.h>
#include <string.h>
#include "lwgps/lwgps.h"
#include "test.h"

/**
 * \brief           One epoch of ATGM336H (GPS + BeiDou) receiver, see issue #49
 *                  8 satellites in use (7 GPS + 1 BeiDou), 14 in view (10 GPS + 4 BeiDou)
 */
static const char* const epoch_lines[] = {
    "$GNGGA,230924.000,3746.55402,N,12223.73182,W,1,08,1.0,-3.6,M,0.0,M,,*40\r\n",
    "$GNGLL,3746.55402,N,12223.73182,W,230924.000,A,A*54\r\n",
    "$GPGSA,A,3,05,07,08,09,17,21,22,,,,,,2.4,1.0,2.2*33\r\n",
    "$BDGSA,A,3,37,,,,,,,,,,,,2.4,1.0,2.2*20\r\n",
    "$GPGSV,3,1,10,05,16,282,26,07,55,078,16,08,29,052,20,09,23,159,19*70\r\n",
    "$GPGSV,3,2,10,14,,,13,17,25,178,29,20,27,315,16,21,19,248,18*41\r\n",
    "$GPGSV,3,3,10,22,47,244,16,30,71,352,09*76\r\n",
    "$BDGSV,1,1,04,20,,,20,25,,,19,37,43,281,24,38,,,31*54\r\n",
    "$GNRMC,230924.000,A,3746.55402,N,12223.73182,W,0.35,238.88,211026,,,A*6A\r\n",
    "$GNVTG,238.88,T,,M,0.35,N,0.65,K,A*2F\r\n",
    "$GNZDA,230924.000,21,10,2026,00,00*42\r\n",
    "$GPTXT,01,01,01,ANTENNA OK*35\r\n",
};

static const uint8_t ids_in_use[] = {5, 7, 8, 9, 17, 21, 22, 37};
static const uint8_t ids_in_view[] = {5, 7, 8, 9, 14, 17, 20, 21, 22, 30, 20, 25, 37, 38};

/**
 * \brief           Check that published satellite data matches the complete epoch
 * \return          `0` on success, `-1` otherwise
 */
static int
prv_check_complete(const lwgps_t* hgps) {
    RUN_TEST(INT_IS_EQUAL(hgps->sats_in_use, 8));
    for (size_t i = 0; i < sizeof(hgps->satellites_ids); ++i) {
        RUN_TEST(INT_IS_EQUAL(hgps->satellites_ids[i], i < sizeof(ids_in_use) ? ids_in_use[i] : 0));
    }
    RUN_TEST(INT_IS_EQUAL(hgps->sats_in_view, 14));
    for (size_t i = 0; i < sizeof(hgps->sats_in_view_desc) / sizeof(hgps->sats_in_view_desc[0]); ++i) {
        RUN_TEST(INT_IS_EQUAL(hgps->sats_in_view_desc[i].num, i < sizeof(ids_in_view) ? ids_in_view[i] : 0));
    }
    return 0;
}

/**
 * \brief           Run the test of multi-GNSS input data
 */
int
test_run(void) {
    lwgps_t hgps;
    lwgps_init(&hgps);

    /* First epoch: everything is published once both GSA and GSV blocks completed */
    for (size_t i = 0; i < sizeof(epoch_lines) / sizeof(epoch_lines[0]); ++i) {
        lwgps_process(&hgps, epoch_lines[i], strlen(epoch_lines[i]));
    }
    RUN_TEST(prv_check_complete(&hgps) == 0);
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[13].elevation, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[13].snr, 31));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[12].azimuth, 281));

    /* Next epochs: data must stay consistent after every single sentence, user may read at any time */
    for (size_t epoch = 0; epoch < 2; ++epoch) {
        for (size_t i = 0; i < sizeof(epoch_lines) / sizeof(epoch_lines[0]); ++i) {
            lwgps_process(&hgps, epoch_lines[i], strlen(epoch_lines[i]));
            RUN_TEST(prv_check_complete(&hgps) == 0);
        }
    }
    return 0;
}
