
IF (STAGE_FOUND)

  ##########################################
  # Build with GUI
  FIND_PACKAGE(Qt4)

  IF (QT_FOUND)
    INCLUDE(${QT_USE_FILE})
    ADD_DEFINITIONS( ${QT_DEFINITIONS} )

    LINK_DIRECTORIES( ${STAGE_LIBRARY_DIRS} )

    SET_SOURCE_FILES_PROPERTIES( wander.cpp PROPERTIES COMPILE_FLAGS "${FLTK_CFLAGS}" )

    MESSAGE( ${INDENT} "Checking for OpenGL" )
    FIND_PACKAGE( OpenGL REQUIRED )
    IF( NOT OPENGL_GLU_FOUND )
      MESSAGE( FATAL_ERROR "OpenGL GLU not found, aborting" )
    ENDIF( NOT OPENGL_GLU_FOUND )
    MESSAGE( STATUS ${INDENT} "OpenGL GLU found at ${OPENGL_INCLUDE_DIR}" )
    MESSAGE( STATUS "    OPENGL_INCLUDE_DIR = ${OPENGL_INCLUDE_DIR}")
    MESSAGE( STATUS "    OPENGL_glu_LIBRARY = ${OPENGL_glu_LIBRARY}")

    INCLUDE_DIRECTORIES(
      ../../librapicore
      ../../librapistage
      ../../librapigui
      ${STAGE_INCLUDE_DIRS}
      ${GLIB_INCLUDE_DIRS}
      ${QT_INCLUDE_DIR}
    )

    LINK_DIRECTORIES(
      ${STAGE_LIBRARY_DIRS}
      ../../librapicore
      ../../librapistage
      ../../librapigui
    )
    #build a shared library
    ADD_LIBRARY(wander_rapi MODULE
      wander.cpp
      wanderctrl.cpp
    )

    #need to link to some other libraries ? just add them here
    TARGET_LINK_LIBRARIES( wander_rapi
      ${STAGE_LIBRARIES}
      ${OPENGL_LIBRARIES}
      ${QT_LIBRARIES}
      autolab-rapi
      autolab-rapi-stage
      autolab-rapi-gui
    )

  ELSE(QT_FOUND)
  #####################################
  # Build without GUI
    INCLUDE_DIRECTORIES(
      ../../librapicore
      ../../librapistage
      ${STAGE_INCLUDE_DIRS}
      ${GLIB_INCLUDE_DIRS}

    )

    LINK_DIRECTORIES(
      ${STAGE_LIBRARY_DIRS}
      ../../librapicore
      ../../librapistage
    )

    #build a shared library
    ADD_LIBRARY(wander_rapi MODULE
      wander.cpp
      wanderctrl.cpp
    )
    #need to link to some other libraries ? just add them here
    TARGET_LINK_LIBRARIES( wander_rapi
      ${STAGE_LIBRARIES}
      ${OPENGL_LIBRARIES}
      autolab-rapi
      autolab-rapi-stage
    )
  ENDIF(QT_FOUND)

  # delete the "lib" prefix from the plugin libraries
  SET_TARGET_PROPERTIES( wander_rapi PROPERTIES PREFIX "" )

  INSTALL(TARGETS wander_rapi
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
  )
ENDIF(STAGE_FOUND)
