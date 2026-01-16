cmake_minimum_required(VERSION 3.16)

project(HorosWin VERSION 0.1.0 LANGUAGES CXX)
include(InstallRequiredSystemLibraries)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 6 Configuration
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Sql OpenGLWidgets)
qt_standard_project_setup()

# Include Directories
include_directories(include)

# Source Files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/DatabaseManager.cpp
    src/BrowserWidget.cpp
    src/DICOMScanner.cpp
    src/DCMView.cpp
    src/ViewerWidget.cpp
    src/MPRWidget.cpp
    src/MPRViewport.cpp
    src/NetworkManager.cpp
    src/AutoImporter.cpp
    src/PACSPanel.cpp
    src/SettingsDialog.cpp
    src/ExportDialog.cpp
    src/AnonymizerService.cpp
)

set(HEADERS
    include/MainWindow.h
    include/DatabaseManager.h
    include/BrowserWidget.h
    include/DICOMScanner.h
    include/DCMView.h
    include/ViewerWidget.h
    include/ROI.h
    include/MPRWidget.h
    include/MPRViewport.h
    include/NetworkManager.h
    include/AutoImporter.h
    include/PACSPanel.h
    include/SettingsDialog.h
    include/ExportDialog.h
    include/AnonymizerService.h
)

# Executable
qt_add_executable(HorosWin ${SOURCES} ${HEADERS})

# Link Libraries
target_link_libraries(HorosWin PRIVATE 
    Qt6::Widgets 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Sql 
    Qt6::OpenGLWidgets
)

# Installation & Deployment
if(APPLE)
    set_target_properties(HorosWin PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "HorosWin"
        MACOSX_BUNDLE_GUI_IDENTIFIER "org.horosproject.horoswin"
    )
    # macOS Deployment using macdeployqt
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt)
    add_custom_command(TARGET HorosWin POST_BUILD
        COMMAND ${MACDEPLOYQT_EXECUTABLE} "$<TARGET_BUNDLE_FILE:HorosWin>" -dmg
        COMMENT "Running macdeployqt to create DMG..."
    )
endif()

install(TARGETS HorosWin
    BUNDLE DESTINATION .
    RUNTIME DESTINATION .
)

if(WIN32)
    set_target_properties(HorosWin PROPERTIES
        WIN32_EXECUTABLE ON
    )
    
    # Deployment using windeployqt during INSTALL
    # This ensures CPack picks up all DLLs and plugins
    install(CODE "
        execute_process(COMMAND windeployqt --no-compiler-runtime \"\${CMAKE_INSTALL_PREFIX}/HorosWin.exe\")
    ")
endif()

# CPack Configuration
set(CPACK_PACKAGE_NAME "HorosWindows")
set(CPACK_PACKAGE_VENDOR "Horos Project")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Horos Medical Image Viewer for Windows")
set(CPACK_PACKAGE_VERSION "1.0.0")

if(WIN32)
    set(CPACK_GENERATOR "NSIS;ZIP")
    set(CPACK_NSIS_DISPLAY_NAME "Horos for Windows")
    set(CPACK_NSIS_PACKAGE_NAME "Horos for Windows")
    set(CPACK_NSIS_HELP_LINK "http://horosproject.org")
    
    # Shortcuts
    set(CPACK_NSIS_MENU_LINKS "HorosWin.exe" "Horos for Windows")
    set(CPACK_NSIS_CREATE_ICONS_EXTRA "  CreateShortCut '$DESKTOP\\\\Horos for Windows.lnk' '$INSTDIR\\\\HorosWin.exe'")
    set(CPACK_NSIS_DELETE_ICONS_EXTRA "  Delete '$DESKTOP\\\\Horos for Windows.lnk'")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_PACKAGE_NAME "HorosMac")
endif()

set(CPACK_NSIS_MODIFY_PATH ON)
set(CPACK_PACKAGE_INSTALL_DIRECTORY "HorosWindows")
include(CPack)
