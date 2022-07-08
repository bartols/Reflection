set(APP_BUNDLE ${PROJECT_NAME})

file(GLOB SOURCE *.cpp *.h)
add_library(${APP_BUNDLE} STATIC ${SOURCE})
target_include_directories(${APP_BUNDLE} 
     PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../external/optional)

#set_target_properties(${APP_BUNDLE} PROPERTIES
#        "OUTPUT_NAME" "${PROJECT_NAME}"
#        )

install(TARGETS ${APP_BUNDLE} EXPORT ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/lib)
export(TARGETS ${APP_BUNDLE} FILE ${CMAKE_BINARY_DIR}-config.cmake)