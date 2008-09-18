PROJECT(nuclear-tests)

INCLUDE_DIRECTORIES(${nuclear_SOURCE_DIR})

SET(Libraries dl m)

ADD_LIBRARY(lv2_plugin lv2_plugin.cpp)
TARGET_LINK_LIBRARIES(lv2_plugin ${Libraries})

#ADD_EXECUTABLE(test-lv2-plugin lv2_plugin.cpp)
#ADD_TEST(nuclear.sdk.lv2.plugin ${EXECUTABLE_OUTPUT_PATH}/test-lv2-plugin)
