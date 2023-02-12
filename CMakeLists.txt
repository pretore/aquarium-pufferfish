cmake_minimum_required(VERSION 3.22)
project(aquarium-pufferfish VERSION 1.0.5
        LANGUAGES C
        DESCRIPTION "String pool in C")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# Dependencies
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_package(Doxygen OPTIONAL_COMPONENTS dot mscgen dia)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    include(cmake/FetchAquariumCMocka.cmake)
endif()
include(cmake/FetchAquariumSeahorse.cmake)

# Sources
set(EXPORTED_HEADER_FILES
        include/pufferfish/error.h
        include/pufferfish/string_pool.h
        include/pufferfish.h)
set(SOURCES
        ${EXPORTED_HEADER_FILES}
        src/pufferfish.c
        src/string_pool.c
        src/error.c)

if(DOXYGEN_FOUND)
    set(DOXYGEN_EXTRACT_ALL YES)
    doxygen_add_docs(${PROJECT_NAME}-documentation
            ${EXPORTED_HEADER_FILES})
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    include(CTest)
    add_library(${PROJECT_NAME} STATIC "")
    add_compile_definitions(TEST)
    target_sources(${PROJECT_NAME}
            PRIVATE
                ${SOURCES})
    target_link_libraries(${PROJECT_NAME}
            PUBLIC
                ${CMAKE_THREAD_LIBS_INIT}
                aquarium-cmocka
                aquarium-seahorse)
    target_include_directories(${PROJECT_NAME}
            PUBLIC
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
    # Unit Tests
    enable_testing()
    # aquarium-pufferfish-unit-test
    add_executable(${PROJECT_NAME}-unit-test test/test_pufferfish.c)
    target_include_directories(${PROJECT_NAME}-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(${PROJECT_NAME}-unit-test
            PRIVATE
                ${PROJECT_NAME})
    add_test(${PROJECT_NAME}-unit-test ${PROJECT_NAME}-unit-test)
    # aquarium-pufferfish-string-pool-unit-test
    add_executable(${PROJECT_NAME}-string-pool-unit-test
            test/test_string_pool.c)
    target_include_directories(${PROJECT_NAME}-string-pool-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(${PROJECT_NAME}-string-pool-unit-test
            PRIVATE
                ${PROJECT_NAME})
    add_test(${PROJECT_NAME}-string-pool-unit-test
            ${PROJECT_NAME}-string-pool-unit-test)
else()
    add_library(${PROJECT_NAME} "")
    target_sources(${PROJECT_NAME}
            PRIVATE
                ${SOURCES})
    target_include_directories(${PROJECT_NAME}
            PUBLIC
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
    target_link_libraries(${PROJECT_NAME}
            PUBLIC
                ${CMAKE_THREAD_LIBS_INIT}
                aquarium-seahorse)
    set_target_properties(${PROJECT_NAME}
            PROPERTIES
                VERSION ${PROJECT_VERSION}
                SOVERSION ${PROJECT_VERSION_MAJOR})
    include(GNUInstallDirs)
    install(DIRECTORY include/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(TARGETS ${PROJECT_NAME}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
    configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
    install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()
