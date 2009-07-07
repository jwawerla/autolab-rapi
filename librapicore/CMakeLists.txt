
SET( TARGET ${PROJECT_NAME} )

# for config.h
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

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
  binarysensorarray.h
  switcharray.h
  analogsensorarray.h
  pose2d.h
  pose3d.h
  point2d.h
  rectangle.h
  velocity2d.h
  utilities.h
  printerror.h
  dataupdateobserver.h
  rapierror.h
  robotctrl.h
  rapitypes.h
  conversions.h
  waypoint2d.h
  rgbcolor.h
  limit.h
  robotupdateinterface.h
  rapivariable.h
  #booleanstatevariable.h
  variablemonitor.h
  timer.h
  rapivarinterface.h
  datalogger.h
  rapitypesystem.h
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
  binarysensorarray.cpp
  analogsensorarray.cpp
  switcharray.cpp
  pose2d.cpp
  pose3d.cpp
  point2d.cpp
  rectangle.cpp
  velocity2d.cpp
  printerror.c
  utilities.cpp
  rapierror.cpp
  robotctrl.cpp
  waypoint2d.cpp
  rgbcolor.cpp
  limit.cpp
  #rapivariable.cpp
  #booleanstatevariable.cpp
  variablemonitor.cpp
  timer.cpp
  datalogger.cpp
  rapitypesystem.cpp
)

# build a shared library
ADD_LIBRARY( ${TARGET} SHARED ${SOURCES} )

# give the shared library a version number
SET_TARGET_PROPERTIES( ${TARGET} PROPERTIES
                        VERSION ${VERSION}
                        #BUILD ${GIT_VERSION}
)

INSTALL(TARGETS ${TARGET}
  LIBRARY DESTINATION lib )

INSTALL(FILES ${HEADERS}
  DESTINATION include/${PROJECT_NAME}-${APIVERSION}
)
