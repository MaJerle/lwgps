/**
 * \file            lwgps.hpp
 * \brief           C++ wrapper for LwGPS
 */

/*
 * Copyright (c) 2023 Tilen MAJERLE
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
 * This file is part of LwGPS - Lightweight GPS NMEA parser library.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 * Version:         v2.2.0
 */
#ifndef LWGPS_HDR_HPP
#define LWGPS_HDR_HPP

#include "lwgps/lwgps.h"

namespace Lwgps {
class Lwgps {
  private:
    lwgps_t m_hgps;

#if LWGPS_CFG_STATUS || __DOXYGEN__
    lwgps_process_fn m_procfn;
#endif /* LWGPS_CFG_STATUS || __DOXYGEN__ */

  public:
    Lwgps(const Lwgps& other) = delete;            /* No copy constructor */
    Lwgps& operator=(const Lwgps& other) = delete; /* No copy assignment */
    Lwgps(const Lwgps&& other) = delete;           /* No move constructor */
    Lwgps& operator=(Lwgps&& other) = delete;      /* No move assignment */

    Lwgps() { /* Constructor */
        lwgps_init(&m_hgps);
#if LWGPS_CFG_STATUS
        m_procfn = nullptr;
#endif /* LWGPS_CFG_STATUS */
    }

#if LWGPS_CFG_STATUS || __DOXYGEN__
    /**
     * \brief           Set processing callback function
     * 
     * \param           procfn 
     */
    void
    set_process_fn(lwgps_process_fn procfn) {
        this->m_procfn = procfn;
    }
#endif /* LWGPS_CFG_STATUS || __DOXYGEN__ */

    /**
     * \brief           Process NMEA data from GPS receiver
     * \param[in]       data: Received data
     * \param[in]       len: Number of bytes to process
     * \return          `1` on success, `0` otherwise
     */
    uint8_t
    process(const void* data, size_t len) {
        return lwgps_process(&m_hgps, data, len
#if LWGPS_CFG_STATUS
                             ,
                             m_procfn
#endif /* LWGPS_CFG_STATUS */
        );
    }

#if LWESP_CFG_DISTANCE_BEARING || __DOXYGEN__

    /**
     * \brief           Calculate distance and bearing between `2` latitude and longitude coordinates
     * \param[in]       las: Latitude start coordinate, in units of degrees
     * \param[in]       los: Longitude start coordinate, in units of degrees
     * \param[in]       lae: Latitude end coordinate, in units of degrees
     * \param[in]       loe: Longitude end coordinate, in units of degrees
     * \param[out]      d: Pointer to output distance in units of meters
     * \param[out]      b: Pointer to output bearing between start and end coordinate in relation to north in units of degrees
     * \return          `1` on success, `0` otherwise
     */
    static uint8_t
    distance_bearing(lwgps_float_t las, lwgps_float_t los, lwgps_float_t lae, lwgps_float_t loe, lwgps_float_t* d,
                     lwgps_float_t* b) {
        return lwgps_distance_bearing(las, los, lae, loe, d, b);
    }
#endif /* LWESP_CFG_DISTANCE_BEARING || __DOXYGEN__ */

    /**
     * \brief           Convert NMEA GPS speed (in knots = nautical mile per hour) to different speed format
     * \param[in]       sik: Speed in knots, received from GPS NMEA statement
     * \param[in]       ts: Target speed to convert to from knots
     * \return          Speed calculated from knots
     */
    static lwgps_float_t
    to_speed(lwgps_float_t sik, lwgps_speed_t ts) {
        return lwgps_to_speed(sik, ts);
    }

    ~Lwgps() { /* Destructor */
    }
};
}; // namespace Lwgps

#endif /* LWGPS_HDR_HPP */
