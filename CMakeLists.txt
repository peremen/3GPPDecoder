cmake_minimum_required(VERSION 3.16)
project(3GPPDecoder VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets)

qt_add_executable(3GPPDecoder WIN32 MACOSX_BUNDLE
    aboutdialog.cpp aboutdialog.h aboutdialog.ui
    dissectormodel.cpp dissectormodel.h
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    preferencedialog.cpp preferencedialog.h preferencedialog.ui
    tsharkdecoder.cpp tsharkdecoder.h
    umtsrlcdecoder.cpp umtsrlcdecoder.h
)

target_link_libraries(3GPPDecoder PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

# Resources:
set(images_resource_files
    "decoder.png"
)

qt_add_resources(3GPPDecoder "images"
    PREFIX
        "/images"
    FILES
        ${images_resource_files}
)

install(TARGETS 3GPPDecoder
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Consider using qt_generate_deploy_app_script() for app deployment if
# the project can use Qt 6.3. In that case rerun qmake2cmake with
# --min-qt-version=6.3.
