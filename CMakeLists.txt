cmake_minimum_required (VERSION 2.6)
project (SnesEmu)
include_directories(utils memory rom cpu debug)

file(GLOB utils_src utils/*.cpp)
file(GLOB memory_src memory/*.cpp)
file(GLOB rom_src rom/*.cpp)
file(GLOB cpu_src cpu/*.cpp)
file(GLOB root_src *.cpp)

add_executable(SnesEmu ${root_src} ${cpu_src} ${rom_src} ${memory_src} ${utils_src})
