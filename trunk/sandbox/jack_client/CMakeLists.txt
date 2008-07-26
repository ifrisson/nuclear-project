project(jack_client)

SET(Libraries
  jack
  rt
  pthread
  )

ADD_EXECUTABLE(demo1 jack_client.h jack_client.cpp demo1.cpp)
TARGET_LINK_LIBRARIES(demo1 ${Libraries})

ADD_EXECUTABLE(demo2 jack_client.h jack_client.cpp demo2.cpp)
TARGET_LINK_LIBRARIES(demo2 ${Libraries})

ADD_EXECUTABLE(demo3 jack_client.h jack_client.cpp demo3.cpp)
TARGET_LINK_LIBRARIES(demo3 ${Libraries})
