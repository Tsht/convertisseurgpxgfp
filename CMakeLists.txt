cmake_minimum_required(VERSION 3.16)

project(ConvertisseurGPX2GFP VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.7 REQUIRED COMPONENTS Quick Positioning)

qt_standard_project_setup(REQUIRES 6.7)

qt_add_executable(appConvertisseurGPX2GFP
    main.cpp
)

qt_add_qml_module(appConvertisseurGPX2GFP
    URI ConvertisseurGPX2GFP
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES converter.h converter.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appConvertisseurGPX2GFP PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appConvertisseurGPX2GFP
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appConvertisseurGPX2GFP
    PRIVATE Qt6::Quick Qt6::Positioning
)

include(GNUInstallDirs)
install(TARGETS appConvertisseurGPX2GFP
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_qml_app_script(
    TARGET appConvertisseurGPX2GFP
    OUTPUT_SCRIPT deploy_script
)
install(SCRIPT ${deploy_script})

# Configuration de CPack
include(InstallRequiredSystemLibraries)
if(WIN32)
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "TGZ")
endif()
set(CPACK_PACKAGE_NAME "ConvertisseurXML")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}")
include(CPack)
