find_package(Catch2 REQUIRED)
find_package(trompeloeil REQUIRED)

add_subdirectory(mock)

function(add_test_suite name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE midibutton lib-mock Catch2::Catch2WithMain trompeloeil::trompeloeil)
    add_test(${name} ${name})
endfunction()

add_test_suite(MidiButtonTest)
add_test_suite(LedTest)
add_test_suite(ButtonTest)
add_test_suite(ActionTest)


add_custom_target(unittest MidiButtonTest
    COMMAND LedTest
    COMMAND ButtonTest
    COMMAND ActionTest
    COMMENT "Running unittests\n\n"
    VERBATIM
    )
