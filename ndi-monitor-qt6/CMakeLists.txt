cmake_minimum_required(VERSION 3.5)

set(EXE_NAME ndi-monitor-qt6)

project(${EXE_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS MultimediaWidgets)

set(TS_FILES "${EXE_NAME}_en_US.ts")

set(PROJECT_SOURCES
        main.h
        main.cpp
        mainwindow.h
        mainwindow.cpp
        mainwindow.ui
        ../NDI-Widget/ndireceiver.h
        ../NDI-Widget/ndireceiver.cpp
        ../NDI-Widget/ndireceiverworker.h
        ../NDI-Widget/ndireceiverworker.cpp
        ../NDI-Widget/ndiwrapper.h
        ../NDI-Widget/ndiwrapper.cpp
        ${TS_FILES}
)

set(PROJECT_INCLUDES ${PROJECT_INCLUDES} "../NDI-Widget/")

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(STATUS "X64")
  set(ARCH "x64")
else ()
  message(STATUS "X86")
  set(ARCH "x86")
endif ()

##############################################################################
# NDI
if (WIN32)
  if(DEFINED ENV{NDI_SDK_DIR})
    set(NDI_SDK_DIR "$ENV{NDI_SDK_DIR}")
  else()
    message(FATAL_ERROR "Could not find NDI SDK. The NDI_SDK_DIR environment variable must be set to the SDK path.")
  endif()
  message(STATUS "Using NDI_SDK_DIR=${NDI_SDK_DIR}")

  if(DEFINED ENV{NDI_Advanced_SDK_DIR})
    set(NDI_Advanced_SDK_DIR "$ENV{NDI_Advanced_SDK_DIR}")
  else()
    message(FATAL_ERROR "Could not find NDI Advanced SDK. The NDI_Advanced_SDK_DIR environment variable must be set to the SDK path.")
  endif()
  message(STATUS "Using NDI_Advanced_SDK_DIR=${NDI_Advanced_SDK_DIR}")

  # Advanced contains all non-Advanced includes
  set(PROJECT_INCLUDES ${PROJECT_INCLUDES} "${NDI_Advanced_SDK_DIR}/Include/")

  set(PROJECT_LIBS ${PROJECT_LIBS} "${NDI_SDK_DIR}/Lib/${ARCH}/Processing.NDI.Lib.${ARCH}.lib")
  set(PROJECT_LIBS ${PROJECT_LIBS} "${NDI_Advanced_SDK_DIR}/Lib/${ARCH}/Processing.NDI.Lib.Advanced.${ARCH}.lib")
endif ()
##############################################################################

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${EXE_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${EXE_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(${EXE_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${EXE_NAME}
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_include_directories(${EXE_NAME} PRIVATE ${PROJECT_INCLUDES})
target_link_libraries(${EXE_NAME}
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::MultimediaWidgets
  ${PROJECT_LIBS}
)

set_target_properties(${EXE_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${EXE_NAME})
endif()

############################################################################
if (WIN32)
  add_custom_command(TARGET ${EXE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${NDI_SDK_DIR}/Bin/${ARCH}/Processing.NDI.Lib.${ARCH}.dll" $<TARGET_FILE_DIR:${EXE_NAME}>)
  add_custom_command(TARGET ${EXE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${NDI_Advanced_SDK_DIR}/Bin/${ARCH}/Processing.NDI.Lib.Advanced.${ARCH}.dll" $<TARGET_FILE_DIR:${EXE_NAME}>)
endif()
