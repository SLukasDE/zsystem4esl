cmake_minimum_required(VERSION 3.23)

project(
    zsystem4esl
    VERSION 1.6.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(Packages)

option(COMPILE_UNITTESTS "Weather to compile unittests" ON)

if(NOT ALL_IN_ONE_ESL) # always needed for unittests
    find_package_esl()
    find_package_zsystem()
endif(NOT ALL_IN_ONE_ESL)

add_subdirectory(src/main)

if(NOT ALL_IN_ONE_ESL AND COMPILE_UNITTESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/test/main.cpp")
    add_subdirectory(src/test)
endif()

if(NOT ALL_IN_ONE_ESL)
    install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION lib/cmake/${PROJECT_NAME})

    install(FILES "${PROJECT_NAME}Config.cmake" DESTINATION lib/cmake/${PROJECT_NAME})
    #install(FILES "${PROJECT_NAME}Config.cmake" "${PROJECT_NAME}ConfigVersion.cmake" DESTINATION lib/cmake/${PROJECT_NAME})
endif(NOT ALL_IN_ONE_ESL)
