/*
 * This example uses direct processing function,
 * to process dummy NMEA data from GPS receiver
 */
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "lwgps/lwgps.h"

/* External function */
extern void run_tests();

int
main() {
    lwgps_float_t distance, bearing;
    run_tests();

    /* Calculate distance and bearing */
    lwgps_distance_bearing(40.6, -73.7, 48.3, 11.7, &distance, &bearing);

    printf("Distance: %lf meters\r\n", (double)distance);
    printf("Bearing:  %lf degrees\r\n", (double)bearing);

    lwgps_distance_bearing(48.3, 11.7, 40.6, -73.7, &distance, &bearing);
    printf("Distance: %lf meters\r\n", (double)distance);
    printf("Bearing:  %lf degrees\r\n", (double)bearing);

    return 0;
}

/* JFK: 40.642569, -73.783790 */
/* Munich: 48.353962, 11.775114 */
