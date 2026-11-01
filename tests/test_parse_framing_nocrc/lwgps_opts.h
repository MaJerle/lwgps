/**
 * \file            lwgps_opts.h
 * \brief           LwGPS configuration for statement framing test, with checksum check disabled
 */

#ifndef LWGPS_OPTS_HDR_H
#define LWGPS_OPTS_HDR_H

#define LWGPS_CFG_CRC             0
#define LWGPS_CFG_STATEMENT_GPGGA 1
#define LWGPS_CFG_STATEMENT_GPGSA 0
#define LWGPS_CFG_STATEMENT_GPGSV 0
#define LWGPS_CFG_STATEMENT_GPRMC 0

#endif /* LWGPS_OPTS_HDR_H */
