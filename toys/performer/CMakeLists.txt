PROJECT(performer)

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
lo
)

ADD_EXECUTABLE(performer ${SOURCES})
TARGET_LINK_LIBRARIES(performer ${Libraries})
