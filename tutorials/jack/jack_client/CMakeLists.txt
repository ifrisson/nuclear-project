project(jack_demo)

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

ADD_EXECUTABLE(jack_demo ${SOURCES})
TARGET_LINK_LIBRARIES(jack_demo ${Libraries})
