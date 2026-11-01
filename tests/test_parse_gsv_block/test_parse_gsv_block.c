/**
 * \file            test_parse_gsv_block.c
 * \brief           Test GSV/GSA block validation
 *
 * Satellite data of an epoch is published to the user only when the complete block is valid.
 * Any invalid statement aborts the whole block, and previous epoch data is kept.
 */

#include <stdio.h>
#include <string.h>
#include "lwgps/lwgps.h"
#include "test.h"

/* Epoch A: GPS series of 2 messages (6 satellites) + GLONASS series of 1 message (3 satellites) */
#define A_GP1             "$GPGSV,2,1,06,01,10,100,30,02,20,200,31,03,30,300,32,04,40,040,33*78\r\n"
#define A_GP2             "$GPGSV,2,2,06,05,50,050,34,06,60,060,35*7D\r\n"
#define A_GL1             "$GLGSV,1,1,03,65,10,100,20,66,20,200,21,67,30,300,22*55\r\n"

/* Epoch B: GPS series of 2 messages (5 satellites) + GLONASS series of 1 message (2 satellites) */
#define B_GP1             "$GPGSV,2,1,05,11,10,100,30,12,20,200,31,13,30,300,32,14,40,040,33*7B\r\n"
#define B_GP2             "$GPGSV,2,2,05,15,50,050,34*4F\r\n"
#define B_GL1             "$GLGSV,1,1,02,71,10,100,20,72,20,200,21*65\r\n"

/* Faulty variants of epoch B statements */
#define B_GP2_HDR_CORRUPT "$GPGSW,2,2,05,15,50,050,34*4F\r\n"              /* `GSV` -> `GSW`, single bit flip */
#define B_GL1_CRC_CORRUPT "$GLGSV,1,1,02,71,10,100,20,72,20,200,21*64\r\n" /* Checksum fails */
#define B_GP2_NUM_0       "$GPGSV,2,0,05,15,50,050,34*4D\r\n"              /* Message number `0` */
#define B_GP2_TALKER_GL   "$GLGSV,2,2,05,15,50,050,34*53\r\n"              /* Talker changed within series */
#define B_GP2_SATS_6      "$GPGSV,2,2,06,15,50,050,34*4C\r\n"              /* Satellites changed within series */
#define B_GP2_NUM_MSGS_3  "$GPGSV,3,2,05,15,50,050,34*4E\r\n"              /* Messages changed within series */
#define B_GP_TOO_MANY                                                                                                  \
    "$GPGSV,1,1,05,11,10,100,30,12,20,200,31,13,30,300,32,14,40,040,33*78\r\n" /* 5 satellites in 1 message */

/* GSA block per epoch, one statement per constellation */
#define A_GPGSA             "$GPGSA,A,3,01,02,03,,,,,,,,,,1.5,1.0,1.1*37\r\n"
#define A_GLGSA             "$GLGSA,A,3,65,66,,,,,,,,,,,1.5,1.0,1.1*28\r\n"
#define B_GPGSA             "$GPGSA,A,3,11,12,,,,,,,,,,,1.5,1.0,1.1*34\r\n"
#define B_GLGSA             "$GLGSA,A,3,71,,,,,,,,,,,,1.5,1.0,1.1*2D\r\n"
#define B_GLGSA_CRC_CORRUPT "$GLGSA,A,3,71,,,,,,,,,,,,1.5,1.0,1.1*2C\r\n"

#define GGA                 "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76\r\n"

#define EPOCH_A             A_GPGSA A_GLGSA A_GP1 A_GP2 A_GL1 GGA
#define EPOCH_B             B_GPGSA B_GLGSA B_GP1 B_GP2 B_GL1 GGA

static const uint8_t sats_a[] = {1, 2, 3, 4, 5, 6, 65, 66, 67};
static const uint8_t sats_b[] = {11, 12, 13, 14, 15, 71, 72};
static const uint8_t ids_a[] = {1, 2, 3, 65, 66};
static const uint8_t ids_b[] = {11, 12, 71};

static lwgps_t hgps;
static lwgps_statement_t events[16];
static uint8_t events_sats_in_view[16]; /*!< Published satellites in view, at the time of the event */
static size_t events_cnt;

/**
 * \brief           Record every reported statement
 */
static void
prv_evt_fn(lwgps_statement_t res) {
    if (events_cnt < sizeof(events) / sizeof(events[0])) {
        events[events_cnt] = res;
        events_sats_in_view[events_cnt] = hgps.sats_in_view;
    }
    ++events_cnt;
}

/**
 * \brief           Reset recorded events and process the string
 */
static void
prv_process(const char* str) {
    events_cnt = 0;
    lwgps_process(&hgps, str, strlen(str), prv_evt_fn);
}

/**
 * \brief           Check published satellites in view against expected list
 */
static int
prv_check_sats(const uint8_t* sats, size_t cnt) {
    if (hgps.sats_in_view != cnt || hgps.sats_in_view_desc[cnt].num != 0) {
        return 0;
    }
    for (size_t i = 0; i < cnt; ++i) {
        if (hgps.sats_in_view_desc[i].num != sats[i]) {
            return 0;
        }
    }
    return 1;
}

/**
 * \brief           Check published satellites in use against expected list
 */
static int
prv_check_ids(const uint8_t* ids, size_t cnt) {
    if (hgps.satellites_ids[cnt] != 0) {
        return 0;
    }
    for (size_t i = 0; i < cnt; ++i) {
        if (hgps.satellites_ids[i] != ids[i]) {
            return 0;
        }
    }
    return 1;
}

#define CHECK_SATS_A() prv_check_sats(sats_a, sizeof(sats_a))
#define CHECK_SATS_B() prv_check_sats(sats_b, sizeof(sats_b))
#define CHECK_IDS_A()  prv_check_ids(ids_a, sizeof(ids_a))
#define CHECK_IDS_B()  prv_check_ids(ids_b, sizeof(ids_b))

/**
 * \brief           Faulty epoch B streams, each must keep epoch A satellites in view
 */
static const struct {
    const char* name;
    const char* stream;
} faulty_epochs[] = {
    {"corrupted header of GP 2/2", B_GP1 B_GP2_HDR_CORRUPT B_GL1 GGA},
    {"checksum fail on single message GL series", B_GP1 B_GP2 B_GL1_CRC_CORRUPT GGA},
    {"missing GP 2/2", B_GP1 B_GL1 GGA},
    {"GP 2/2 before GP 1/2", B_GP2 B_GP1 B_GL1 GGA},
    {"repeated GP 1/2", B_GP1 B_GP1 B_GP2 B_GL1 GGA},
    {"talker changed within series", B_GP1 B_GP2_TALKER_GL B_GL1 GGA},
    {"satellites changed within series", B_GP1 B_GP2_SATS_6 B_GL1 GGA},
    {"messages changed within series", B_GP1 B_GP2_NUM_MSGS_3 B_GL1 GGA},
    {"more satellites than series can describe", B_GP_TOO_MANY B_GL1 GGA},
    {"block ends with open series", B_GP1 GGA},
    {"block ends with open series, after complete series", B_GL1 B_GP1 GGA},
    {"checksum fail on statement after the block", B_GP1 B_GP2 B_GL1 "$GPGGA,*00\r\n" GGA},
};

/**
 * \brief           Epoch B streams with message number `0` in one statement, at different positions and talkers.
 *                  Statement must be reported as invalid data and abort the block
 */
static const struct {
    const char* name;
    const char* stream;
    size_t lines;   /*!< Number of statements in the stream */
    size_t bad_idx; /*!< Index of the statement with message number `0` */
} zero_num_epochs[] = {
    {"first message of the block",
     "$GPGSV,2,0,05,11,10,100,30,12,20,200,31,13,30,300,32,14,40,040,33*7A\r\n" B_GP2 B_GL1 GGA, 4, 0},
    {"middle of open series", B_GP1 B_GP2_NUM_0 B_GL1 GGA, 4, 1},
    {"single message series at the end of the block",
     B_GP1 B_GP2 "$GLGSV,1,0,02,71,10,100,20,72,20,200,21*64\r\n" GGA, 4, 2},
    {"zero total messages", B_GP1 B_GP2 "$GLGSV,0,0,02,71,10,100,20,72,20,200,21*65\r\n" GGA, 4, 2},
    {"talker GP", B_GP1 B_GP2 B_GL1 "$GPGSV,1,0,01,40,10,100,20*4F\r\n" GGA, 5, 3},
    {"talker GL", B_GP1 B_GP2 B_GL1 "$GLGSV,1,0,01,40,10,100,20*53\r\n" GGA, 5, 3},
    {"talker GA", B_GP1 B_GP2 B_GL1 "$GAGSV,1,0,01,40,10,100,20*5E\r\n" GGA, 5, 3},
    {"talker GB", B_GP1 B_GP2 B_GL1 "$GBGSV,1,0,01,40,10,100,20*5D\r\n" GGA, 5, 3},
    {"talker BD", B_GP1 B_GP2 B_GL1 "$BDGSV,1,0,01,40,10,100,20*5E\r\n" GGA, 5, 3},
    {"talker GQ", B_GP1 B_GP2 B_GL1 "$GQGSV,1,0,01,40,10,100,20*4E\r\n" GGA, 5, 3},
    {"talker GN", B_GP1 B_GP2 B_GL1 "$GNGSV,1,0,01,40,10,100,20*51\r\n" GGA, 5, 3},
};

/**
 * \brief           Run the GSV/GSA block test
 */
int
test_run(void) {
    size_t faulty_failed = 0;

    /* Clean epochs are published, each one replaces the previous */
    lwgps_init(&hgps);
    prv_process(EPOCH_A);
    RUN_TEST(CHECK_SATS_A());
    RUN_TEST(CHECK_IDS_A());
    prv_process(EPOCH_B);
    RUN_TEST(CHECK_SATS_B());
    RUN_TEST(CHECK_IDS_B());

    /* Block is published only when statement of another type ends it */
    lwgps_init(&hgps);
    prv_process(EPOCH_A);
    prv_process(B_GP1 B_GP2 B_GL1);
    RUN_TEST(CHECK_SATS_A());
    prv_process(GGA);
    RUN_TEST(CHECK_SATS_B());

    /*
     * GSV block last in the burst: it is published by the first statement of the next burst,
     * exactly at its end of line, before its event is reported
     */
    lwgps_init(&hgps);
    prv_process(GGA A_GPGSA A_GLGSA A_GP1 A_GP2 A_GL1);
    RUN_TEST(INT_IS_EQUAL(events_cnt, 6));
    for (size_t i = 0; i < events_cnt; ++i) {
        RUN_TEST(INT_IS_EQUAL(events_sats_in_view[i], 0)); /* Nothing published during the burst */
    }
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[0].num, 0));
    RUN_TEST(CHECK_IDS_A()); /* GSA block was ended by the first GSV statement */

    /* First statement of the next burst, byte by byte: unchanged until its end of line */
    events_cnt = 0;
    for (const char* ch = GGA; *ch != '\0'; ++ch) {
        lwgps_process(&hgps, ch, 1, prv_evt_fn);
        if (*ch == '\r') {
            break;
        }
        RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view, 0));
        RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[0].num, 0));
        RUN_TEST(INT_IS_EQUAL(events_cnt, 0));
    }
    RUN_TEST(CHECK_SATS_A());
    RUN_TEST(INT_IS_EQUAL(events_cnt, 1));
    RUN_TEST(INT_IS_EQUAL(events[0], STAT_GGA));
    RUN_TEST(INT_IS_EQUAL(events_sats_in_view[0], sizeof(sats_a))); /* Already published in the event */

    /* Rest of the burst, GSV last again: previous block stays published */
    prv_process("\n" B_GPGSA B_GLGSA B_GP1 B_GP2 B_GL1);
    RUN_TEST(INT_IS_EQUAL(events_cnt, 5));
    for (size_t i = 0; i < events_cnt; ++i) {
        RUN_TEST(INT_IS_EQUAL(events_sats_in_view[i], sizeof(sats_a)));
    }
    RUN_TEST(CHECK_SATS_A());

    /* Statement unknown to the library ends the block too */
    prv_process("$GPTXT,01,01,01,ANTENNA OK*35\r\n");
    RUN_TEST(INT_IS_EQUAL(events_cnt, 1));
    RUN_TEST(INT_IS_EQUAL(events[0], STAT_UNKNOWN));
    RUN_TEST(INT_IS_EQUAL(events_sats_in_view[0], sizeof(sats_b)));
    RUN_TEST(CHECK_SATS_B());

    /* Faulty epochs keep previous data, and next clean epoch recovers */
    for (size_t i = 0; i < sizeof(faulty_epochs) / sizeof(faulty_epochs[0]); ++i) {
        lwgps_init(&hgps);
        prv_process(EPOCH_A);
        prv_process(faulty_epochs[i].stream);
        if (!CHECK_SATS_A()) {
            printf("Faulty epoch not discarded: %s\r\n", faulty_epochs[i].name);
            ++faulty_failed;
        }
        prv_process(EPOCH_B);
        if (!CHECK_SATS_B()) {
            printf("No recovery after faulty epoch: %s\r\n", faulty_epochs[i].name);
            ++faulty_failed;
        }
    }
    RUN_TEST(INT_IS_EQUAL(faulty_failed, 0));

    /* Message number `0` is invalid data, aborts the block, and next clean epoch recovers */
    for (size_t i = 0; i < sizeof(zero_num_epochs) / sizeof(zero_num_epochs[0]); ++i) {
        size_t failed = 0;

        lwgps_init(&hgps);
        prv_process(EPOCH_A);
        prv_process(zero_num_epochs[i].stream);
        if (events_cnt != zero_num_epochs[i].lines) {
            ++failed;
        } else {
            for (size_t j = 0; j < events_cnt; ++j) {
                if (j == zero_num_epochs[i].bad_idx ? events[j] != STAT_INVALID_DATA
                                                    : (events[j] == STAT_INVALID_DATA
                                                       || events[j] == STAT_CHECKSUM_FAIL)) {
                    ++failed;
                }
            }
        }
        if (failed) {
            printf("Message number 0 not reported as invalid data: %s\r\n", zero_num_epochs[i].name);
        }
        if (!CHECK_SATS_A()) {
            printf("Message number 0 did not abort the block: %s\r\n", zero_num_epochs[i].name);
            ++failed;
        }
        prv_process(EPOCH_B);
        if (!CHECK_SATS_B()) {
            printf("No recovery after message number 0: %s\r\n", zero_num_epochs[i].name);
            ++failed;
        }
        faulty_failed += failed;
    }
    RUN_TEST(INT_IS_EQUAL(faulty_failed, 0));

    /* Start in the middle of a block, e.g. after power-up: nothing is published until the next block */
    lwgps_init(&hgps);
    prv_process(B_GP2 B_GL1 GGA);
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view, 0));
    RUN_TEST(INT_IS_EQUAL(hgps.sats_in_view_desc[0].num, 0));
    prv_process(EPOCH_B);
    RUN_TEST(CHECK_SATS_B());

    /* Events: out of sequence statement is invalid data, valid statements after abort are reported but ignored */
    lwgps_init(&hgps);
    prv_process(EPOCH_A);
    prv_process(B_GP1 B_GP2_TALKER_GL B_GL1 GGA);
    RUN_TEST(INT_IS_EQUAL(events_cnt, 4));
    RUN_TEST(INT_IS_EQUAL(events[0], STAT_GSV));
    RUN_TEST(INT_IS_EQUAL(events[1], STAT_INVALID_DATA));
    RUN_TEST(INT_IS_EQUAL(events[2], STAT_GSV));
    RUN_TEST(INT_IS_EQUAL(events[3], STAT_GGA));
    RUN_TEST(CHECK_SATS_A());

    /* GSA block with checksum fail keeps previous satellites in use, next clean block recovers */
    lwgps_init(&hgps);
    prv_process(EPOCH_A);
    prv_process(B_GPGSA B_GLGSA_CRC_CORRUPT B_GP1 B_GP2 B_GL1 GGA);
    RUN_TEST(CHECK_IDS_A());
    RUN_TEST(CHECK_SATS_B()); /* GSV block is independent of GSA block */
    prv_process(EPOCH_B);
    RUN_TEST(CHECK_IDS_B());

    return 0;
}
