
add_library(lib-mock Arduino.cpp)
target_include_directories(lib-mock SYSTEM INTERFACE ".")
target_link_libraries(lib-mock trompeloeil::trompeloeil)
