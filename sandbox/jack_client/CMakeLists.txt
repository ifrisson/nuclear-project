project(jack_client)

SET(Libraries
  jack
  rt
  pthread
  )

ADD_EXECUTABLE(demo1 demo1.cpp)
TARGET_LINK_LIBRARIES(demo1 ${Libraries})

ADD_EXECUTABLE(demo2 demo2.cpp)
TARGET_LINK_LIBRARIES(demo2 ${Libraries})

ADD_EXECUTABLE(demo3 demo3.cpp)
TARGET_LINK_LIBRARIES(demo3 ${Libraries})

ADD_EXECUTABLE(simple_polyphonic simple_polyphonic.cpp)
TARGET_LINK_LIBRARIES(simple_polyphonic ${Libraries})

ADD_EXECUTABLE(simple_monophonic simple_monophonic.cpp)
TARGET_LINK_LIBRARIES(simple_monophonic ${Libraries})
