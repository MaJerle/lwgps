/**
 * \author  Tilen Majerle
 * \email   tilen@majerle.eu
 * \website http://majerle.eu/projects/gps-nmea-parser-parser-for-embedded-systems
 * \license MIT
 * \version v0.2
 * \brief   GPS Library
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
#ifndef GPS_H
#define GPS_H 020

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup GPS
 * \brief  Platform independent, written in ANSII C, GPS AT parser library for SIMcom modules   
 * \{
 ?
 */
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "math.h"

/* Buffer implementation */
#include "buffer.h"

/* GPS configuration */
#include "gps_config.h"

/**
 * @defgroup GPS_Macros
 * @brief    Library defines
 * @{
 */

/**
 * \}
 */
 
/**
 * \defgroup GPS_Typedefs
 * \brief    Library Typedefs
 * \{
 */

/**
 * \brief  GPS library result enumeration
 */
typedef enum _GPS_Result_t {
    gpsOK,                              /*!< OK */
    gpsERROR,                           /*!< An error occurred */
    gpsNODATA,                          /*!< No data from GPS */
    gpsOLDDATA,                         /*!< An old data still remaining */
    gpsNEWDATA                          /*!< New pack of fresh data just received */
} GPS_Result_t;

/**
 * \brief  GPS Fix type
 */
typedef enum _GPS_Fix_t {
    GPS_Fix_Invalid = 0x00,             /*!< No fix */
    GPS_Fix_GPS = 0x01,                 /*!< Fix is GPS */
    GPS_Fix_DGPS = 0x02                 /*!< Fix is Differential GPS */
} GPS_Fix_t;

/**
 * \brief  GPS Fix mode
 */
typedef enum GPS_FixMode_t {
    GPS_FixMode_Invalid = 0x01,         /*!< No fix */
    GPS_FixMode_2D = 0x02,              /*!< 2D fix */
    GPS_FixMode_3D = 0x03               /*!< 3D fix */
} GPS_FixMode_t;

/**
 * \brief  GPS satellite description
 */
typedef struct _GPS_Sat_t {
    uint8_t ID;                         /*!< Satellite ID */
    uint8_t Elevation;                  /*!< Elevation */
    uint16_t Azimuth;                   /*!< Azimuth according to north */
    uint8_t SNR;                        /*!< Signal to noise ration in units of dB */
} GPS_Sat_t;

/**
 * \brief  GPS time
 */
typedef struct _GPS_Time_t {
    uint8_t Hours;                      /*!< Hours in 24h format, 0 to 23 */
    uint8_t Minutes;                    /*!< Minutes */
    uint8_t Seconds;                    /*!< Seconds */
    uint8_t Hundreds;                   /*!< Hundreds of second */
    uint16_t Thousands;                 /*!< Thousands of second */
} GPS_Time_t;

/**
 * \brief  GPS date
 */
typedef struct _GPS_Date_t {
    uint8_t Day;                        /*!< Day in a month, 1 to 31 */
    uint8_t Month;                      /*!< Month in a year, 1 to 12 */
    uint16_t Year;                      /*!< Full year in 2xxx format */
} GPS_Date_t;

/**
 * \brief  GPS speed enumeration
 */
typedef enum _GPS_Speed_t {
    /* Metric */
    GPS_Speed_KilometerPerSecond,       /*!< Convert speed to kilo meters per second */
    GPS_Speed_MeterPerSecond,           /*!< Convert speed to meters per second */
    GPS_Speed_KilometerPerHour,         /*!< Convert speed to kilo meters per hour */
    GPS_Speed_MeterPerMinute,           /*!< Convert speed to meter per minute */
    /* Imperial */
    GPS_Speed_MilePerSecond,            /*!< Convert speed to miles per second */
    GPS_Speed_MilePerHour,              /*!< Convert speed to miles per hour */
    GPS_Speed_FootPerSecond,            /*!< Convert speed to foots per second */
    GPS_Speed_FootPerMinute,            /*!< Convert speed to foots per minute */
    /* For Runners and Joggers */
    GPS_Speed_MinutePerKilometer,       /*!< Convert speed to minutes per kilo meter */
    GPS_Speed_SecondPerKilometer,       /*!< Convert speed to seconds per kilo meter */
    GPS_Speed_SecondPer100Meters,       /*!< Convert speed to seconds per 100 meters */
    GPS_Speed_MinutePerMile,            /*!< Convert speed to minutes per mile */
    GPS_Speed_SecondPerMile,            /*!< Convert speed to seconds per mile */
    GPS_Speed_SecondPer100Yards,        /*!< Convert speed to seconds per 100 yards */
    /* Nautical */
    GPS_Speed_SeaMilePerHour,           /*!< Convert speed to sea miles per hour */
} GPS_Speed_t;

/**
 * \brief  GPS Distance and bearing
 */
typedef struct _GPS_Distance_t  {
    float LatitudeStart;                /*!< Latitude of starting point. */
    float LongitudeStart;               /*!< Longitude of starting point. */
    float LatitudeEnd;                  /*!< Latitude of ending point. */
    float LongitudeEnd;                 /*!< Longitude of ending point. */
    float Distance;                     /*!< Distance between 2 points which will be calculated. */
    float Bearing;                      /*!< Bearing from start to stop point according to North. */
} GPS_Distance_t;

/**
 * \brief  Custom data types for custom statements
 */
typedef enum _GPS_CustomType_t {
    GPS_CustomType_Float,				/*!< Parse received data as float */
    GPS_CustomType_Int,					/*!< Parse received data as int */
    GPS_CustomType_String,				/*!< Parse received data as string */
	GPS_CustomType_Char,				/*!< Parse received data as char */
	GPS_CustomType_LatLong				/*!< Parse received data as latitude or longitude */
} GPS_CustomType_t;

/**
 * \brief  Structure for custom data
 */
typedef struct _GPS_Custom_t {
    const char* Statement;              /*!< Statement value, including "$" at beginning. For example, "$GPRMC" */
    uint8_t TermNumber;                 /*!< Term number position inside statement */
    union {
        char S[13];                     /*!< Value from GPS receiver stored as string */
        char C;                         /*!< Value saved as character */
        float F;                        /*!< Value saved as float number */
        float L;                        /*!< Value saved as latitude/longitude (float number) */
        int I;                          /*!< Value saved as integer number */
    } Value;                            /*!< Union of different types of data to read as user */
    GPS_CustomType_t Type;              /*!< Type for data */
    uint8_t Updated;                    /*!< Updated flag. If this parameter is set to 1, then new update has been made. Meant for private use */
} GPS_Custom_t;

/**
 * \brief  GPS working structure
 */
typedef struct _GPS_t {
    /* GPGGA statement */
    float Latitude;                     /*!< GPS Latitude value */
    float Longitude;                    /*!< GPS Longitude value */
    float Altitude;                     /*!< GPS Altitude value */
    GPS_Fix_t Fix;                      /*!< GPS Fix type */
    uint8_t SatsInUse;                  /*!< Number of satellites in use for fix */
    GPS_Time_t Time;                    /*!< UTC Time */

    /* GPGSA statement */
    GPS_FixMode_t FixMode;              /*!< GPS Fix mode */
    uint8_t SatelliteIDs[12];           /*!< ID numbers of satellites in use. Valid range is from 0 to SatsInUse - 1*/
    float HDOP;                         /*!< Horizontal dilution of precision */
    float PDOP;                         /*!< Position dilution of precision */
    float VDOP;                         /*!< Vertical dilution of precision */

    /* GPGSV statement */
    uint8_t SatsInView;                 /*!< Satellites in view */
    GPS_Sat_t SatsDesc[24];             /*!< Array of descriptions for satellites in view */

    /* GPRMC statement */
    GPS_Date_t Date;                    /*!< UTC date */
    uint8_t Valid;                      /*!< GPS valid status */
    float Speed;                        /*!< Current speed in knots */
    float Coarse;                       /*!< Current coarse on ground */
    float Variation;                    /*!< Variation value */

    /* Private data */
    GPS_Custom_t* CustomStatements[GPS_CUSTOM_COUNT];   /*!< Array of pointers to custom GPS statements */
    uint8_t CustomStatementsCount;      /*!< Number of custom GPS statements */
} GPS_t;

/**
* \}
*/

/**
* \defgroup GPS_Functions
* \brief    Library functions
* \{
*/

/**
 * \brief         Initialize GPS stack
 * \param[in,out] *GPS: Pointer to working \ref GPS_t structure
 * \retval        Member of \ref GPS_Result_t enumeration
 */
GPS_Result_t GPS_Init(GPS_t* GPS);

/**
 * \brief         Add new data to GPS stack
 * \note          This function must be called when new data are received from GPS. Use it as input method for GPS stack
 * \param[in]     *ch: Pointer to characters to write to stack
 * \param[in]     count: Number of characters to write at a time
 * \retval        Number of characters written to buffer
 */
uint32_t GPS_DataReceived(uint8_t* ch, size_t count);

/**
 * \brief         Process update GPS stack
 * \param[in,out] *GPS: Pointer to working \ref GPS_t structure
 * \retval        Member of \ref GPS_Result_t enumeration
 */
GPS_Result_t GPS_Update(GPS_t* GPS);

/**
 * \brief         Convert speed in knots (from GPS) to user selectable speed
 * \param[in]     SpeedInKnots: float value from GPS module
 * \param[in]     toSpeed: Select to which speed you want conversion from knot. This parameter ca be a value of GPS_Speed_t enumeration.
 * \retval        Calculated speed from knots to user selectable format
 */
float GPS_ConvertSpeed(float SpeedInKnots, GPS_Speed_t toSpeed);

/**
 * \brief         Calculate distance between 2 coordinates on earth and bearing from start to end point in relation to the north
 * \note          Calculation results will be saved in *Distance_Data @ref GPS_Distance_t structure
 * \param[in,out] *Distance: Pointer to @ref GPS_Distance_t structure with latitude and longitude set values
 * \retval        Member of \ref GPS_Result_t enumeration
 */
GPS_Result_t GPS_DistanceBetween(GPS_Distance_t* Distance);

/**
 * \brief         Add custom GPGxx statement to array of user selectable statements.
 *                   Array is available to user using \ref GPS_t working structure
 * \note          Also note, that your GPS receiver HAVE TO send statement type you use in this function, or
 *                   \ref GPS_Update function will always return that there is no new data available to read
 * \param[in,out] *GPS: Pointer to working \ref GPS_t structure
 * \param[out]    *Custom: Pointer to empty \ref GPS_Custom_t structure
 * \param[in]     *GPG_Statement: String of NMEA starting line address, including "$" at beginning
 * \param[in]     TermNumber: Position in NMEA statement
 * \param[in]     Type: Data type to parse at given position
 * \retval        Member of \ref GPS_Result_t enumeration
 */
GPS_Result_t GPS_Custom_Add(GPS_t* GPS, GPS_Custom_t* Custom, const char* GPG_Statement, uint8_t TermNumber, GPS_CustomType_t Type);

/**
* \brief         Delete custom GPGxx statement from array of user selectable statements.
*                   Array is available to user using \ref GPS_t working structure
* \param[in,out] *GPS: Pointer to working \ref GPS_t structure
* \param[in]     *Custom: Pointer to \ref GPS_Custom_t structure to delete from list of custom statements
* \retval        Member of \ref GPS_Result_t enumeration
*/
GPS_Result_t GPS_Custom_Delete(GPS_t* GPS, GPS_Custom_t* Custom);

/**
 * \}
 */

/**
 * \}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
