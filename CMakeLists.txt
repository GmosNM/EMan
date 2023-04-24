cmake_minimum_required(VERSION 3.5)
project(EMan)
set(CMAKE_export_compile_commands ON)
set(EXECUTABLE_OUTPUT_PATH ../bin)


set(SRC
    src/main.cpp
    src/EMan.h
    src/EMan.cpp
)


add_executable(EMan ${SRC})
target_compile_definitions(EMan PRIVATE _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING)
