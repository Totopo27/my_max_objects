include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base/script/max-pretarget.cmake)

# Name of the external
set(PROJECT_NAME jweb_processor)

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

# Link with Max SDK libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${MAX_SDK_LIBS})

include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base/script/max-posttarget.cmake)