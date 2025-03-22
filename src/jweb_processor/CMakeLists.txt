include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base/script/max-pretarget.cmake)

# Name of the project
set(PROJECT_NAME "jweb_processor")

# Add source files
add_library(
	${PROJECT_NAME}
	MODULE
	${PROJECT_NAME}.c
)

# Add Max SDK includes
target_include_directories(${PROJECT_NAME} PRIVATE
	"${MAX_SDK_INCLUDES}"
	"${MAX_SDK_MSP_INCLUDES}"
	"${MAX_SDK_JIT_INCLUDES}"
)

# Windows and macOS specific settings
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC "${MAX_SDK_LIBS}")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".mxe64")
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} PUBLIC "${MAX_SDK_LIBS}")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".mxo")
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base/script/max-posttarget.cmake)