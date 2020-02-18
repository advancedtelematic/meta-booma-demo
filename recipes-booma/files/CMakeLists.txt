
# Copyright (C) 2019 HERE Global B.V. and its affiliate(s).
# All rights reserved.
#
# This software and other materials contain proprietary information
# controlled by HERE and are protected by applicable copyright legislation.
# Any use and utilization of this software and other materials and
# disclosure to any third parties is conditional upon having a separate
# agreement with HERE for the access, use, utilization or disclosure of this
# software. In the absence of such agreement, the use of the software is not
# allowed.
#

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)

project("dataservice-write" VERSION 0.1.0)

find_package(olp-cpp-sdk-authentication REQUIRED)
find_package(olp-cpp-sdk-dataservice-write REQUIRED)

find_package(Boost REQUIRED COMPONENTS program_options)
find_package(leveldb REQUIRED)
find_package(Snappy REQUIRED)
find_package(Threads REQUIRED)

add_executable(dataserviceWriteApp ${CMAKE_CURRENT_SOURCE_DIR}/example.h ${CMAKE_CURRENT_SOURCE_DIR}/example.cpp)

if (IOS)
    find_library(CORE_FOUNDATION Foundation)
    find_library(CF_NETWORK_FRAMEWORK CFNetwork)
    find_library(SECURITY_FRAMEWORK Security)
    find_library(UIKIT_FRAMEWORK UIKit)

    set(RESOURCE_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/ios/resources/Main.storyboard
                        ${CMAKE_CURRENT_SOURCE_DIR}/ios/resources/dummy.txt)
    target_sources(dataserviceWriteApp PUBLIC
                        ${CMAKE_CURRENT_SOURCE_DIR}/ios/AppDelegate.h
                        ${CMAKE_CURRENT_SOURCE_DIR}/ios/AppDelegate.mm
                        ${CMAKE_CURRENT_SOURCE_DIR}/ios/ViewController.h
                        ${CMAKE_CURRENT_SOURCE_DIR}/ios/ViewController.mm
                        ${CMAKE_CURRENT_SOURCE_DIR}/ios/main.mm
                        ${RESOURCE_FILES})
    set_property (TARGET dataserviceWriteApp APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc")

    set(APP_BUNDLE_IDENTIFIER "com.here.olp.examples.dataservice-write")

    set(MACOSX_BUNDLE_INFO_STRING ${APP_BUNDLE_IDENTIFIER})
    set(MACOSX_BUNDLE_GUI_IDENTIFIER ${APP_BUNDLE_IDENTIFIER})
    set(MACOSX_BUNDLE_BUNDLE_NAME ${APP_BUNDLE_IDENTIFIER})
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "1.0")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "1.0")
    set_target_properties(dataserviceWriteApp PROPERTIES
        XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
        RESOURCE "${RESOURCE_FILES}"
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/ios/Info.plist
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
        XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
    )

    target_link_libraries(dataserviceWriteApp
        ${CORE_FOUNDATION}
        ${SECURITY_FRAMEWORK}
        ${CF_NETWORK_FRAMEWORK}
        ${UIKIT_FRAMEWORK}
    )
else()
    target_sources(dataserviceWriteApp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
endif()

target_include_directories(dataserviceWriteApp
   PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(dataserviceWriteApp
   olp-cpp-sdk-authentication
   olp-cpp-sdk-dataservice-write
   ${Boost_LIBRARIES}
)
