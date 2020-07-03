.. _float_double:

Float/double precision
======================

With configuration of ``GSM_CFG_DOUBLE``, it is possible to enable ``double`` floating point precision.
All floating point variables are then configured in *double precision*.

When configuration is set to ``0``, floating point variables are configured in *single precision* format.

.. note::
	Single precision uses less memory in application. As a drawback, application may be a subject of data loss at latter digits.

.. toctree::
    :maxdepth: 2