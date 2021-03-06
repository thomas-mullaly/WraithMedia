# - Try to find GStreamer
# Once done this will define
#
#  GSTREAMER_FOUND - system has GStreamer
#  GSTREAMER_INCLUDE_DIR - the GStreamer include directory
#  GSTREAMER_LIBRARIES - the libraries needed to use GStreamer
#  GSTREAMER_DEFINITIONS - Compiler switches required for using GStreamer
#
#  (c)2006, Tim Beaulen <tbscope@gmail.com>

# TODO: Other versions --> GSTREAMER_X_Y_FOUND (Example: GSTREAMER_0_8_FOUND and GSTREAMER_0_10_FOUND etc)

IF (GSTREAMER_INCLUDE_DIR AND GSTREAMER_LIBRARIES AND GSTREAMER_INTERFACE_LIBRARY)
   # in cache already
   SET(GStreamer_FIND_QUIETLY TRUE)
ELSE (GSTREAMER_INCLUDE_DIR AND GSTREAMER_LIBRARIES AND GSTREAMER_INTERFACE_LIBRARY)
   SET(GStreamer_FIND_QUIETLY FALSE)
ENDIF (GSTREAMER_INCLUDE_DIR AND GSTREAMER_LIBRARIES AND GSTREAMER_INTERFACE_LIBRARY)

IF (NOT WIN32)
   # use pkg-config to get the directories and then use these values
   # in the FIND_PATH() and FIND_LIBRARY() calls
   INCLUDE(UsePkgConfig)
   PKGCONFIG(gstreamer-0.10 _GStreamerIncDir _GStreamerLinkDir _GStreamerLinkFlags _GStreamerCflags)
   #MESSAGE(STATUS "DEBUG: GStreamer include directory = ${_GStreamerIncDir}")
   #MESSAGE(STATUS "DEBUG: GStreamer link directory = ${_GStreamerLinkDir}")
   #MESSAGE(STATUS "DEBUG: GStreamer link flags = ${_GStreamerLinkFlags}")
   #MESSAGE(STATUS "DEBUG: GStreamer CFlags = ${_GStreamerCflags}") 
   SET(GSTREAMER_DEFINITIONS ${_GStreamerCflags})
ENDIF (NOT WIN32)

FIND_PATH(GSTREAMER_INCLUDE_DIR gst/gst.h
   PATHS
   ${_GStreamerIncDir}
   #PATH_SUFFIXES gst
   )

FIND_LIBRARY(GSTREAMER_LIBRARIES NAMES gstreamer-0.10
   PATHS
   ${_GStreamerLinkDir}
   )

FIND_LIBRARY(GSTREAMER_INTERFACE_LIBRARY NAMES gstinterfaces-0.10
   PATHS
   ${_GStreamerLinkDir}
   )

IF (GSTREAMER_INCLUDE_DIR)
   #MESSAGE(STATUS "DEBUG: Found GStreamer include dir: ${GSTREAMER_INCLUDE_DIR}")
ELSE (GSTREAMER_INCLUDE_DIR)
   MESSAGE(STATUS "GStreamer: WARNING: include dir not found")
ENDIF (GSTREAMER_INCLUDE_DIR)

IF (GSTREAMER_LIBRARIES)
   #MESSAGE(STATUS "DEBUG: Found GStreamer library: ${GSTREAMER_LIBRARIES}")
ELSE (GSTREAMER_LIBRARIES)
   MESSAGE(STATUS "GStreamer: WARNING: library not found")
ENDIF (GSTREAMER_LIBRARIES)

IF (GSTREAMER_INTERFACE_LIBRARY)
   #MESSAGE(STATUS "DEBUG: Found GStreamer interface library: ${GSTREAMER_INTERFACE_LIBRARY}")
ELSE (GSTREAMER_INTERFACE_LIBRARY)
   MESSAGE(STATUS "GStreamer: WARNING: interface library not found")
ENDIF (GSTREAMER_INTERFACE_LIBRARY)

IF (GSTREAMER_INCLUDE_DIR AND GSTREAMER_LIBRARIES AND GSTREAMER_INTERFACE_LIBRARY)
   SET(GSTREAMER_FOUND TRUE)
ELSE (GSTREAMER_INCLUDE_DIR AND GSTREAMER_LIBRARIES AND GSTREAMER_INTERFACE_LIBRARY)
   SET(GSTREAMER_FOUND FALSE)
ENDIF (GSTREAMER_INCLUDE_DIR AND GSTREAMER_LIBRARIES AND GSTREAMER_INTERFACE_LIBRARY)

IF (GSTREAMER_FOUND)
   IF (NOT GStreamer_FIND_QUIETLY)
      MESSAGE(STATUS "Found GStreamer: ${GSTREAMER_LIBRARIES}")
   ENDIF (NOT GStreamer_FIND_QUIETLY)
ELSE (GSTREAMER_FOUND)
   IF (GStreamer_FIND_REQUIRED)
      MESSAGE(SEND_ERROR "Could NOT find GStreamer")
   ENDIF (GStreamer_FIND_REQUIRED)
ENDIF (GSTREAMER_FOUND)

MARK_AS_ADVANCED(GSTREAMER_INCLUDE_DIR GSTREAMER_LIBRARIES GSTREAMER_INTERFACE_LIBRARY)
