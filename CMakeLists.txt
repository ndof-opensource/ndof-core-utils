cmake_minimum_required(VERSION 3.28)

project(ndof-core-utils
    VERSION 0.1.0
    DESCRIPTION "Foundational utilities for the ndof framework family"
    HOMEPAGE_URL "https://github.com/ndof-opensource/ndof-core-utils"
    LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# This project does not use C++20 modules; CMake 3.28+ module scanning also
# injects compiler flags that clang-tidy cannot parse.
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

option(NDOF_BUILD_TESTS "Build unit tests" ${PROJECT_IS_TOP_LEVEL})
# OFF by default so source builds by consumers never break on a newer
# compiler's warnings; the team's CMake presets and CI turn it ON.
option(NDOF_WERROR "Treat compiler warnings as errors" OFF)

add_library(core-utils
    src/strings.cpp
    src/version.cpp)
add_library(ndof::core-utils ALIAS core-utils)

set_target_properties(core-utils PROPERTIES
    OUTPUT_NAME ndof-core-utils)

target_compile_features(core-utils PUBLIC cxx_std_23)

target_include_directories(core-utils PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_definitions(core-utils PRIVATE
    NDOF_LIBRARY_NAME="ndof-core-utils"
    NDOF_LIBRARY_VERSION="${PROJECT_VERSION}")

target_compile_options(core-utils PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive- $<$<BOOL:${NDOF_WERROR}>:/WX>>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Wconversion -Wshadow $<$<BOOL:${NDOF_WERROR}>:-Werror>>)

if(NDOF_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ---- installation (plain CMake package; no package-manager assumptions) -----
install(TARGETS core-utils EXPORT ndof-core-utils-targets)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ndof-core-utils-targets
    NAMESPACE ndof::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ndof-core-utils)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ndof-core-utils-config-version.cmake
    COMPATIBILITY SameMinorVersion)

configure_package_config_file(
    cmake/config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ndof-core-utils-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ndof-core-utils)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ndof-core-utils-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ndof-core-utils-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ndof-core-utils)
