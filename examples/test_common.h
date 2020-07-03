#ifndef TEST_COMMON_HDR_H
#define TEST_COMMON_HDR_H

#include <math.h>
#include <stdlib.h>

#define RUN_TEST(x)         do {                \
        if ((x)) {                                  \
            printf("Test passed on line %u with condition " # x "\r\n", (unsigned)__LINE__); \
        } else {                                    \
            printf("Test FAILED on line %u with condition " # x "\r\n", (unsigned)__LINE__ ); \
            exit(1);                                \
        }                                           \
    } while (0)
#define FLT_IS_EQUAL(x, y)      (fabs((double)(x) - (double)(y)) < 0.00001)
#define INT_IS_EQUAL(x, y)      ((int)((x) == (y)))

#endif /* TEST_COMMON_HDR_H */
