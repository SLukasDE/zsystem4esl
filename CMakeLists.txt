cmake_minimum_required(VERSION 3.23)

project(zsystem4esl VERSION 1.5.0
            DESCRIPTION "Common files for eslx library"
            LANGUAGES CXX)
            
#include(FetchContent)
#FetchContent_Declare(
#  catch
#  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#  GIT_TAG        v2.13.6
#)
#FetchContent_MakeAvailable(catch)


# The compiled library code is here
#add_subdirectory(src/main)
# STATIC, SHARED, MODULE, INTERFACE
add_library(zsystem4esl STATIC
      ./src/main/zsystem4esl/Plugin.cpp
      ./src/main/zsystem4esl/system/signal/ThreadHandler.cpp
      ./src/main/zsystem4esl/system/signal/DirectHandler.cpp
      ./src/main/zsystem4esl/system/signal/Signal.h
      ./src/main/zsystem4esl/system/signal/ThreadManager.h
      ./src/main/zsystem4esl/system/signal/Signal.cpp
      ./src/main/zsystem4esl/system/signal/DirectHandler.h
      ./src/main/zsystem4esl/system/signal/ThreadManager.cpp
      ./src/main/zsystem4esl/system/signal/ThreadHandler.h
      ./src/main/zsystem4esl/system/stacktrace/Stacktrace.cpp
      ./src/main/zsystem4esl/system/stacktrace/Stacktrace.h
      ./src/main/zsystem4esl/system/process/Process.cpp
      ./src/main/zsystem4esl/system/process/Consumer.cpp
      ./src/main/zsystem4esl/system/process/Reader.cpp
      ./src/main/zsystem4esl/system/process/Writer.cpp
      ./src/main/zsystem4esl/system/process/Producer.cpp
      ./src/main/zsystem4esl/system/process/Writer.h
      ./src/main/zsystem4esl/system/process/Consumer.h
      ./src/main/zsystem4esl/system/process/Process.h
      ./src/main/zsystem4esl/system/process/Producer.h
      ./src/main/zsystem4esl/system/process/Reader.h
      ./src/main/zsystem4esl/Plugin.h
      ./src/main/zsystem4esl/Logger.h
)

target_include_directories(zsystem4esl PUBLIC ./src/main)
target_compile_features(zsystem4esl PUBLIC cxx_std_11)
set_property(TARGET zsystem4esl PROPERTY POSITION_INDEPENDENT_CODE ON)

find_package(esl REQUIRED)
find_package(zsystem REQUIRED)

target_link_libraries(zsystem4esl PRIVATE esl::esl)
target_link_libraries(zsystem4esl PRIVATE zsystem::zsystem)

install(TARGETS zsystem4esl
        EXPORT zsystem4esl-export
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

install(EXPORT      zsystem4esl-export
        FILE        zsystem4eslTargets.cmake
        NAMESPACE   zsystem4esl::
        DESTINATION lib/cmake
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/zsystem4eslConfig.cmake
        DESTINATION "lib/cmake"
)




# The compiled test code is here
#add_subdirectory(src/test)
