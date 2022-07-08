set(TEST_BUNDLE ${PROJECT_NAME}_test)

file(GLOB SOURCES *.cpp *.h)
add_executable(${TEST_BUNDLE} ${SOURCES})

target_include_directories(${TEST_BUNDLE}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../external/optional
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../external/TinyXml
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src)

# Specify which targets the test bundle depends on
target_link_libraries(${TEST_BUNDLE} ${APP_BUNDLE} gtest Reflection tiny_xml)

# Register tests
add_test(NAME ${TEST_BUNDLE}
         COMMAND ${TEST_BUNDLE})