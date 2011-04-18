# - Find zlib
# Find the native ZLIB includes and library
#
#  ZLIB_INCLUDE_DIR - where to find zlib.h, etc.
#  ZLIB_LIBRARIES   - List of libraries when using zlib.
#  ZLIB_FOUND       - True if zlib found.
#
#  Modified by Nicolas Schlumberger, (c) 2007
#  to make it work on the Tardis Infrastucture of the ETH Zurich
#
IF(UNIX)
  IF (ZLIB_INCLUDE_DIR)
  # Already in cache, be silent
    SET(ZLIB_FIND_QUIETLY TRUE)
  ENDIF (ZLIB_INCLUDE_DIR)
  FIND_PATH(ZLIB_INCLUDE_DIR zlib.h
    /usr/pack/zziplib-0.13.49-sd/include/
    /usr/local/include
    /usr/include
  )
  SET(ZLIB_NAMES z zlib zdll)
  FIND_LIBRARY(ZLIB_LIBRARY
    NAMES ${ZLIB_NAMES}
    PATHS /usr/pack/zziplib-0.13.49-sd/i686-debian-linux3.1/lib/ /usr/lib /usr/local/lib
  )
  IF (ZLIB_INCLUDE_DIR AND ZLIB_LIBRARY)
    SET(ZLIB_FOUND TRUE)
      SET( ZLIB_LIBRARIES ${ZLIB_LIBRARY} )
  ELSE (ZLIB_INCLUDE_DIR AND ZLIB_LIBRARY)
    SET(ZLIB_FOUND FALSE)
    SET( ZLIB_LIBRARIES )
  ENDIF (ZLIB_INCLUDE_DIR AND ZLIB_LIBRARY)
  IF (ZLIB_FOUND)
    IF (NOT ZLIB_FIND_QUIETLY)
      MESSAGE(STATUS "Found ZLIB: ${ZLIB_LIBRARY}")
    ENDIF (NOT ZLIB_FIND_QUIETLY)
  ELSE (ZLIB_FOUND)
    IF (ZLIB_FIND_REQUIRED)
        MESSAGE(STATUS "Looked for Z libraries named ${ZLIBS_NAMES}.")
        MESSAGE(FATAL_ERROR "Could NOT find z library")
    ENDIF (ZLIB_FIND_REQUIRED)
  ENDIF (ZLIB_FOUND)
  MARK_AS_ADVANCED(
    ZLIB_LIBRARY
    ZLIB_INCLUDE_DIR
  )
ENDIF(UNIX)
IF(WIN32)
  IF(EXISTS "../libs/ogre/Dependencies/lib/Release")
    SET (ZLIB_LIBRARY_DIR "../libs/ogre/Dependencies/lib/Release")
  ENDIF(EXISTS "../libs/ogre/Dependencies/lib/Release")
  FIND_PATH(ZLIB_INCLUDE_DIR zlib.h
    ../libs/ogre/Dependencies/include
  )
  FIND_LIBRARY(ZLIB_LIBRARY
    NAMES zlib
    PATHS ${ZLIB_LIBRARY_DIR}
  )
  IF(ZLIB_LIBRARY AND ZLIB_INCLUDE_DIR)
    MESSAGE(STATUS "Zlib was found.")
  ELSE(ZLIB_LIBRARY AND ZLIB_INCLUDE_DIR)
    MESSAGE(FATAL_ERROR "Zlib was NOT found.")
  ENDIF(ZLIB_LIBRARY AND ZLIB_INCLUDE_DIR)
ENDIF(WIN32)
