
SET( PROJECT_CB autolab-rapi-chatterbox )

SET( TARGET ${PROJECT_CB} )

ADD_SUBDIRECTORY(i2c)

SET( HEADERS
  RapiChatterbox
  cbrobot.h
  cbdrivetrain2dof.h
  cbpowerpack.h
  cbodometry.h
  cblaser.h
  cbdriver.h
  cbirsensor.h
  cblights.h
  cbtextdisplay.h
  cbbumper.h
  cbwallsensor.h
  cbwheeldropsensor.h
  cbcliffsensor.h
  cbovercurrentsensor.h
  cblowsidedriver.h
  cbtypes.h
)

SET( SOURCES
  cbrobot.cpp
  cbdrivetrain2dof.cpp
  cbpowerpack.cpp
  cbodometry.cpp
  cblaser.cpp
  cbdriver.cpp
  cbirsensor.cpp
  cblights.cpp
  cbtextdisplay.cpp
  cbbumper.cpp
  cbwallsensor.cpp
  cbwheeldropsensor.cpp
  cbcliffsensor.cpp
  cblowsidedriver.cpp
  cbovercurrentsensor.cpp
)

INCLUDE_DIRECTORIES(
  ../librapicore
  ./i2c
)

LINK_DIRECTORIES( ../librapicore
                  ../i2c )

LINK_LIBRARIES( ${TARGET} i2c autolab-rapi)

# build a shared library
ADD_LIBRARY( ${TARGET} SHARED ${SOURCES} )

# give the shared library a version number
SET_TARGET_PROPERTIES( ${TARGET} PROPERTIES
                        VERSION ${VERSION}
)

INSTALL(TARGETS ${TARGET}
  LIBRARY DESTINATION lib )

INSTALL(FILES ${HEADERS}
  DESTINATION include/${PROJECT_CB}-${APIVERSION}
)
