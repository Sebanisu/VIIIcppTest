################################
#enable testing heh
enable_testing()
################################
################################
# GTest
################################
# https://stackoverflow.com/questions/58677831/cmake-adding-gtest-to-build
# older but uses add_test()
# https://stackoverflow.com/questions/8507723/how-to-start-working-with-gtest-and-cmake
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.10.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)


################################
# Tests
################################
# Add test cpp file
#add_executable(${TESTNAME} testgtest.cpp)
# Link test executable against gtest & gtest_main
#target_link_libraries(${TESTNAME} gtest gtest_main)