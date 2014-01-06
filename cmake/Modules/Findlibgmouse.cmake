# Based on Findlibusb-1.0.cmake
# Once done this will define
#
#  LIBGMOUSE_FOUND - system has libusb
#  LIBGMOUSE_INCLUDE_DIRS - the libusb include directory
#  LIBGMOUSE_LIBRARIES - Link these to use libusb
#  LIBGMOUSE_DEFINITIONS - Compiler switches required for using libusb
#
#  Adapted from cmake-modules Google Code project
#
#  Copyright (c) 2006 Andreas Schneider <mail@cynapses.org>
#
#  (Changes for libusb) Copyright (c) 2008 Kyle Machulis <kyle@nonpolynomial.com>
#
#  (Changes for libgmouse) Copyright (c) 2014 Tomas Poledny <saljacky@gmail.com>
#
# Redistribution and use is allowed according to the terms of the New BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#


if (LIBGMOUSE_LIBRARIES AND LIBGMOUSE_INCLUDE_DIRS)
  # in cache already
  set(LIBUSB_FOUND TRUE)
else (LIBGMOUSE_LIBRARIES AND LIBGMOUSE_INCLUDE_DIRS)
  find_path(LIBGMOUSE_INCLUDE_DIR
    NAMES
	libgmouse
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
	PATH_SUFFIXES
	  libgmouse
  )

  find_library(LIBGMOUSE_LIBRARY
    NAMES
      gmouse
    PATHS
      /usr/lib/
      /usr/local/lib/
      /opt/local/lib/
      /sw/lib/
  )

  set(LIBGMOUSE_INCLUDE_DIRS
    ${LIBGMOUSE_INCLUDE_DIR}
  )
  set(LIBGMOUSE_LIBRARIES
    ${LIBGMOUSE_LIBRARY}
)

  if (LIBGMOUSE_INCLUDE_DIRS AND LIBGMOUSE_LIBRARIES)
     set(LIBGMOUSE_FOUND TRUE)
  endif (LIBGMOUSE_INCLUDE_DIRS AND LIBGMOUSE_LIBRARIES)

  if (LIBGMOUSE_FOUND)
    if (NOT gmouse_FIND_QUIETLY)
      message(STATUS "Found libgmouse:")
	  message(STATUS " - Includes: ${LIBGMOUSE_INCLUDE_DIRS}")
	  message(STATUS " - Libraries: ${LIBGMOUSE_LIBRARIES}")
    endif (NOT gmouse_FIND_QUIETLY)
  else (LIBGMOUSE_FOUND)
    if (gmouse_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find libgmouse")
    endif (gmouse_FIND_REQUIRED)
  endif (LIBGMOUSE_FOUND)

  # show the LIBGMOUSE_INCLUDE_DIRS and LIBGMOUSE_LIBRARIES variables only in the advanced view
  mark_as_advanced(LIBGMOUSE_INCLUDE_DIRS LIBGMOUSE_LIBRARIES)

endif (LIBGMOUSE_LIBRARIES AND LIBGMOUSE_INCLUDE_DIRS)