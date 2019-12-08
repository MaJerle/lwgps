.. _thread_safety:

Thread safety
=============

Library tends to be as simple as possible.
No specific features have been implemented for thread safety.

When library is using multi-thread environment and if multi threads tend to access to shared resources,
user must resolve it with care, using mutual exclusion.

.. tip::
	When single thread is dedicated for GPS processing, no special mutual exclusion is necessary.

.. toctree::
    :maxdepth: 2