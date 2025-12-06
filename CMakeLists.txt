cmake_minimum_required(VERSION 3.16)

project(AI-Girlfriend VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS WebSockets)
find_package(Qt6 REQUIRED COMPONENTS QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appAI-Girlfriend
    main.cpp
)

qt_add_qml_module(appAI-Girlfriend
    URI AI-Girlfriend
    QML_FILES
        Main.qml
        Components/SideButton.qml
        Components/HistoryListModel.qml
        Components/MessageBubble.qml
        Components/ChatMessageListModel.qml
        Pages/StatePage.qml
        Pages/SettingPage.qml
        Pages/ChatPage.qml
        Pages/VoiceChatPage.qml
        js/addListModel.js
    SOURCES
        audiomgr.cpp audiomgr.h 
        configmgr.cpp configmgr.h 
        gateway.cpp gateway.h
        application.h application.cpp
    RESOURCES 
        res.qrc
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appAI-Girlfriend PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appAI-Girlfriend
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appAI-Girlfriend
    PRIVATE Qt6::Quick
)
target_link_libraries(appAI-Girlfriend PRIVATE Qt6::Core)
target_link_libraries(appAI-Girlfriend PRIVATE Qt6::Multimedia)
target_link_libraries(appAI-Girlfriend PRIVATE Qt6::WebSockets)
target_link_libraries(appAI-Girlfriend PRIVATE Qt6::QuickControls2)

include(GNUInstallDirs)
install(TARGETS appAI-Girlfriend
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
