cmake_minimum_required (VERSION 3.14)

project (OPP_FinalProject_2nd_semester)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
if (MSVC)
    add_compile_options (/permissive-)
endif ()

add_executable (OPP_FinalProject_2nd_semester "")

add_subdirectory (include)
add_subdirectory (src)
add_subdirectory (resources)
add_subdirectory (States/src)
add_subdirectory (States/include)
add_subdirectory (resources/players)
add_subdirectory (resources/archers)
add_subdirectory (resources/fallingBlocks)
add_subdirectory (resources/other)
add_subdirectory (resources/maps)
add_subdirectory (external/box2d/src)

set (SFML_STATIC_LIBRARIES TRUE)
set (SFML_DIR "C:/SFML/SFML-2.5.1/lib/cmake/SFML")

find_package (SFML 2.5 COMPONENTS graphics audio network REQUIRED)

set_target_properties (sfml-main sfml-system sfml-window sfml-graphics sfml-audio sfml-network PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)
configure_file ("C:/SFML/SFML-2.5.1/bin/openal32.dll" ${CMAKE_BINARY_DIR} COPYONLY)

target_link_libraries (OPP_FinalProject_2nd_semester sfml-graphics sfml-audio sfml-main sfml-network)
target_link_libraries (OPP_FinalProject_2nd_semester box2d)