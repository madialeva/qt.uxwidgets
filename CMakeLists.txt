cmake_minimum_required(VERSION 3.21)

project(UxWidgets LANGUAGES CXX)

# Reusable Qt controls library (typed field + composite label/field/icon
# control). It is standalone and consumable from any Qt project.

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(UXWIDGETS_HEADERS
    include/UxWidgets/UxField.h
    include/UxWidgets/UxTextField.h
    include/UxWidgets/UxNumberField.h
    include/UxWidgets/UxDateField.h
    include/UxWidgets/UxInput.h
    include/UxWidgets/UxTextInput.h
    include/UxWidgets/UxNumberInput.h
    include/UxWidgets/UxDateInput.h
)

set(UXWIDGETS_SOURCES
    src/UxField.cpp
    src/UxTextField.cpp
    src/UxNumberField.cpp
    src/UxDateField.cpp
    src/UxInput.cpp
    src/UxTextInput.cpp
    src/UxNumberInput.cpp
    src/UxDateInput.cpp
)

add_library(UxWidgets STATIC ${UXWIDGETS_SOURCES} ${UXWIDGETS_HEADERS})
add_library(UxWidgets::UxWidgets ALIAS UxWidgets)

target_include_directories(UxWidgets PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(UxWidgets PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Demo application for validating controls without coupling them to an application.
option(UXWIDGETS_BUILD_DEMO "Build the UxWidgets demo application" ON)
if(UXWIDGETS_BUILD_DEMO)
    add_subdirectory(demo)
endif()
