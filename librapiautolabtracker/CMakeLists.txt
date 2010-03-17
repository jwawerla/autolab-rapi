SET( PROJECT_AT autolab-rapi-autolabtracker)
SET( TARGET ${PROJECT_AT} )

INCLUDE_DIRECTORIES(
  ../librapicore
)

LINK_DIRECTORIES( 
 ../librapicore
)

SET( HEADERS
  RapiAutolabTracker
  autolabtracker.h
)

SET( SOURCES
  autolabtracker.cpp
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
