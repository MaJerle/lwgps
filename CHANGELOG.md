# Changelog

## Develop

## v2.4.0

- Fix `LWESP_CFG_DISTANCE_BEARING` misnamed config option, renamed to `LWGPS_CFG_DISTANCE_BEARING`
- Add `GCC-Debug` CMake preset for native builds
- Guard `lwgps_distance_bearing` declaration with `LWGPS_CFG_DISTANCE_BEARING` to match its definition
- Fix GSV cycle boundary detection so a single corrupted non-GSV sentence can no longer leave stale satellite data from the previous epoch mixed into the next one

## v2.3.0

- Add support for differential GPS last time
- Rework library CMake with removed INTERFACE type
- Add `time_valid` and `date_valid` to give information if information from modem is valid
- Fix the platformio library package description
- Improve the satellite management

## v2.2.0

- Split `CMakeLists.txt` files between library and executable
- Change license year to `2023`
- Add `.clang-format` draft
- Deprecate lowercase `lwgps_speed_xxx` enumeration. Temporary implement macro to keep backward compatibility. Will be removed in next major release
- Improve `C++` port

## v2.1.0

- Add configuration settings to be consistend with other LwXX libraries
- Apply code style settings with Artistic style options

## v2.0.0

- Break compatibility with v1.x
- Function prefix set to `lwgps_`
- Macros prefix set to `LWGPS_`
- Added support for PUBX Ublox statement

## v1.1.0

- Use pre-increment instead of post-increment
- Remove buffer library and propose ringbuff instead
- Other code style enhancements

## v1.0.0

- Initial release

