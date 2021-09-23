cmake_minimum_required(VERSION 3.4.1)  # for example

set(PLUGIN_NAME "flutter-breakpad")

add_library(${PLUGIN_NAME} SHARED
  "src/main/cpp/flutter_breakpad.cpp"
)

target_link_libraries(${PLUGIN_NAME} log)