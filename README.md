# Lightweight GPS NMEA parser

LwGPS is a lightweight, platform independent, all-constellation GNSS NMEA parser library for embedded systems.

[Open documentation](https://docs.majerle.eu/projects/lwgps/)

## Features

* Written in C (C11), compatible with `stdint.h` data types
* Platform independent, easy to use
* Built-in support for all-constellation GNSS statements
    * `GPGGA`, `G*GGA` or `GNGGA`: GNSS fix data
    * `GPGSA`, `G*GSA` or `GNGSA`: Active satellites and dilution of precision
    * `GPGSV`, `G*GSV` or `GNGSV`: List of satellites in view zone
    * `GPRMC`, `G*RMC` or `GNRMC`: Recommended minimum specific GNSS/Transit data
* Optional support for `PUBX` (uBlox proprietary) statements, including `PUBX_TIME`
* Optional detailed per-satellite info (elevation, azimuth, SNR) for `GSV` statement
* Optional NMEA checksum validation
* Distance and bearing calculation between two coordinates
* Speed conversion helper between many units: metric, imperial and pace
* Optional per-statement processing callback
* C++ wrapper class available
* Optional `float` or `double` floating point units
* Low-level layer is separated from application layer, thus allowing you to add custom communication with GPS device
* Works with operating systems
* Works with different communication interfaces
* Can be used as an ESP-IDF component
* User friendly MIT license

## Contribute

Fresh contributions are always welcome. Simple instructions to proceed:

1. Fork Github repository
2. Follow [C style & coding rules](https://github.com/MaJerle/c-code-style) and use `clang-format` to format the code
3. Create a pull request to `develop` branch with new features or bug fixes

Alternatively you may:

1. Report a bug
2. Ask for a feature request

## Test

To build the code and run basic tests on your host::

        cd examples
        make test
