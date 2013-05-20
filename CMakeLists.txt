#          Copyright Markus Mohr 2013.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required (VERSION 2.6) 

################################################################################
#                        Variables and definitions                             #
################################################################################
project(MCMparallels)

add_definitions( -O3 )
add_definitions( -ffast-math )
add_definitions( -fno-math-errno )

set(EXTERNAL_LIBS
    pthread
    boost-thread
    )

#3rd party libraries
set(cli_LIBRARIES )

#add source files
include_directories(include)
add_subdirectory(src)

set_directory_properties (
    PROPERTIES
    ADDITIONAL_MAKE_CLEAN_FILES
    "MCMparallels;"
)

################################################################################
#                                 Artifacts                                    #
################################################################################

add_library(MCMparallels STATIC ${SOURCES})
target_link_libraries(MCMparallels ${EXTERNAL_LIBS})


set (LIBRARY ${CMAKE_BINARY_DIR}/libMCMparallels.a)
set (HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent_execute.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SimpleRoundRobinScheduler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Task.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/TaskManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/TaskQueue.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/TaskScheduler.h
)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/MCMparallels/)
INSTALL_FILES(/lib FILES ${LIBRARY})
INSTALL_FILES(/include/MCMparallels FILES ${HEADERS} )

SET(CPACK_GENERATOR "TBZ2")
INCLUDE(CPack)
