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
 * Version:         v2.1.0
 */
#ifndef LWGPS_HDR_HPP
#define LWGPS_HDR_HPP

#include "lwgps/lwgps.h"

namespace Lwgps {
class Lwgps {
  private:
    lwgps_t m_hgps;

  public:
    Lwgps(const Lwgps& other) = delete;            /* No copy constructor */
    Lwgps& operator=(const Lwgps& other) = delete; /* No copy assignment */
    Lwgps(const Lwgps&& other) = delete;           /* No move constructor */
    Lwgps& operator=(Lwgps&& other) = delete;      /* No move assignment */

    Lwgps() { /* Constructor */
        lwgps_init(&m_hgps);
    }

    ~Lwgps() { /* Destructor */
    }
};
}; // namespace Lwgps

#endif /* LWGPS_HDR_HPP */
