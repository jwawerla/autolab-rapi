SET( PROJECT_AT autolab-rapi-tracker )

# for config.h
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

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
