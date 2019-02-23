cmake_minimum_required (VERSION 2.8)
SET(CMAKE_CXX_FLAGS "-std=c++1y -O3")

project (py_exec)

set (LIB
    src/lib/dyn_class.cpp
    src/lib/dyn_class.hpp
    src/lib/convert.cpp
    src/lib/convert.hpp
    src/lib/module.cpp
    )

set (SOURCES0
        src/py_eval.cpp
)

set (SOURCES1
        src/py_exec.cpp
)

set (SOURCES2
        src/py_module.cpp
)

set (SOURCES3
        src/py_import.cpp
)

set (SOURCES4
        src/py_func.cpp
)

set (SOURCES5
        src/py_bindfunc.cpp
)

include_directories(.)
include_directories(./src)
include_directories(./src/lib)

include_directories(${PROJECT_SOURCE_DIR}/../max-sdk-7.3.3/source/c74support/max-includes)
#include_directories(${PROJECT_SOURCE_DIR}/../max-sdk-7.1.0/source/c74support/max-includes)
#include_directories(${PROJECT_SOURCE_DIR}/../maxSDK-6.1.4/c74support/max-includes)

find_package(PythonInterp 3 REQUIRED)
find_package(PythonLibs 3 REQUIRED)

INCLUDE_DIRECTORIES(${PYTHON_LIBRARIES})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})

add_subdirectory(pybind11)

set (CMAKE_OSX_ARCHITECTURES "x86_64" "i386")

###
add_library(py_shared SHARED ${LIB})
target_link_libraries(py_shared "-undefined dynamic_lookup")

###

add_library(py_eval MODULE ${SOURCES0})
set_property(TARGET py_eval PROPERTY CXX_STANDARD 11)

target_link_libraries(py_eval PRIVATE pybind11::embed)
target_link_libraries(py_eval py_shared)

target_link_libraries(py_eval ${LIBS} "-undefined dynamic_lookup")
set_target_properties(py_eval PROPERTIES PREFIX "")
set_target_properties(py_eval PROPERTIES SUFFIX ".mxo")
set_target_properties(py_eval PROPERTIES BUNDLE TRUE)
set_target_properties(py_eval PROPERTIES BUNDLE_EXTENSION "mxo")

install(TARGETS py_eval DESTINATION ${PROJECT_SOURCE_DIR}/bin)

set_target_properties(py_eval PROPERTIES OUTPUT_NAME "py.eval")

###

add_library(py_exec MODULE ${SOURCES1})
set_property(TARGET py_exec PROPERTY CXX_STANDARD 11)

target_link_libraries(py_exec PRIVATE pybind11::embed)
target_link_libraries(py_exec py_shared)

target_link_libraries(py_exec ${LIBS} "-undefined dynamic_lookup")
set_target_properties(py_exec PROPERTIES PREFIX "")
set_target_properties(py_exec PROPERTIES SUFFIX ".mxo")
set_target_properties(py_exec PROPERTIES BUNDLE TRUE)
set_target_properties(py_exec PROPERTIES BUNDLE_EXTENSION "mxo")

install(TARGETS py_exec DESTINATION ${PROJECT_SOURCE_DIR}/bin)

set_target_properties(py_exec PROPERTIES OUTPUT_NAME "py.exec")

####


add_library(py_module MODULE ${SOURCES2})
set_property(TARGET py_module PROPERTY CXX_STANDARD 11)

target_link_libraries(py_module PRIVATE pybind11::embed)
target_link_libraries(py_module py_shared)

target_link_libraries(py_module ${LIBS} "-undefined dynamic_lookup")
set_target_properties(py_module PROPERTIES PREFIX "")
set_target_properties(py_module PROPERTIES SUFFIX ".mxo")
set_target_properties(py_module PROPERTIES BUNDLE TRUE)
set_target_properties(py_module PROPERTIES BUNDLE_EXTENSION "mxo")

install(TARGETS py_exec DESTINATION ${PROJECT_SOURCE_DIR}/bin)

set_target_properties(py_module PROPERTIES OUTPUT_NAME "py.module")

###

add_library(py_import MODULE ${SOURCES3})
set_property(TARGET py_import PROPERTY CXX_STANDARD 11)

target_link_libraries(py_import PRIVATE pybind11::embed)
target_link_libraries(py_import py_shared)

target_link_libraries(py_import ${LIBS} "-undefined dynamic_lookup")
set_target_properties(py_import PROPERTIES PREFIX "")
set_target_properties(py_import PROPERTIES SUFFIX ".mxo")
set_target_properties(py_import PROPERTIES BUNDLE TRUE)
set_target_properties(py_import PROPERTIES BUNDLE_EXTENSION "mxo")

install(TARGETS py_import DESTINATION ${PROJECT_SOURCE_DIR}/bin)

set_target_properties(py_import PROPERTIES OUTPUT_NAME "py.import")

###

add_library(py_func MODULE ${SOURCES4})
set_property(TARGET py_func PROPERTY CXX_STANDARD 11)

target_link_libraries(py_func PRIVATE pybind11::embed)
target_link_libraries(py_func py_shared)

target_link_libraries(py_func ${LIBS} "-undefined dynamic_lookup")
set_target_properties(py_func PROPERTIES PREFIX "")
set_target_properties(py_func PROPERTIES SUFFIX ".mxo")
set_target_properties(py_func PROPERTIES BUNDLE TRUE)
set_target_properties(py_func PROPERTIES BUNDLE_EXTENSION "mxo")

install(TARGETS py_func DESTINATION ${PROJECT_SOURCE_DIR}/bin)

set_target_properties(py_func PROPERTIES OUTPUT_NAME "py.func")

###

add_library(py_bindfunc MODULE ${SOURCES5})
set_property(TARGET py_bindfunc PROPERTY CXX_STANDARD 11)

target_link_libraries(py_bindfunc PRIVATE pybind11::embed)
target_link_libraries(py_bindfunc py_shared)

target_link_libraries(py_bindfunc ${LIBS} "-undefined dynamic_lookup")
set_target_properties(py_bindfunc PROPERTIES PREFIX "")
set_target_properties(py_bindfunc PROPERTIES SUFFIX ".mxo")
set_target_properties(py_bindfunc PROPERTIES BUNDLE TRUE)
set_target_properties(py_bindfunc PROPERTIES BUNDLE_EXTENSION "mxo")

install(TARGETS py_bindfunc DESTINATION ${PROJECT_SOURCE_DIR}/bin)

set_target_properties(py_bindfunc PROPERTIES OUTPUT_NAME "py.bindfunc")

