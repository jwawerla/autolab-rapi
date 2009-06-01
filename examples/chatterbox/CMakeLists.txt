
IF (WITH_CB)
  INCLUDE_DIRECTORIES(
    ../../librapicore
    ../../librapichatterbox
  )

  LINK_DIRECTORIES(
    ../../librapicore
    ../../librapichatterbox
  )

  ADD_EXECUTABLE(chatterbox
    chatterbox.cpp
    chatterboxctrl.cpp
  )

  TARGET_LINK_LIBRARIES(chatterbox
    autolab-rapi
    autolab-rapi-chatterbox
  )

ENDIF (WITH_CB)