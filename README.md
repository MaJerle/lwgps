# GPS NMEA 0183 Parser

Platform independent GPS NMEA parser for embedded systems

## Features

- Platform independent, easy to port
- Written in C99 language
- Built-in support for `4` GPS statements
    - `GPGGA` or `GNGGA`: GPS fix data
    - `GPGSA` or `GNGSA`: GPS active satellites and dillusion of position
    - `GPGSV` or `GNGSV`: List of satellites in view zone
    - `GPRMC` or `GNRMC`: Recommended minimum specific GPS/Transit data
- Optional `float` or `double` floating point units
- Low-level layer is separated from application layer, thus allows you to add custom communication with GPS device
- Works with operating systems
- Works with different communication interfaces
- User friendly MIT license

## Documentation

Full API documentation with description and examples is available and is regulary updated with the source changes

http://majerle.eu/documentation/gps/html/index.html

## Contribution

I invite you to give feature request or report a bug. Please use issues tracker.
