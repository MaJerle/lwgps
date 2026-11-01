/**
 * \file            test_parse_framing_nocrc.c
 * \brief           Test statement framing with checksum check disabled
 */

#include <stdio.h>
#include <string.h>
#include "lwgps/lwgps.h"
#include "test.h"

/**
 * \brief           Run the framing test without checksum
 */
int
test_run(void) {
    static const char with_crc[] = "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,2.5*00\r\n";
    static const char no_crc[] = "$GPGGA,092751.000,5321.6802,N,00630.3372,W,1,8,1.03,99.9,M,55.2,M,3.5\r\n";
    lwgps_t hgps;

    lwgps_init(&hgps);

    /* Checksum value is not verified */
    lwgps_process(&hgps, with_crc, strlen(with_crc));
    RUN_TEST(FLT_IS_EQUAL(hgps.altitude, 61.7));
    RUN_TEST(FLT_IS_EQUAL(hgps.dgps_age, 2.5));

    /* Statement without checksum is accepted, including its last term */
    lwgps_process(&hgps, no_crc, strlen(no_crc));
    RUN_TEST(FLT_IS_EQUAL(hgps.altitude, 99.9));
    RUN_TEST(FLT_IS_EQUAL(hgps.dgps_age, 3.5));

    return 0;
}
