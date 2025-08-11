cmake_minimum_required(VERSION 3.15)

project(halsim_server VERSION ${HAL_VERSION} LANGUAGES CXX)

# HALSIM SERVER
add_executable(
    halsim_server
    server/main.cpp)

target_link_libraries(
    halsim_server PRIVATE
    bfc::bfc
    cum::cum
    logless::logless)
        
# HALSIM / CLIENT
add_library(hal STATIC
    client/sim_spi.cpp)
        
target_include_directories(hal PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/hal>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/halsim>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(
    hal PRIVATE
    bfc::bfc
    cum::cum
    logless::logless)
                
# install(
#     TARGETS hal
#     EXPORT halTargets
#     LIBRARY DESTINATION lib
#     ARCHIVE DESTINATION lib
#     RUNTIME DESTINATION bin)

# install(
#     FILES
#         ${CMAKE_SOURCE_DIR}/hal/hal/adc.hpp
#         ${CMAKE_SOURCE_DIR}/hal/hal/gpio.hpp
#         ${CMAKE_SOURCE_DIR}/hal/hal/i2c.hpp
#         ${CMAKE_SOURCE_DIR}/hal/hal/pwm.hpp
#         ${CMAKE_SOURCE_DIR}/hal/hal/spi.hpp
#         ${CMAKE_SOURCE_DIR}/hal/hal/uart.hpp
#     DESTINATION include/hal)

# install(
#     EXPORT halTargets
#     FILE halTargets.cmake
#     NAMESPACE hal::
#     DESTINATION lib/cmake/hal)

# include(CMakePackageConfigHelpers)
# configure_package_config_file(
#     Config.cmake.in
#     ${CMAKE_CURRENT_BINARY_DIR}/halConfig.cmake
#     INSTALL_DESTINATION lib/cmake/hal)

# write_basic_package_version_file(
#     ${CMAKE_CURRENT_BINARY_DIR}/halConfigVersion.cmake
#     VERSION ${PROJECT_VERSION}
#     COMPATIBILITY AnyNewerVersion)

# install(
#     FILES
#     ${CMAKE_CURRENT_BINARY_DIR}/halConfig.cmake
#     ${CMAKE_CURRENT_BINARY_DIR}/halConfigVersion.cmake
#     DESTINATION lib/cmake/hal)
