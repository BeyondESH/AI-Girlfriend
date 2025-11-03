cmake_minimum_required(VERSION 3.16)

project(AIGirlfriend VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS WebSockets)
find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appAIGirlfriend
    main.cpp
)

qt_add_qml_module(appAIGirlfriend
    URI AIGirlfriend
    QML_FILES
        Main.qml
        SOURCES threadmgr.h threadmgr.cpp
        SOURCES test.h test.cpp
        SOURCES httpmgr.h httpmgr.cpp
        SOURCES global.h
        SOURCES global.cpp
        SOURCES audiomgr.h audiomgr.cpp
        SOURCES websocketmgr.h websocketmgr.cpp
        SOURCES messagemgr.h messagemgr.cpp
        SOURCES jsonmgr.h jsonmgr.cpp
        SOURCES configmgr.h configmgr.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appAIGirlfriend PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appAIGirlfriend
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appAIGirlfriend PRIVATE Qt6::WebSockets)
target_link_libraries(appAIGirlfriend PRIVATE Qt6::Multimedia)
target_link_libraries(appAIGirlfriend
    PRIVATE Qt6::Quick
)
target_link_libraries(appAIGirlfriend PRIVATE Qt6::Core)
target_link_libraries(appAIGirlfriend PRIVATE Qt6::Core)
target_link_libraries(appAIGirlfriend PRIVATE Qt6::Core)
target_link_libraries(appAIGirlfriend PRIVATE Qt6::Core)
target_link_libraries(appAIGirlfriend PRIVATE Qt6::Core)

include(GNUInstallDirs)
install(TARGETS appAIGirlfriend
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
