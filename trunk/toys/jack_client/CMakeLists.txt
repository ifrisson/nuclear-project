project(jack_client)

FILE(GLOB HEADERS *.h)
FILE(GLOB SOURCES *.cpp)

IF(CMAKE_HAS_SORT)
  LIST(SORT HEADERS)
  LIST(SORT SOURCES)
ENDIF(CMAKE_HAS_SORT)

SET(Libraries
  jack
  rt
  pthread
  )

ADD_EXECUTABLE(jack_client01 ${SOURCES})
TARGET_LINK_LIBRARIES(jack_client01 ${Libraries})
