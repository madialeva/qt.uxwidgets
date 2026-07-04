# App de demostración de UxWidgets. Valida los tres controles sin acoplarse a
# ninguna aplicación. AUTOMOC se hereda del CMakeLists.txt padre.

add_executable(UxWidgetsDemo main.cpp)

target_link_libraries(UxWidgetsDemo PRIVATE
    UxWidgets::UxWidgets
    Qt6::Widgets
)

# --- Despliegue autocontenido de la demo (solo Windows) --------------------
# Tras compilar, windeployqt copia las DLLs y plugins de Qt junto al .exe; con
# --compiler-runtime añade también la runtime de MinGW. Así la demo arranca con
# doble clic sin necesidad de tener el bin de Qt en el PATH.
if(WIN32)
    get_target_property(_qmake_exe Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_exe}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET UxWidgetsDemo POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                    --no-translations --compiler-runtime
                    "$<TARGET_FILE:UxWidgetsDemo>"
            COMMENT "Desplegando Qt junto a UxWidgetsDemo (windeployqt)"
            VERBATIM)

        # Refuerzo: copiar explícitamente las runtimes de MinGW por si la versión
        # de windeployqt no las trajera todas (p. ej. libgcc).
        get_filename_component(_mingw_bin "${CMAKE_CXX_COMPILER}" DIRECTORY)
        foreach(_dll libgcc_s_seh-1 libstdc++-6 libwinpthread-1)
            if(EXISTS "${_mingw_bin}/${_dll}.dll")
                add_custom_command(TARGET UxWidgetsDemo POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            "${_mingw_bin}/${_dll}.dll"
                            "$<TARGET_FILE_DIR:UxWidgetsDemo>"
                    VERBATIM)
            endif()
        endforeach()
    else()
        message(WARNING "windeployqt no encontrado; la demo no se desplegará automáticamente.")
    endif()
endif()
