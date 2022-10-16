project(tests)

add_executable(tests tests.cpp)

target_link_libraries(tests number fourier)

add_test(NAME tests COMMAND tests)