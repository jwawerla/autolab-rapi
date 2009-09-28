
SET( PRJ autolab-rapi-stage )

SET( TARGET ${PRJ} )

INCLUDE(FindPkgConfig)

IF (STAGE_FOUND)
  MESSAGE(STAGE_LIBRARY_DIRS " ${STAGE_LIBRARY_DIRS}" )
  MESSAGE(STAGE_LIBRARIES " ${STAGE_LIBRARIES}" )
  MESSAGE(STAGE_INCLUDE_DIRS " ${STAGE_INCLUDE_DIRS}" )

  # do some work to cope with frameworks on OS X
  STRING(REGEX REPLACE ";" " " STAGE_LDFLAGS_MORE "${STAGE_LDFLAGS_OTHER}")
  MESSAGE(STAGE_LDFLAGS_MORE " ${STAGE_LDFLAGS_MORE}" )

  INCLUDE_DIRECTORIES( ${STAGE_INCLUDE_DIRS}
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
    stageblobfinder.h
    loosestagerobot.h
    loosestagelaser.h
    loosestagelights.h
    loosestagedrivetrain2dof.h
    loosestagepowerpack.h
    loosestagefiducialfinder.h
    loosestagetextdisplay.h
    loosestagesonar.h
    loosestageblobfinder.h
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
    stageblobfinder.cpp
    loosestagerobot.cpp
    loosestagelaser.cpp
    loosestagelights.cpp
    loosestagedrivetrain2dof.cpp
    loosestagepowerpack.cpp
    loosestagefiducialfinder.cpp
    loosestagetextdisplay.cpp
    loosestagesonar.cpp
    loosestageblobfinder.cpp
  )

  # build a shared library
  ADD_LIBRARY( ${TARGET} SHARED ${SOURCES} )

  # give the shared library a version number
  SET_TARGET_PROPERTIES( ${TARGET} PROPERTIES
                          VERSION ${VERSION}
								  LINK_FLAGS "${STAGE_LDFLAGS_MORE}"
  )


  INSTALL(TARGETS ${TARGET}
    LIBRARY DESTINATION lib
  )

  INSTALL(FILES ${HEADERS}
    DESTINATION include/${PRJ}-${APIVERSION}
  )

 # link Stage and the core RAPI library
 TARGET_LINK_LIBRARIES( ${TARGET} ${STAGE_LIBRARIES} autolab-rapi )

ELSEIF(STAGE_FOUND)
  MESSAGE( STATUS "Stage was not found, rapi-stage will NOT be build")
ENDIF(STAGE_FOUND)



