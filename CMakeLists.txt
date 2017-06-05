cmake_minimum_required (VERSION 2.6)
project (SnesEmu)
include_directories(utils memory rom cpu debug interrupt device)

if (CMAKE_VERSION VERSION_LESS "3.1")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set (CMAKE_CXX_FLAGS "--std=gnu++14 ${CMAKE_CXX_FLAGS}")
endif ()
else ()
    set (CMAKE_CXX_STANDARD 14)
endif ()

# All warnings on
set (CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS}")

file(GLOB utils_src utils/*.cpp)
file(GLOB memory_src memory/*.cpp)
file(GLOB rom_src rom/*.cpp)
file(GLOB device_src device/*.cpp)
file(GLOB cpu_src cpu/*.cpp)
file(GLOB cpu_opcodes_src cpu/opcodes/*.cpp)
file(GLOB root_src *.cpp)

file(GLOB test_utils_src test/utils/*.cpp)

add_executable(SnesEmu ${root_src}
                       ${cpu_src}
                       ${rom_src}
                       ${memory_src}
                       ${device_src}
                       ${utils_src}
                       ${cpu_opcodes_src} cpu/Cpu65816Debugger.cpp cpu/Cpu65816Debugger.hpp cpu/BuildConfig.hpp)

# Setup CMake to run tests
enable_testing()

# Prep ourselves for compiling boost
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})

# I like to keep test files in a separate source directory called test
file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/cpu/*.cpp)

#Run through each source
foreach(testSrc ${TEST_SRCS})
    #Extract the filename without an extension (NAME_WE)
    get_filename_component(testName ${testSrc} NAME_WE)

    include_directories(utils memory rom cpu debug interrupt device test test/cpu test/utils)

    #Add compile target
    add_executable(${testName} ${testSrc} ${cpu_src} ${cpu_opcodes_src} ${rom_src} ${memory_src}
                   ${device_src} ${utils_src} ${test_utils_src}
                   test/utils/TestProgram.hpp test/utils/TestProgram.cpp test/utils/TestCommon.hpp)

    #link to Boost libraries AND your targets and dependencies
    target_link_libraries(${testName} ${Boost_LIBRARIES})

    #I like to move testing binaries into a testBin directory
    set_target_properties(${testName} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/testBin)

    #Finally add it to test execution -
    #Notice the WORKING_DIRECTORY and COMMAND
    add_test(NAME ${testName}
             WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin
             COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/testBin/${testName} )

endforeach(testSrc)
