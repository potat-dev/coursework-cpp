project(tests)
include(FetchContent)

# cxx standard
set (CMAKE_CXX_STANDARD 11)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain number fourier)

include(CTest)
include(Catch)

# discover tests
catch_discover_tests(tests)