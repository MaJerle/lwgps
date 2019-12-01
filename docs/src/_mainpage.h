/**
 * \mainpage
 * \tableofcontents
 * 
 * GPS NMEA library is a generic parser for NMEA 0183 standard output from GPS devices
 *
 * \section         sect_features Features
 *
 *  - Platform independent, easy to port
 *  - Written in C99 language
 *  - Built-in support for `4` GPS statements
 *     - `GPGGA` or `GNGGA`: GPS fix data
 *     - `GPGSA` or `GNGSA`: GPS active satellites and dillusion of position
 *     - `GPGSV` or `GNGSV`: List of satellites in view zone
 *     - `GPRMC` or `GNRMC`: Recommended minimum specific GPS/Transit data
 *  - Optional `float` or `double` floating point units
 *  - Low-level layer is separated from application layer, thus allows you to add custom communication with GPS device
 *  - Works with operating systems
 *  - Works with different communication interfaces
 *  - User friendly MIT license
 *
 * \section         sect_resources Download & Resources
 *
 *  - <a class="download_url" href="https://github.com/MaJerle/GPS_NMEA_Parser/releases">Download library from Github releases</a>
 *  - <a href="https://github.com/MaJerle/GPS_NMEA_Parser_res">Resources and examples repository</a>
 *  - <a href="https://github.com/MaJerle/GPS_NMEA_Parser">Official development repository on Github</a>
 *
 * \section         sect_contribute How to contribute
 * 
 *  - Official development repository is hosted on Github
 *  - <a href="https://github.com/MaJerle/c_code_style">Respect C style and coding rules</a>
 *
 * \section         sect_license License
 *
 * \verbatim        
 * Copyright (c) 2019 Tilen MAJERLE
 *  
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE. \endverbatim
 *
 */