# - Find soci - C++ backend for databases 
# Find the SOCI includes and library
#
#  SOCI_INCLUDE_DIR - where to find mysql.h, etc.
#  SOCI_LIBRARIES   - List of libraries when using Neko.
#  SOCI_FOUND       - True if Neko found.

IF (SOCI_INCLUDE_DIR)
  # Already in cache, be silent
  SET(SOCI_FIND_QUIETLY TRUE)
ENDIF (SOCI_INCLUDE_DIR)

FIND_PATH(SOCI_INCLUDE_DIR 
  NAMES	soci.h
  PATHS /usr/local/include/soci 
		/usr/include/soci 
)

# Finally the library itself
find_library(SOCI_LIBRARY
  NAMES soci_core 
  PATHS /usr/lib
		/usr/local/lib32
		/usr/local/lib64
)

SET(SOCI_NAMES soci)

IF (SOCI_INCLUDE_DIR AND SOCI_LIBRARY)
  SET(SOCI_FOUND TRUE)
  SET( SOCI_LIBRARIES ${SOCI_LIBRARY} )
ELSE (SOCI_INCLUDE_DIR AND SOCI_LIBRARY)
  SET(SOCI_FOUND FALSE)
  SET( SOCI_LIBRARIES )
ENDIF (SOCI_INCLUDE_DIR AND SOCI_LIBRARY)

# Just display a message about success or fail.
IF (SOCI_FOUND)
#  IF (NOT SOCI_FIND_QUIETLY)
    MESSAGE(STATUS "Found Soci: ${SOCI_LIBRARY}")
#  ENDIF (NOT SOCI_FIND_QUIETLY)
ELSE (SOCI_FOUND)
  IF (SOCI_FIND_REQUIRED)
    MESSAGE(STATUS "Looked for Soci libraries named ${SOCI_NAMES}.")
    MESSAGE(FATAL_ERROR "Could NOT find soci library")
  ENDIF (SOCI_FIND_REQUIRED)
ENDIF (SOCI_FOUND)

MARK_AS_ADVANCED(
  SOCI_LIBRARY
  SOCI_INCLUDE_DIR
 )
