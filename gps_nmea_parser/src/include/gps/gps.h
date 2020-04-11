/**
 * \file            gps.h
 * \brief           GPS main file
 */

/*
 * Copyright (c) 2020 Tilen MAJERLE
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
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of GPS NMEA parser library.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 * Version:         v1.1.0
 */
#ifndef GPS_HDR_H
#define GPS_HDR_H

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \addtogroup      GPS_NMEA
 * \defgroup        GPS_NMEA_CONFIG Configuration
 * \brief           Default configuration setup
 * \{
 */

/**
 * \brief           Enables `1` or disables `0` `double precision` for floating point
 *                  values such as latitude, longitude, altitude.
 *
 *                  `double` is used as variable type when enabled, `float` when disabled.
 */
#ifndef GPS_CFG_DOUBLE
#define GPS_CFG_DOUBLE                      1
#endif

/**
 * \brief           Enables `1` or disables `0` status reporting callback
 *                  by gps_process()
 *
 * \note            This is an extension, so not enabled by default.
 */
#ifndef GPS_CFG_STATUS
#define GPS_CFG_STATUS                      0
#endif

/**
 * \brief           Enables `1` or disables `0` `GGA` statement parsing.
 *
 * \note            This statement must be enabled to parse:
 *                      - Latitude, Longitude, Altitude
 *                      - Number of satellites in use, fix (no fix, GPS, DGPS), UTC time
 */
#ifndef GPS_CFG_STATEMENT_GPGGA
#define GPS_CFG_STATEMENT_GPGGA             1
#endif

/**
 * \brief           Enables `1` or disables `0` `GSA` statement parsing.
 *
 * \note            This statement must be enabled to parse:
 *                      - Position/Vertical/Horizontal dilution of precision
 *                      - Fix mode (no fix, 2D, 3D fix)
 *                      - IDs of satellites in use
 */
#ifndef GPS_CFG_STATEMENT_GPGSA
#define GPS_CFG_STATEMENT_GPGSA             1
#endif

/**
 * \brief           Enables `1` or disables `0` `RMC` statement parsing.
 *
 * \note            This statement must be enabled to parse:
 *                      - Validity of GPS signal
 *                      - Ground speed in knots and coarse in degrees
 *                      - Magnetic variation
 *                      - UTC date
 */
#ifndef GPS_CFG_STATEMENT_GPRMC
#define GPS_CFG_STATEMENT_GPRMC             1
#endif

/**
 * \brief           Enables `1` or disables `0` `GSV` statement parsing.
 *
 * \note            This statement must be enabled to parse:
 *                      - Number of satellites in view
 *                      - Optional details of each satellite in view. See \ref GPS_CFG_STATEMENT_GPGSV_SAT_DET
 */
#ifndef GPS_CFG_STATEMENT_GPGSV
#define GPS_CFG_STATEMENT_GPGSV             1
#endif

/**
 * \brief           Enables `1` or disables `0` detailed parsing of each
 *                  satellite in view for `GSV` statement.
 *
 * \note            When this feature is disabled, only number of "satellites in view" is parsed
 */
#ifndef GPS_CFG_STATEMENT_GPGSV_SAT_DET
#define GPS_CFG_STATEMENT_GPGSV_SAT_DET     0
#endif

/**
 * \brief           Enables `1` or disables `0` parsing and generation
 *                  of PUBX (uBlox) messages
 *
 *                  PUBX are a nonstandard ublox-specific extensions,
 *                  so disabled by default.
 */
#ifndef GPS_CFG_STATEMENT_PUBX
#define GPS_CFG_STATEMENT_PUBX     0
#endif

/**
 * \brief           Enables `1` or disables `0` parsing and generation
 *                  of PUBX (uBlox) TIME messages.
 *
 * \note            TIME messages can be used to obtain:
 *                      - UTC time of week
 *                      - UTC week number
 *                      - Leap seconds (allows conversion to eg. TAI)
 *
 *                  This is a nonstandard ublox-specific extension,
 *                  so disabled by default.
 *
 *                  This configure option requires GPS_CFG_STATEMENT_PUBX
 */
#ifndef GPS_CFG_STATEMENT_PUBX_TIME
#define GPS_CFG_STATEMENT_PUBX_TIME     0
#endif
/* Guard against accidental parser breakage */
#if GPS_CFG_STATEMENT_PUBX_TIME && !GPS_CFG_STATEMENT_PUBX
#error GPS_CFG_STATEMENT_PUBX must be enabled when enabling GPS_CFG_STATEMENT_PUBX_TIME
#endif /* GPS_CFG_STATEMENT_PUBX_TIME && !GPS_CFG_STATEMENT_PUBX */

/**
 * \}
 */

/**
 * \defgroup        GPS_NMEA API functions
 * \brief           Platform independent GPS NMEA parser
 * \{
 */

/**
 * \brief           GPS float definition, can be either `float` or `double`
 * \note            Check for \ref GPS_CFG_DOUBLE configuration
 */
#if GPS_CFG_DOUBLE || __DOXYGEN__
typedef double gps_float_t;
#else
typedef float gps_float_t;
#endif

/**
 * \brief           Satellite descriptor
 */
typedef struct {
    uint8_t num;                                /*!< Satellite number */
    uint8_t elevation;                          /*!< Elevation value */
    uint16_t azimuth;                           /*!< Azimuth in degrees */
    uint8_t snr;                                /*!< Signal-to-noise ratio */
} gps_sat_t;

/**
 * \brief           ENUM of possible GPS statements parsed
 */
typedef enum {
    STAT_UNKNOWN    = 0,                        /*!< Unknown NMEA statement */
    STAT_GGA        = 1,                        /*!< GPGGA statement */
    STAT_GSA        = 2,                        /*!< GPGSA statement */
    STAT_GSV        = 3,                        /*!< GPGSV statement */
    STAT_RMC        = 4,                        /*!< GPRMC statement */
    STAT_UBX        = 5,                        /*!< UBX statement (uBlox specific) */
    STAT_UBX_TIME   = 6,                        /*!< UBX TIME statement (uBlox specific) */
    STAT_CHECKSUM_FAIL = UINT8_MAX              /*!< Special case, used when checksum fails */
} gps_statement_t;

/**
 * \brief           GPS main structure
 */
typedef struct {
#if GPS_CFG_STATEMENT_GPGGA || __DOXYGEN__
    /* Information related to GPGGA statement */
    gps_float_t latitude;                       /*!< Latitude in units of degrees */
    gps_float_t longitude;                      /*!< Longitude in units of degrees */
    gps_float_t altitude;                       /*!< Altitude in units of meters */
    gps_float_t geo_sep;                        /*!< Geoid separation in units of meters */
    uint8_t sats_in_use;                        /*!< Number of satellites in use */
    uint8_t fix;                                /*!< Fix status. `0` = invalid, `1` = GPS fix, `2` = DGPS fix, `3` = PPS fix */
    uint8_t hours;                              /*!< Hours in UTC */
    uint8_t minutes;                            /*!< Minutes in UTC */
    uint8_t seconds;                            /*!< Seconds in UTC */
#endif /* GPS_CFG_STATEMENT_GPGGA || __DOXYGEN__ */

#if GPS_CFG_STATEMENT_GPGSA || __DOXYGEN__
    /* Information related to GPGSA statement */
    gps_float_t dop_h;                          /*!< Dolution of precision, horizontal */
    gps_float_t dop_v;                          /*!< Dolution of precision, vertical */
    gps_float_t dop_p;                          /*!< Dolution of precision, position */
    uint8_t fix_mode;                           /*!< Fix mode. `1` = NO fix, `2` = 2D fix, `3` = 3D fix */
    uint8_t satellites_ids[12];                 /*!< List of satellite IDs in use. Valid range is `0` to `sats_in_use` */
#endif /* GPS_CFG_STATEMENT_GPGSA || __DOXYGEN__ */

#if GPS_CFG_STATEMENT_GPGSV || __DOXYGEN__
    /* Information related to GPGSV statement */
    uint8_t sats_in_view;                       /*!< Number of satellites in view */
#if GPS_CFG_STATEMENT_GPGSV_SAT_DET || __DOXYGEN__
    gps_sat_t sats_in_view_desc[12];
#endif
#endif /* GPS_CFG_STATEMENT_GPGSV || __DOXYGEN__ */

#if GPS_CFG_STATEMENT_GPRMC || __DOXYGEN__
    /* Information related to GPRMC statement */
    uint8_t is_valid;                           /*!< GPS valid status */
    gps_float_t speed;                          /*!< Ground speed in knots */
    gps_float_t course;                         /*!< Ground coarse */
    gps_float_t variation;                      /*!< Magnetic variation */
    uint8_t date;                               /*!< Fix date */
    uint8_t month;                              /*!< Fix month */
    uint8_t year;                               /*!< Fix year */
#endif /* GPS_CFG_STATEMENT_GPRMC || __DOXYGEN__ */

#if GPS_CFG_STATEMENT_PUBX_TIME || __DOXYGEN__
#if !GPS_CFG_STATEMENT_GPGGA && !__DOXYGEN__
    /* rely on time fields from GPGGA if possible */
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
#endif /* !GPS_CFG_STATEMENT_GPGGA && !__DOXYGEN__ */
#if !GPS_CFG_STATEMENT_GPRMC && !__DOXYGEN__
    /* rely on date fields from GPRMC if possible */
    uint8_t date;
    uint8_t month;
    uint8_t year;
#endif /* !GPS_CFG_STATEMENT_GPRMC && !__DOXYGEN__ */
    /* fields only available in PUBX_TIME */
    gps_float_t utc_tow;                        /*!< UTC TimeOfWeek, eg 113851.00 */
    uint16_t utc_wk;                            /*!< UTC week number, continues beyond 1023 */
    uint8_t leap_sec;                           /*!< UTC leap seconds; UTC + leap_sec = TAI */
    uint32_t clk_bias;                          /*!< Receiver clock bias, eg 1930035 */
    gps_float_t clk_drift;                      /*!< Receiver clock drift, eg -2660.664 */
    uint32_t tp_gran;                           /*!< Time pulse granularity, eg 43 */
#endif /* GPS_CFG_STATEMENT_PUBX_TIME || __DOXYGEN__ */

#if !__DOXYGEN__
    struct {
        gps_statement_t stat;                   /*!< Statement index */
        char term_str[13];                      /*!< Current term in string format */
        uint8_t term_pos;                       /*!< Current index position in term */
        uint8_t term_num;                       /*!< Current term number */

        uint8_t star;                           /*!< Star detected flag */

        uint8_t crc_calc;                       /*!< Calculated CRC string */

        union {
            uint8_t dummy;                      /*!< Dummy byte */
#if GPS_CFG_STATEMENT_GPGGA
            struct {
                gps_float_t latitude;           /*!< GPS latitude position in degrees */
                gps_float_t longitude;          /*!< GPS longitude position in degrees */
                gps_float_t altitude;           /*!< GPS altitude in meters */
                gps_float_t geo_sep;            /*!< Geoid separation in units of meters */
                uint8_t sats_in_use;            /*!< Number of satellites currently in use */
                uint8_t fix;                    /*!< Type of current fix, `0` = Invalid, `1` = GPS fix, `2` = Differential GPS fix */
                uint8_t hours;                  /*!< Current UTC hours */
                uint8_t minutes;                /*!< Current UTC minutes */
                uint8_t seconds;                /*!< Current UTC seconds */
            } gga;                              /*!< GPGGA message */
#endif /* GPS_CFG_STATEMENT_GPGGA */
#if GPS_CFG_STATEMENT_GPGSA
            struct {
                gps_float_t dop_h;              /*!< Horizontal dilution of precision */
                gps_float_t dop_v;              /*!< Vertical dilution of precision */
                gps_float_t dop_p;              /*!< Position dilution of precision */
                uint8_t fix_mode;               /*!< Fix mode, `1` = No fix, `2` = 2D fix, `3` = 3D fix */
                uint8_t satellites_ids[12];     /*!< IDs of satellites currently in use */
            } gsa;                              /*!< GPGSA message */
#endif /* GPS_CFG_STATEMENT_GPGSA */
#if GPS_CFG_STATEMENT_GPGSV
            struct {
                uint8_t sats_in_view;           /*!< Number of stallites in view */
                uint8_t stat_num;               /*!< Satellite line number during parsing GPGSV data */
            } gsv;                              /*!< GPGSV message */
#endif /* GPS_CFG_STATEMENT_GPGSV */
#if GPS_CFG_STATEMENT_GPRMC
            struct {
                uint8_t is_valid;               /*!< Status whether GPS status is valid or not */
                uint8_t date;                   /*!< Current UTC date */
                uint8_t month;                  /*!< Current UTC month */
                uint8_t year;                   /*!< Current UTC year */
                gps_float_t speed;              /*!< Current spead over the ground in knots */
                gps_float_t course;             /*!< Current course over ground */
                gps_float_t variation;          /*!< Current magnetic variation in degrees */
            } rmc;                              /*!< GPRMC message */
#endif /* GPS_CFG_STATEMENT_GPRMC */
#if GPS_CFG_STATEMENT_PUBX_TIME
            struct {
                uint8_t hours;                  /*!< Current UTC hours */
                uint8_t minutes;                /*!< Current UTC minutes */
                uint8_t seconds;                /*!< Current UTC seconds */
                uint8_t date;                   /*!< Current UTC date */
                uint8_t month;                  /*!< Current UTC month */
                uint8_t year;                   /*!< Current UTC year */
                gps_float_t utc_tow;            /*!< UTC TimeOfWeek, eg 113851.00 */
                uint16_t utc_wk;                /*!< UTC week number, continues beyond 1023 */
                uint8_t leap_sec;               /*!< UTC leap seconds; UTC + leap_sec = TAI */
                uint32_t clk_bias;              /*!< Receiver clock bias, eg 1930035 */
                gps_float_t clk_drift;          /*!< Receiver clock drift, eg -2660.664 */
                uint32_t tp_gran;               /*!< Time pulse granularity, eg 43 */
            } time;                             /*!< PUBX TIME message */
#endif /* GPS_CFG_STATEMENT_PUBX_TIME */
        } data;                                 /*!< Union with data for each information */
    } p;                                        /*!< Structure with private data */
#endif /* !__DOXYGEN__ */
} gps_t;

/**
 * \brief           List of optional speed transformation from GPS values (in knots)
 */
typedef enum {
    /* Metric values */
    gps_speed_kps,                              /*!< Kilometers per second */
    gps_speed_kph,                              /*!< Kilometers per hour */
    gps_speed_mps,                              /*!< Meters per second */
    gps_speed_mpm,                              /*!< Meters per minute */

    /* Imperial values */
    gps_speed_mips,                             /*!< Miles per second */
    gps_speed_mph,                              /*!< Miles per hour */
    gps_speed_fps,                              /*!< Foots per second */
    gps_speed_fpm,                              /*!< Foots per minute */

    /* Optimized for runners/joggers */
    gps_speed_mpk,                              /*!< Minutes per kilometer */
    gps_speed_spk,                              /*!< Seconds per kilometer */
    gps_speed_sp100m,                           /*!< Seconds per 100 meters */
    gps_speed_mipm,                             /*!< Minutes per mile */
    gps_speed_spm,                              /*!< Seconds per mile */
    gps_speed_sp100y,                           /*!< Seconds per 100 yards */

    /* Nautical values */
    gps_speed_smph,                             /*!< Sea miles per hour */
} gps_speed_t;

/**
 * \brief           Signature for caller-suplied callback function from gps_process
 * \param[in]       res: statement type of recently parsed statement
 */
typedef void (*gps_process_fn)(gps_statement_t res);

/**
 * \brief           Check if current GPS data contain valid signal
 * \note            \ref GPS_CFG_STATEMENT_GPRMC must be enabled and `GPRMC` statement must be sent from GPS receiver
 * \param[in]       _gh: GPS handle
 * \return          `1` on success, `0` otherwise
 */
#if GPS_CFG_STATEMENT_GPRMC || __DOXYGEN__
#define gps_is_valid(_gh)           ((_gh)->is_valid)
#else
#define gps_is_valid(_gh)           (0)
#endif /* GPS_CFG_STATEMENT_GPRMC || __DOXYGEN__ */

uint8_t     gps_init(gps_t* gh);
#if GPS_CFG_STATUS || __DOXYGEN__
uint8_t     gps_process(gps_t* gh, const void* data, size_t len, gps_process_fn evt_fn);
#else /* GPS_CFG_STATUS */
uint8_t     gps_process(gps_t* gh, const void* data, size_t len);
#endif /* !GPS_CFG_STATUS */
uint8_t     gps_distance_bearing(gps_float_t las, gps_float_t los, gps_float_t lae, gps_float_t loe, gps_float_t* d, gps_float_t* b);
gps_float_t gps_to_speed(gps_float_t sik, gps_speed_t ts);

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GPS_HDR_H */
