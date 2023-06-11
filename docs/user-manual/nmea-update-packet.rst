.. _nmea_packet_update:

NMEA data refresh
=================

LwGPS is designed to parse standard NMEA output from GPS module.

.. tip::
    You can read more about `NMEA 0183 here <https://en.wikipedia.org/wiki/NMEA_0183>`_.

GPS module outputs several NMEA statements periodically, for instance once a second. In rare cases, outputs can be even every `100ms`.
The common *problem* we try to solve is what happens if application tries to access GPS parsed data, while library processed only part of 
new NMEA statement.

Depending on the application requirements, it is necessary to make sure data used by the application are all from the single NMEA output packet,
and not split between different ones. Below are ``2`` examples of several statements GPS module will output every second.

First statement at any given time:
``$GPRMC,183729,A,3907.356,N,12102.482,W,000.0,360.0,080301,015.5,E*6F``
``$GPGGA,183730,3907.356,N,12102.482,W,1,05,1.6,646.4,M,-24.1,M,,*75``
``$GPGSA,A,3,02,,,07,,09,24,26,,,,,1.6,1.6,1.0*3D``
``$GPGSV,2,1,08,02,43,088,38,04,42,145,00,05,11,291,00,07,60,043,35*71``
``$GPGSV,2,2,08,08,02,145,00,09,46,303,47,24,16,178,32,26,18,231,43*77``

New statement after one second:
``$GPRMC,183729,A,3907.356,N,12102.482,W,000.0,360.0,080301,015.5,E*6F``
``$GPGGA,183730,3907.356,N,12102.482,W,1,05,1.6,646.4,M,-24.1,M,,*75``
``$GPGSA,A,3,02,,,07,,09,24,26,,,,,1.6,1.6,1.0*3D``
``$GPGSV,2,1,08,02,43,088,38,04,42,145,00,05,11,291,00,07,60,043,35*71``
``$GPGSV,2,2,08,08,02,145,00,09,46,303,47,24,16,178,32,26,18,231,43*77``

If application manages to check GPS parsed data after first packet has been processed and second didn't arrive yet, there is no issue.
Application parsed data are all belonging to single packet, at specific time.

But what would happen if application starts using GPS data while ``GPGGA`` packet is being received for second time?

* Application has new ``GPRMC`` information, from new packet
* Application still keeps ``GPGGA``, ``GPGSA`` and ``GPGSV`` data from old packets

This could be a major issue for some applications. Time, speed and position do not match anymore.

Common approach
***************

A common approach to this is to have a source of time in the application.
A set of timeouts could determine if packet has just started, or has just been completed and is now fully filled with new data.

An algorithm would be, assuming GPS sends packet data every ``1`` second:

* When character comes, if time of previous character is greater than maximum time between ``2`` characters (let's say ``10ms``, even if this is a lot), this is probably start of new packet.
* If new time is ``>10ms`` since last received character, it was probably the last character.
* Application can now use new data
* Application goes to *wait new packet mode*
* Go back to step nr.1

.. toctree::
    :maxdepth: 2
