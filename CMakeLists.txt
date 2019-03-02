cmake_minimum_required (VERSION 2.8)
SET(CMAKE_CXX_FLAGS "-std=c++11")

project (max_py)

set (LIB
    src/lib/dyn_class.cpp
    src/lib/dyn_class.hpp
    src/lib/convert.cpp
    src/lib/convert.hpp
    src/lib/module.cpp
    src/lib/c_atom.cpp
    src/lib/py_box.cpp
    src/lib/e_object.cpp
    )

include_directories(.)
include_directories(./src)
include_directories(./src/lib)

if(APPLE)
add_definitions(-DMAC_VERSION)
endif()

if (WIN32)
add_definitions(-DWIN_VERSION)
add_definitions(-DNOMINMAX)
add_definitions(-DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE)
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
SET (LINK_FLAGS "${LINK_FLAGS} -shared --enable-auto-import")
endif()

# TODO: choose max sdk / submodule?
#include_directories(${PROJECT_SOURCE_DIR}/../max-sdk-7.3.3/source/c74support/max-includes)
#include_directories(${PROJECT_SOURCE_DIR}/../max-sdk-7.1.0/source/c74support/max-includes)
#include_directories(${PROJECT_SOURCE_DIR}/../maxSDK-6.1.4/c74support/max-includes)

include_directories(submodules/max-sdk/source/c74support/max-includes)

# TODO: python version
find_package(PythonInterp 3 REQUIRED)
find_package(PythonLibs 3 REQUIRED)

INCLUDE_DIRECTORIES(${PYTHON_LIBRARIES})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})

if (WIN32)
LINK_DIRECTORIES(${PYTHON_LIBRARIES})
endif()

add_subdirectory(submodules/pybind11)
INCLUDE_DIRECTORIES(submodules/pybind11/include)

set (CMAKE_OSX_ARCHITECTURES "x86_64" "i386")

###
add_library(py_shared SHARED ${LIB})

target_link_libraries(py_shared "-undefined dynamic_lookup")

if (WIN32)
    target_link_libraries(py_shared "-FORCE:UNRESOLVED")
endif()

target_link_libraries(py_shared ${PYTHON_LIBRARIES})

macro(py_external name_ext_)
    set(name_ py_${name_ext_})
    set(obj_ "py.${name_ext_}")
    add_library(${name_} MODULE src/${name_}.cpp)
    set_property(TARGET ${name_} PROPERTY CXX_STANDARD 11)

    target_link_libraries(${name_} PRIVATE pybind11::embed)
    target_link_libraries(${name_} PRIVATE py_shared)

    target_link_libraries(${name_} PRIVATE ${LIBS} "-undefined dynamic_lookup")
    set_target_properties(${name_} PROPERTIES PREFIX "")
    set_target_properties(${name_} PROPERTIES SUFFIX ".mxo")
    set_target_properties(${name_} PROPERTIES BUNDLE TRUE)
    set_target_properties(${name_} PROPERTIES BUNDLE_EXTENSION "mxo")

    install(TARGETS ${name_} DESTINATION ${PROJECT_SOURCE_DIR}/bin)

    set_target_properties(${name_} PROPERTIES OUTPUT_NAME ${obj_})
endmacro(py_external)

py_external("eval")
py_external("exec")
py_external("func")
py_external("module")
py_external("callback")

# TODO:

#py_external("msg")
#py_external("bindfunc")
#py_external("import")


###
add_definitions(-DWITH_COVERAGE)
enable_testing(true)
add_subdirectory(tests)
