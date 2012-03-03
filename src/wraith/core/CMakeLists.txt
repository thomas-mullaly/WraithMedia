#######################################################################
# Wraith Core
#######################################################################
file(GLOB_RECURSE wraith_core_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(wraithcore SHARED ${wraith_core_SRCS})
target_link_libraries(wraithcore ${QT_LIBRARIES} ${GStreamer_LIBRARIES} ${GStreamer_base_LIBRARIES}
                     ${GStreamer_fft_LIBRARIES} ${Boost_LIBRARIES})
