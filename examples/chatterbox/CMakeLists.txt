
IF (WITH_CB)
  INCLUDE_DIRECTORIES(
    ../../librapicore
    ../../librapichatterbox
    ../../librapiautolabtracker
  )

  LINK_DIRECTORIES(
    ../../librapicore
    ../../librapichatterbox
    ../../librapiautolabtracker
  )

  ADD_EXECUTABLE(chatterbox
    chatterbox.cpp
    chatterboxctrl.cpp
  )

  TARGET_LINK_LIBRARIES(chatterbox
    autolab-rapi
    autolab-rapi-chatterbox
    autolab-rapi-autolabtracker
  )

ENDIF (WITH_CB)