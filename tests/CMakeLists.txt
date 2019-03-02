include_directories(Catch/single_include)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/lib)

include_directories(${CMAKE_BINARY_DIR})

#SET(CMAKE_CXX_FLAGS "-std=c++1y -O0")

add_definitions("-DTEST_DATA_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"")

if(WITH_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

    find_program(LCOV NAMES lcov PATHS /usr/bin /usr/local/bin)
    find_program (GCOV NAMES gcov gcov-7 gcov-6 gcov-5 PATHS /usr/bin /usr/local/bin)

    if(LCOV AND GCOV)
        message(STATUS "lcov found: ${LCOV}")
        message(STATUS "gcov found: ${GCOV}")
        add_custom_target(coverage_tilde
            COMMAND ${LCOV}
                --gcov-tool=${GCOV}
                --directory ${PROJECT_BINARY_DIR} #"${CMAKE_BINARY_DIR}"
                --capture
                --remove "${CMAKE_SOURCE_DIR}/../imgui/imgui/*"
                --remove "${CMAKE_SOURCE_DIR}/src/Properties/json/*"
                --remove "${CMAKE_SOURCE_DIR}/tests/Catch/*"
                --output-file coverage_tilde.info
            COMMAND ${LCOV}
                --extract coverage_tilde.info
                    "${CMAKE_SOURCE_DIR}/src/*"
                --remove "${CMAKE_SOURCE_DIR}/../imgui/imgui/*"
                --remove "${CMAKE_SOURCE_DIR}/src/Properties/json/*"
                --remove "${CMAKE_SOURCE_DIR}/tests/Catch/*"
                --output-file coverage_tilde.info
            COMMAND ${LCOV}
                --list coverage_tilde.info)
#            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}

        add_custom_target(coverage_report_tilde
            COMMAND genhtml --output-directory ${CMAKE_BINARY_DIR}/coverage_tilde coverage_tilde.info
            COMMAND open ${CMAKE_BINARY_DIR}/coverage_tilde/index.html)
    endif()
endif()

find_package(PythonInterp 3 REQUIRED)
find_package(PythonLibs 3 REQUIRED)

INCLUDE_DIRECTORIES(${PYTHON_LIBRARIES})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})

macro(max_py_add_test test_title test_name)
    add_executable("test_${test_name}" "test_${test_name}.cpp")

    target_link_libraries("test_${test_name}" py_shared )
    target_link_libraries("test_${test_name}" pybind11::embed)

    if (LCOV AND GCOV)
        target_link_libraries("test_${test_name}" gcov )
    endif()
    set(_exec_cmd "test_${test_name}")

    add_test(NAME ${test_title} COMMAND ${_exec_cmd})
endmacro()

max_py_add_test(t_converters converters)

# target_sources(test_converters PUBLIC "${PROJECT_SOURCE_DIR}/src/lib/convert.cpp")


# add_library(test_catch_main_lib_tilde STATIC tests_catch_main.cpp)

# add_test(properties tilde_properties)
# target_sources(test_tilde_properties PUBLIC "${PROJECT_SOURCE_DIR}/src/Properties/Property.cpp")
# target_sources(test_tilde_properties PUBLIC "${PROJECT_SOURCE_DIR}/src/Properties/PropertyList.cpp")
# target_sources(test_tilde_properties PUBLIC "${PROJECT_SOURCE_DIR}/src/Properties/PropertyTypes.cpp")
# target_sources(test_tilde_properties PUBLIC "${PROJECT_SOURCE_DIR}/src/FileIO/PdStringConverter.cpp")
