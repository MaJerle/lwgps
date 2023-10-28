/*
 * This example uses direct processing function,
 * to process dummy NMEA data from GPS receiver
 */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "lwgps/lwgps.h"
#include "lwgps/lwgps.hpp"

/* External function */
extern "C" void run_tests();

Lwgps::Lwgps gps;

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

    gps.distance_bearing(0, 0, 0, 0, 0, 0);

    return 0;
}

/* JFK: 40.642569, -73.783790 */
/* Munich: 48.353962, 11.775114 */
