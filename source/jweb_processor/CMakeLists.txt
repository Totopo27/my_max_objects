include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk/script/max-pretarget.cmake)

set(PROJECT_NAME "jweb_processor")
project(${PROJECT_NAME})

set(SOURCE_FILES
    ${PROJECT_NAME}.c
)

add_library(${PROJECT_NAME} MODULE ${SOURCE_FILES})

include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk/script/max-posttarget.cmake)