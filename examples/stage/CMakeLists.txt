
IF (STAGE_FOUND)

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
    ${STAGE_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
  )

  LINK_DIRECTORIES(
    ${STAGE_LIBRARY_DIRS}
    ../../librapicore
    ../../librapistage
  )
  #build a shared library
  ADD_LIBRARY(wander MODULE
    wander.cpp
    wanderctrl.cpp
  )

  # delete the "lib" prefix from the plugin libraries
  SET_TARGET_PROPERTIES( wander PROPERTIES PREFIX "" )

  #need to link to some other libraries ? just add them here
  TARGET_LINK_LIBRARIES( wander
    ${STAGE_LIBRARIES}
    ${OPENGL_LIBRARIES}
    autolab-rapi
    autolab-rapi-stage
  )

  INSTALL(TARGETS wander
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
  )

ENDIF(STAGE_FOUND)
