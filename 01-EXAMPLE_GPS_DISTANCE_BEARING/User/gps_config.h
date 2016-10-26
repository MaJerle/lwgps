/**
 * \author  Tilen Majerle
 * \email   tilen@majerle.eu
 * \website http://majerle.eu/projects/gps-nmea-parser-parser-for-embedded-systems
 * \license MIT
 * \brief   Configuration part for GPS library
 *	
\verbatim
   ----------------------------------------------------------------------
    Copyright (c) 2016 Tilen Majerle

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software, 
    and to permit persons to whom the Software is furnished to do so, 
    subject to the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
    AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
   ----------------------------------------------------------------------
\endverbatim
 */
#ifndef GPS_CONF_H
#define GPS_CONF_H 010

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/*                                                                        */
/*   Edit file name to gps_conf.h and edit values for your platform       */
/*                                                                        */
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

/**
 * \defgroup GPS_CONF
 * \brief    Configuration parameters for GPS library
 * \{
 */

/**
 * \brief  Sets number of maximal custom GPS statements allowed in library
 *
 */
#define GPS_CUSTOM_COUNT            5

/**
 * \brief  Set receive data buffer size for GPS parsing
 *
 */
#define GPS_BUFFER_SIZE             256

/**
 * \}
 */

#endif
