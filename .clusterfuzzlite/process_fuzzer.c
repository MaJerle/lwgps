#include <stdint.h>
#include <lwgps/lwgps.h>

int
LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    lwgps_t hgps;

    lwgps_init(&hgps);
    lwgps_process(&hgps, data, size);

    return 0;
}
