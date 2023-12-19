cmake_minimum_required(VERSION 3.3)

project(lunasvg VERSION 2.3.8 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

option(BUILD_SHARED_LIBS "Build as a shared library" OFF)
option(LUNASVG_BUILD_EXAMPLES "Build example(s)" OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FREETYPE REQUIRED freetype2)

add_library(lunasvg)

target_include_directories(lunasvg SYSTEM PUBLIC ${FREETYPE_INCLUDE_DIRS})

target_link_libraries(lunasvg PUBLIC ${FREETYPE_LIBRARIES})

add_subdirectory(include)
add_subdirectory(source)
add_subdirectory(3rdparty/plutovg)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(lunasvg PUBLIC LUNASVG_SHARED)
    target_compile_definitions(lunasvg PRIVATE LUNASVG_EXPORT)
endif()

if(LUNASVG_BUILD_EXAMPLES)
    add_executable(svg2png svg2png.cpp)
    target_link_libraries(svg2png PRIVATE lunasvg)
    target_include_directories(svg2png PRIVATE 3rdparty/stb)
endif()

set(LUNASVG_LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
set(LUNASVG_INCDIR ${CMAKE_INSTALL_PREFIX}/include)

install(FILES
    include/lunasvg.h
    DESTINATION ${LUNASVG_INCDIR}
)

install(TARGETS lunasvg
    LIBRARY     DESTINATION    ${LUNASVG_LIBDIR}
    ARCHIVE     DESTINATION    ${LUNASVG_LIBDIR}
    INCLUDES    DESTINATION    ${LUNASVG_INCDIR}
)
