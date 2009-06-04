
SET( PRJ autolab-rapi-gui )

SET( TARGET ${PRJ} )

FIND_PACKAGE(Qt4)

IF (WITH_GUI)
IF (QT_FOUND)

  INCLUDE(${QT_USE_FILE})
  ADD_DEFINITIONS( ${QT_DEFINITIONS} )

  # list all qt header files here for moc
  QT_WRAP_CPP(${TARGET} MOC_SRC
    mainwindow.h
    dataline.h
    dataled.h
    devicewidget.h
    fiducialfinderwidget.h
    devicewidgetlist.h
  )

  SET( HEADERS
    RapiGui
    gui.h
    #ndinterface.h
  )

  SET( SOURCES
    gui.cpp
    mainwindow.cpp
    robotwidget.cpp
    devicewidget.cpp
    drivetrain2dofwidget.cpp
    velocity2dwidget.cpp
    pose2dwidget.cpp
    rangefinderwidget.cpp
    powerpackwidget.cpp
    dataline.cpp
    dataled.cpp
    databar.cpp
    variablemonitorwidget.cpp
    fiducialfinderwidget.cpp
    consolewidget.cpp
    textdisplaywidget.cpp
    devicewidgetlist.cpp
    binarysensorarraywidget.cpp
   #ndwidget.cpp
    ${MOC_SRC}
  )

  LINK_DIRECTORIES(
    ${QT_LIBRARY_DIR}
    ../librapicore
  )

  INCLUDE_DIRECTORIES(
    ${QT_INCLUDE_DIR}
    ../librapicore
  )

  LINK_LIBRARIES(${TARGET}
    ${QT_LIBRARIES}
    autolab-rapi
  )

  MESSAGE(STATUS "QT ${QT_INCLUDE_DIR} ")

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
ELSE(QT_FOUND)
  MESSAGE(STATUS "QT4 not found, autolab-rapi-gui will NOT be build")
ENDIF(QT_FOUND)
ENDIF(WITH_GUI)
