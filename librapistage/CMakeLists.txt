
SET( PRJ autolab-rapi-stage )

SET( TARGET ${PRJ} )

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE( GLIB REQUIRED gthread-2.0 )

IF( GLIB_FOUND )
  MESSAGE( STATUS ${INDENT} "Glib version ${GLIB_VERSION} detected at ${GLIB_PREFIX}" )
  MESSAGE( STATUS "    GLIB_CFLAGS = ${GLIB_CFLAGS}" )
  MESSAGE( STATUS "    GLIB_LDFLAGS = ${GLIB_LDFLAGS}" )
ELSE( GLIB_FOUND )
  MESSAGE( ${INDENT} "Glib not detected" )
ENDIF( GLIB_FOUND )

IF (STAGE_FOUND)
  MESSAGE(STAGE "${STAGE_LIBRARY_DIRS}" )
  MESSAGE(STAGE "${STAGE_LIBRARIES}" )
  MESSAGE(STAGEDIR "${STAGE_INCLUDE_DIRS}" )

  MESSAGE( ${INDENT} "Checking for OpenGL" )

  FIND_PACKAGE( OpenGL REQUIRED )
  IF( NOT OPENGL_GLU_FOUND )
    MESSAGE( FATAL_ERROR "OpenGL GLU not found, aborting" )
  ENDIF( NOT OPENGL_GLU_FOUND )
  MESSAGE( STATUS ${INDENT} "OpenGL GLU found at ${OPENGL_INCLUDE_DIR}" )
  MESSAGE( STATUS "    OPENGL_INCLUDE_DIR = ${OPENGL_INCLUDE_DIR}")
  MESSAGE( STATUS "    OPENGL_glu_LIBRARY = ${OPENGL_glu_LIBRARY}")

  INCLUDE_DIRECTORIES( ${STAGE_INCLUDE_DIRS}
                       ${GLIB_INCLUDE_DIRS}
                       ../librapicore
  )

  LINK_DIRECTORIES( ${STAGE_LIBRARY_DIRS} )


  SET( HEADERS
    stagerobot.h
    stagelaser.h
    stagesonar.h
    stagedrivetrain2dof.h
    stageodometry.h
    stagepowerpack.h
    stagefiducialfinder.h
    stagelights.h
    stagetextdisplay.h
    loosestagerobot.h
    loosestagelaser.h
    loosestagelights.h
    loosestagedrivetrain2dof.h
    loosestagepowerpack.h
    loosestagefiducialfinder.h
    loosestagetextdisplay.h
    loosestagesonar.h
    RapiStage
    RapiLooseStage
  )

  SET( SOURCES
    stagerobot.cpp
    stagelaser.cpp
    stagesonar.cpp
    stagedrivetrain2dof.cpp
    stageodometry.cpp
    stagepowerpack.cpp
    stagefiducialfinder.cpp
    stagelights.cpp
    stagetextdisplay.cpp
    loosestagerobot.cpp
    loosestagelaser.cpp
    loosestagelights.cpp
    loosestagedrivetrain2dof.cpp
    loosestagepowerpack.cpp
    loosestagefiducialfinder.cpp
    loosestagetextdisplay.cpp
    loosestagesonar.cpp
  )

  # build a shared library
  ADD_LIBRARY( ${TARGET} SHARED ${SOURCES} )

  # give the shared library a version number
  SET_TARGET_PROPERTIES( ${TARGET} PROPERTIES
                          VERSION ${VERSION}
  )

  INSTALL(TARGETS ${TARGET}
    LIBRARY DESTINATION lib
  )

  INSTALL(FILES ${HEADERS}
    DESTINATION include/${PRJ}-${APIVERSION}
  )

ELSEIF(STAGE_FOUND)
  MESSAGE( STATUS "Stage was not found, rapi-stage will NOT be build")
ENDIF(STAGE_FOUND)



