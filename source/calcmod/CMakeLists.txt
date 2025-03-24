cmake_minimum_required(VERSION 3.0)

project(calcmod)

if (NOT DEFINED C74_MAX_SDK_DIR)
    if (DEFINED ENV{C74_MAX_SDK_DIR})
        set(C74_MAX_SDK_DIR $ENV{C74_MAX_SDK_DIR})
    else()
        set(C74_MAX_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base)
    endif()
endif()

set(C74_INCLUDES 
    ${C74_MAX_SDK_DIR}/source/c74support/max-includes
    ${C74_MAX_SDK_DIR}/source/c74support/msp-includes
)

set(C74_LIBS
    ${C74_MAX_SDK_DIR}/source/c74support/max-includes/x64/MaxAPI.lib
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/calcmod.c
)

include_directories(${C74_INCLUDES})

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${C74_LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".mxe64")

if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES BUNDLE TRUE)
    set_target_properties(${PROJECT_NAME} PROPERTIES BUNDLE_EXTENSION "mxo")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "calcmod")

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../externals"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../externals"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../externals"
)