set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR powerpc-eabi)

set(DEVKITPPC $ENV{DEVKITPPC})
set(DEVKITPRO $ENV{DEVKITPRO})

set(CMAKE_C_COMPILER   ${DEVKITPPC}/bin/powerpc-eabi-gcc)
set(CMAKE_CXX_COMPILER ${DEVKITPPC}/bin/powerpc-eabi-g++)
set(CMAKE_C_FLAGS      "-MMD -MP -mrvl -mcpu=750 -meabi -mhard-float")
set(CMAKE_CXX_FLAGS    "-MMD -MP -mrvl -mcpu=750 -meabi -mhard-float")

set(MARSHMALLOW_WII true)

set(CMAKE_FIND_ROOT_PATH ${DEVKITPPC} ${DEVKITPRO} ${DEVKITPPC}/powerpc-eabi ${DEVKITPRO}/libogc)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

include_directories(${DEVKITPRO}/libogc/include)
link_directories(${DEVKITPRO}/libogc/lib/wii)

add_definitions(-DGEKKO)

