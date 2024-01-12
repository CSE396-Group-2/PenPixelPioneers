cmake_minimum_required(VERSION 3.16)
project(camera LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/multimedia/camera")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Multimedia MultimediaWidgets Widgets SerialPort)

# Set the paths for OpenCV
set(OpenCV_INCLUDE_DIRS "../opencv/include")  # Directory containing OpenCV header files
set(OpenCV_LIB_DIR "../opencv/x64/mingw/lib") # Directory containing OpenCV libraries

# Find OpenCV package
find_package(OpenCV REQUIRED PATHS ${OpenCV_LIB_DIR})

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

qt_add_executable(camera
    MANUAL_FINALIZATION
    camera.cpp camera.h camera.ui
    imagesettings.cpp imagesettings.h imagesettings.ui
    main.cpp
    videosettings.cpp videosettings.h videosettings.ui
    metadatadialog.cpp metadatadialog.h
    line_segmentor.h line_segmentor.cpp
    identify_writer.h identify_writer.cpp
    utils.h utils.cpp
    constants.h constants.cpp
    pre_processor.h pre_processor.cpp
    feature_extractor.h feature_extractor.cpp
    bg_remover.py transform.py

    default.json cli.cpp
    sheettopng.hpp sheettopng.cpp
    pngtosvg.hpp pngtosvg.cpp
    svgtottf.hpp svgtottf.cpp svgtottf.py
    textsimulation.h textsimulation.cpp
    worker.h worker.cpp
    fontrenderer.h fontrenderer.cpp
    # Add your resource files here
)

# Link OpenCV libraries and Qt6 modules to your executable
target_link_libraries(camera PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Multimedia
    Qt::MultimediaWidgets
    Qt::Widgets
    Qt::SerialPort # Link QtSerialPort library
    ${OpenCV_LIBS} # Link OpenCV libraries
)

set_target_properties(camera PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

if(APPLE AND NOT IOS)
    set_target_properties(camera PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/macos/Info.plist.in"
    )
elseif(IOS)
    set_target_properties(camera PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/ios/Info.plist.in"
    )
endif()

set_property(TARGET camera APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
             ${CMAKE_CURRENT_SOURCE_DIR}/android)

# Resources
qt_add_resources(camera "camera"
    PREFIX
        "/"
    FILES
        ${camera_resource_files}
)

qt_finalize_executable(camera)

install(TARGETS camera
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
