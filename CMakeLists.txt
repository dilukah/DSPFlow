cmake_minimum_required(VERSION 3.16)

project(DSPFlow VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Charts)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appDSPFlow
    main.cpp
    pipeline.h
    step.h
    steps.h
)

qt_add_qml_module(appDSPFlow
    URI DSPFlow
    VERSION 1.0
    QML_FILES
        QML_FILES qml/components/PipelineView.qml
        QML_FILES qml/components/StepView.qml
        QML_FILES qml/Main.qml
        SOURCES stepFactory.h

)

set_target_properties(appDSPFlow PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appDSPFlow
    PRIVATE Qt6::Quick Qt6::Charts
)

include(GNUInstallDirs)
install(TARGETS appDSPFlow
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
