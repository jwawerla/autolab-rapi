
SET( TARGET ${PROJECT_NAME} )

SET( HEADERS
  RapiCore
  robot.h
  device.h
  rangefinder.h
  drivetrain2dof.h
  localizer2d.h
  odometry.h
  powerpack.h
  fiducialfinder.h
  lights.h
  textdisplay.h
  pose2d.h
  pose3d.h
  point2d.h
  velocity2d.h
  utilities.h
  printerror.h
  dataupdateobserver.h
  rapierror.h
  rapitypes.h
  conversions.h
)

SET( SOURCES
  robot.cpp
  device.cpp
  rangefinder.cpp
  drivetrain2dof.cpp
  localizer2d.cpp
  odometry.cpp
  powerpack.cpp
  fiducialfinder.cpp
  lights.cpp
  textdisplay.cpp
  pose2d.cpp
  pose3d.cpp
  point2d.cpp
  velocity2d.cpp
  printerror.c
  rapierror.cpp
)

# build a shared library
ADD_LIBRARY( ${TARGET} SHARED ${SOURCES} )

# give the shared library a version number
SET_TARGET_PROPERTIES( ${TARGET} PROPERTIES
                        VERSION ${VERSION}
)

INSTALL(TARGETS ${TARGET}
  LIBRARY DESTINATION lib )

INSTALL(FILES ${HEADERS}
  DESTINATION include/${PROJECT_NAME}-${APIVERSION}
)
