#######################################################################
# Wraith GUI
#######################################################################
file(GLOB_RECURSE wraith_gui_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE wraith_gui_UI ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)

set(wraith_gui_HDRS
    MainWindow.hpp
    SliderWidget.hpp
    visualizer/VisualizerWidget.hpp
    equalizer/EqualizerBand.hpp
    equalizer/EqualizerBandLabel.hpp
    equalizer/EqualizerWidget.hpp)

file(GLOB_RECURSE wraith_gui_RES ${CMAKE_CURRENT_SOURCE_DIR}/extras/*.qrc)

QT4_ADD_RESOURCES(wraith_gui_RCC ${wraith_gui_RES})
QT4_WRAP_CPP(wraith_gui_MOC ${wraith_gui_HDRS})
QT4_WRAP_UI(wraith_gui_UI_WRAPPED ${wraith_gui_UI})

add_library(wraithgui SHARED ${wraith_gui_SRCS} ${wraith_gui_UI_WRAPPED} ${wraith_gui_MOC} ${wraith_gui_RCC})
add_dependencies(wraithgui wraithcore)
target_link_libraries(wraithgui wraithcore ${QT_LIBRARIES})
