# Changelog

## Develop

- Split CMakeLists.txt files between library and executable
- Change license year to 2022
- Add `.clang-format` draft
- Deprecate lowercase `lwgps_speed_xxx` enumeration. Temporary implement macro to keep backward compatibility. Will be removed in next major release

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

