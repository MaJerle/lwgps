.. _how_it_works:

How it works
============

LwGPS parses raw data formatted as NMEA 0183 statements from GPS receivers. It supports up to ``4`` different statements:

* ``GPGGA`` or ``GNGGA``: GPS fix data
* ``GPGSA`` or ``GNGSA``: GPS active satellites and dillusion of position
* ``GPGSV`` or ``GNGSV``: List of satellites in view zone
* ``GPRMC`` or ``GNRMC``: Recommended minimum specific GPS/Transit data

.. tip::
	By changing different configuration options, it is possible to disable some statements.
	Check :ref:`api_lwgps_opt` for more information.

Application must assure to properly receive data from GPS receiver.
Usually GPS receivers communicate with host embedded system with UART protocol and output directly formatted NMEA 0183 statements.

.. note::
	Application must take care of properly receive data from GPS.

Application must use :cpp:func:`lwgps_process` function for data processing. Function will:

* Detect statement type, such as *GPGGA* or *GPGSV*
* Parse all the terms of specific statement
* Check valid CRC after each statement

Programmer's model is as following:

* Application receives data from GPS receiver
* Application sends data to :cpp:func:`lwgps_process` function
* Application uses processed data to display altitude, latitude, longitude, and other parameters

Check :ref:`examples` for typical example

.. toctree::
    :maxdepth: 2
