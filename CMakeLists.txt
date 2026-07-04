cmake_minimum_required(VERSION 3.21)

project(UxWidgets LANGUAGES CXX)

# Librería de controles Qt reutilizables (campo tipado + control compuesto
# etiqueta/campo/icono). Autocontenida: no depende de ninguna aplicación,
# consumible desde cualquier proyecto Qt.

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(UXWIDGETS_HEADERS
    include/UxWidgets/UxCampo.h
    include/UxWidgets/UxCampoTexto.h
    include/UxWidgets/UxCampoNumero.h
    include/UxWidgets/UxCampoFecha.h
    include/UxWidgets/UxInput.h
    include/UxWidgets/UxInputTexto.h
    include/UxWidgets/UxInputNumero.h
    include/UxWidgets/UxInputFecha.h
)

set(UXWIDGETS_SOURCES
    src/UxCampo.cpp
    src/UxCampoTexto.cpp
    src/UxCampoNumero.cpp
    src/UxCampoFecha.cpp
    src/UxInput.cpp
    src/UxInputTexto.cpp
    src/UxInputNumero.cpp
    src/UxInputFecha.cpp
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

# App de demostración para validar los controles sin acoplarse a ninguna aplicación.
option(UXWIDGETS_BUILD_DEMO "Construir la app de demostración de UxWidgets" ON)
if(UXWIDGETS_BUILD_DEMO)
    add_subdirectory(demo)
endif()
