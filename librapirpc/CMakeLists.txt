
SET( PROJECT_RPC autolab-rapi-rpc )

SET( TARGET ${PROJECT_RPC} )

SET( HEADERS
  RapiRpc
  rpcdrivetrain.h
  rpcpowerpack.h
  rpcrangefinder.h
  rpcrobot.h
  robotrpcclient.h
)  

SET( SOURCES
  robotrpcclient.cpp
  rpcdrivetrain.cpp
  rpcpowerpack.cpp
  rpcrangefinder.cpp
  rpcrobot.cpp
)

INCLUDE_DIRECTORIES(
  ../librapicore
  ../librapichatterbox #TODO:stupid but we need tChargeSource
)

LINK_DIRECTORIES(
  ../librapicore
)

LINK_LIBRARIES( ${TARGET} autolab-rapi )

# build a shared library
ADD_LIBRARY( ${TARGET} SHARED ${SOURCES} )

# give the shared library a version number
SET_TARGET_PROPERTIES( ${TARGET} PROPERTIES
                        VERSION ${VERSION}
)

INSTALL(TARGETS ${TARGET}
  LIBRARY DESTINATION lib )

INSTALL(FILES ${HEADERS}
  DESTINATION include/${PROJECT_RPC}-${APIVERSION}
)

FIND_PACKAGE(Qt4)

# build an executable simple client
IF( WITH_GUI )
IF( QT_FOUND )
  INCLUDE( ${QT_USE_FILE})
  ADD_DEFINITIONS( ${QT_DEFINITIONS} )
  SET( CLIENT rpcclient )
  INCLUDE_DIRECTORIES(
    ${QT_INCLUDE_DIR}
    ../librapigui
  )
  LINK_DIRECTORIES(
    ${QT_LIBRARY_DIR}
    ../librapigui
    .
  )
  ADD_EXECUTABLE( ${CLIENT}
    client.cpp
  )
  TARGET_LINK_LIBRARIES( ${CLIENT} 
    ${QT_LIBRARIES}
    autolab-rapi-gui
    autolab-rapi-rpc
  )
  # for now, don't install...have to do some magic to LD_LIBRARY_PATH
  #INSTALL( TARGETS ${CLIENT} 
  #  RUNTIME DESTINATION bin )
ENDIF( QT_FOUND )
ENDIF( WITH_GUI )
