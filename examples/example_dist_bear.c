#include "lwgps/lwgps.h"

/* Distance and bearing results */
lwgps_float_t dist, bear;

/* New York coordinates */
lwgps_float_t lat1 = 40.685721;
lwgps_float_t lon1 = -73.820465;

/* Munich coordinates */
lwgps_float_t lat2 = 48.150906;
lwgps_float_t lon2 = 11.554176;

/* Go from New York to Munich */
/* Calculate distance and bearing related to north */
lwgps_distance_bearing(lat1, lon1, lat2, lon2, &dist, &bear);
printf("Distance: %f meters\r\n", (float)dist);
printf("Initial bearing: %f degrees\r\n", (float)bear);

/* Go from Munich to New York */
/* Calculate distance and bearing related to north */
lwgps_distance_bearing(lat2, lon2, lat1, lon1, &dist, &bear);
printf("Distance: %f meters\r\n", (float)dist);
printf("Initial bearing: %f degrees\r\n", (float)bear);