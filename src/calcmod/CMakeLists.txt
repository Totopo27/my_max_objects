include(${CMAKE_CURRENT_SOURCE_DIR}/../max-sdk-base/script/max-pretarget.cmake)

set(PROJECT_NAME "calcmod")
set(PROJECT_VERSION "1.0.0")
set(AUTHOR "Totopo27")
set(DESCRIPTION "External object for sum, subtraction and modulo operations with temperament")

include_directories( 
    "${MAX_SDK_INCLUDES}"
    "${MAX_SDK_MSP_INCLUDES}"
    "${MAX_SDK_JIT_INCLUDES}"
)

add_library( 
    ${PROJECT_NAME} 
    MODULE
    ${PROJECT_NAME}.c
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../max-sdk-base/script/max-posttarget.cmake)