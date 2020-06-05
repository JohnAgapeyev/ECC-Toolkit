cmake_minimum_required(VERSION 3.9)
project(ECC-Toolkit)

set(ECC_VERSION_MAJOR 1)
set(ECC_VERSION_MINOR 0)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "")
set(CMAKE_CXX_FLAGS_MINSIZEREL "")

set(BASE_COMPILE_OPTIONS -pipe -march=native -fcf-protection=full -fstack-protector-strong)
set(WARNINGS -Wall -Wextra -Wpedantic -Wformat=2 -Wuninitialized -Wundef -Wcast-align -Wstrict-overflow=2 -Wwrite-strings -Wno-format-nonliteral)
set(DEBUG_OPTIONS ${BASE_COMPILE_OPTIONS} -g -O0)
set(RELEASE_OPTIONS ${BASE_COMPILE_OPTIONS} -s -Ofast)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_LINK_WHAT_YOU_USE ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}")

SET(MAIN_HEADER "ecctoolkit.h")

SET(TEST
    "test/main.cpp"
    )

#find_package(Python3 COMPONENTS Development.Embed)
find_package(Python3 COMPONENTS Development)

enable_testing()

add_executable(ecctest ${TEST})

target_compile_options(ecctest
    PRIVATE ${WARNINGS}
    PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_OPTIONS}>" "$<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>")
target_compile_definitions(ecctest PRIVATE "$<$<CONFIG:RELEASE>:NDEBUG>")
target_include_directories(ecctest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${Python3_INCLUDE_DIRS})
target_link_libraries(ecctest PRIVATE ${Python3_LIBRARIES})

set_property(TARGET ecctest PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

#add_test(NAME encryption_tests COMMAND test_binary -t encryption_tests)
#add_test(NAME signing_tests COMMAND test_binary -t sign_tests)
