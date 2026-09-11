# UxWidgets demo application. It validates the three controls without coupling
# them to an application. AUTOMOC is inherited from the parent CMakeLists.txt.

add_executable(UxWidgetsDemo main.cpp)

target_link_libraries(UxWidgetsDemo PRIVATE
    UxWidgets::UxWidgets
    Qt6::Widgets
)

# --- Self-contained demo deployment (Windows only) --------------------------
# After building, windeployqt copies Qt DLLs and plugins next to the executable.
# --compiler-runtime also copies the MinGW runtime so the demo can launch by
# double-clicking without Qt binaries in PATH.
if(WIN32)
    get_target_property(_qmake_exe Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_exe}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET UxWidgetsDemo POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                    --no-translations --compiler-runtime
                    "$<TARGET_FILE:UxWidgetsDemo>"
            COMMENT "Deploying Qt next to UxWidgetsDemo (windeployqt)"
            VERBATIM)

        # Copy MinGW runtimes explicitly in case windeployqt does not deploy all
        # of them, for example libgcc.
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
        message(WARNING "windeployqt was not found; the demo will not be deployed automatically.")
    endif()
endif()
