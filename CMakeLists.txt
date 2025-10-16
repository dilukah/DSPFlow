cmake_minimum_required(VERSION 3.16)

project(DSPFlow VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Charts Test)

qt_standard_project_setup(REQUIRES 6.8)

add_library(dspcore STATIC
    pipeline.h
    step.h
    steps.h
    stepFactory.h
)

target_include_directories(dspcore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(dspcore PRIVATE Qt6::Quick Qt6::Charts)

# Main application
qt_add_executable(appDSPFlow
    main.cpp
)

qt_add_qml_module(appDSPFlow
    URI DSPFlow
    VERSION 1.0
    QML_FILES
        qml/components/PipelineView.qml
        qml/components/StepView.qml
        qml/Main.qml
)

set_target_properties(appDSPFlow PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appDSPFlow
    PRIVATE dspcore Qt6::Quick Qt6::Charts
)

include(GNUInstallDirs)
install(TARGETS appDSPFlow
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Unit tests
enable_testing()

add_executable(test_pipeline
    tests/test_pipeline.cpp
)

target_link_libraries(test_pipeline
    PRIVATE dspcore Qt6::Test Qt6::Core
)

add_test(NAME PipelineTest COMMAND test_pipeline)
