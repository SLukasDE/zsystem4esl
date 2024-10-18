file(GLOB_RECURSE ${PROJECT_NAME}_MAIN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE ${PROJECT_NAME}_MAIN_INC ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
list(FILTER ${PROJECT_NAME}_MAIN_INC INCLUDE REGEX "${CMAKE_CURRENT_SOURCE_DIR}/esl/.*" )
set(${PROJECT_NAME}_MAIN_INC_DIR  ${CMAKE_CURRENT_SOURCE_DIR})

set_property(GLOBAL PROPERTY ${PROJECT_NAME}_MAIN_SRC     "${${PROJECT_NAME}_MAIN_SRC}")
set_property(GLOBAL PROPERTY ${PROJECT_NAME}_MAIN_INC     "${${PROJECT_NAME}_MAIN_INC}")
set_property(GLOBAL PROPERTY ${PROJECT_NAME}_MAIN_INC_DIR "${${PROJECT_NAME}_MAIN_INC_DIR}")

if(ALL_IN_ONE_ESL)

    message(STATUS "Skip building of submodule ${PROJECT_NAME}")

else(ALL_IN_ONE_ESL)

    message(STATUS "Building submodule ${PROJECT_NAME}")

    if(MSVC)
       	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
   	endif()
	
    if(${PROJECT_NAME}_MAIN_SRC)
   	    if (BUILD_SHARED_LIBS)
       	    message(STATUS "-> lib type is SHARED")
   	        add_library(${PROJECT_NAME} SHARED)
        else (BUILD_SHARED_LIBS)
           	message(STATUS "-> lib type is STATIC")
       	    add_library(${PROJECT_NAME} STATIC)
   	    endif (BUILD_SHARED_LIBS)
        target_sources(${PROJECT_NAME} PRIVATE ${${PROJECT_NAME}_MAIN_SRC})
   	else(${PROJECT_NAME}_MAIN_SRC)
       	if (BUILD_SHARED_LIBS)
           	message(STATUS "-> lib type is INTERFACE (but SHARED has been requested)")
        else (BUILD_SHARED_LIBS)
           	message(STATUS "-> lib type is INTERFACE")
       	endif (BUILD_SHARED_LIBS)
       	add_library(${PROJECT_NAME} INTERFACE)
   	endif(${PROJECT_NAME}_MAIN_SRC)
	
   	target_sources(${PROJECT_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${PROJECT_NAME}_MAIN_INC})

    #add_library(esl::zsystem4esl ALIAS ${PROJECT_NAME})
    add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

    target_link_libraries(${PROJECT_NAME} PUBLIC
        esa::esa
        esl::esl
        #common4esl::common4esl
        zsystem::zsystem)

	#target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        FILE_SET HEADERS
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include)

endif(ALL_IN_ONE_ESL)
