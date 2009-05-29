cmake_minimum_required(VERSION 2.6)

PROJECT(autolab-rapi)
SET( V_MAJOR 0 )
SET( V_MINOR 1 )
SET( V_BUGFIX 0 )

SET( VERSION ${V_MAJOR}.${V_MINOR}.${V_BUGFIX} )
SET( APIVERSION ${V_MAJOR}.${V_MINOR} )

# Enable graphics: use with -DWITH_GUI:BOOL=off/on
OPTION (WITH_GUI "Enable building of Rapi graphics stuff" ON)

# Enable chatterbox: use with -DWITH_CB:BOOL=off/on
OPTION (WITH_CB "Enable building of chatterbox library" ON)

# Enable stage: use with -DWITH_STAGE:BOOL=off/on
OPTION (WITH_STAGE "Enable building of state library" ON)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE( STAGE stage )
FIND_PACKAGE(Qt4)

IF(QT_FOUND)
ELSEIF(QT_FOUND)
  SET(WITH_GUI off)
ENDIF(QT_FOUND)

IF( APPLE )
  # skipping chatterbox, as it needs Linux
  SET(WITH_CB off)
ENDIF( APPLE )


ADD_DEFINITIONS(-g -Wall -O2)
ADD_SUBDIRECTORY(librapicore)

IF (WITH_STAGE)
 ADD_SUBDIRECTORY(librapistage)
ENDIF(WITH_STAGE)

if (WITH_CB)
  ADD_SUBDIRECTORY(librapichatterbox)
ENDIF(WITH_CB)

IF (WITH_GUI)
  ADD_SUBDIRECTORY(librapigui)
  ADD_DEFINITIONS(-DRAPI_GUI)
ENDIF(WITH_GUI)


ADD_SUBDIRECTORY(examples)


# Create and install the pkgconfig file from the .in template
SET( PKGFILE ${CMAKE_CURRENT_SOURCE_DIR}/autolab-rapi.pc )
CONFIGURE_FILE( ${PKGFILE}.in ${PKGFILE} @ONLY)
INSTALL( FILES ${PKGFILE} DESTINATION lib/pkgconfig/)

IF (STAGE_FOUND)
  SET( PKGFILE ${CMAKE_CURRENT_SOURCE_DIR}/autolab-rapi-stage.pc )
  CONFIGURE_FILE( ${PKGFILE}.in ${PKGFILE} @ONLY)
  INSTALL( FILES ${PKGFILE} DESTINATION lib/pkgconfig/)
ENDIF (STAGE_FOUND)

SET( PKGFILE ${CMAKE_CURRENT_SOURCE_DIR}/autolab-rapi-chatterbox.pc )
CONFIGURE_FILE( ${PKGFILE}.in ${PKGFILE} @ONLY)
INSTALL( FILES ${PKGFILE} DESTINATION lib/pkgconfig/)

IF (QT_FOUND)
  IF (WITH_GUI)
    SET( PKGFILE ${CMAKE_CURRENT_SOURCE_DIR}/autolab-rapi-gui.pc )
    CONFIGURE_FILE( ${PKGFILE}.in ${PKGFILE} @ONLY)
    INSTALL( FILES ${PKGFILE} DESTINATION lib/pkgconfig/)
  ELSE(WITH_GUI)
    MESSAGE(STATUS "lib-rapi-gui will NOT be build, enable with WITH_GUI")
  ENDIF(WITH_GUI)
ENDIF (QT_FOUND)
