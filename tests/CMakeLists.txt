enable_testing()
#set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

# So we can find the wraith includes.
include_directories(${WRAITH_INCLUDE_DIR})

add_subdirectory(core)
add_subdirectory(collection)
