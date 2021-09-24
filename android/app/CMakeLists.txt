cmake_minimum_required(VERSION 3.4.1)  # for example

set(PLUGIN_NAME "flutter-breakpad")

add_library(breakpad_client STATIC IMPORTED)
set_target_properties(breakpad_client PROPERTIES
        IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cmakeLibs/${ANDROID_ABI}/libbreakpad_client.a
        )

add_library(${PLUGIN_NAME} SHARED
        "src/main/cpp/flutter_breakpad.cpp"
        )

target_include_directories(${PLUGIN_NAME}
        PRIVATE # recursively with PRIVATE
        "src/main/cpp/breakpad"
        )

target_link_libraries(${PLUGIN_NAME} breakpad_client log)