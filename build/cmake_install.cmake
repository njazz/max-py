# Install script for directory: /Users/njazz/Documents/github/max-py

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/njazz/Documents/github/max-py/bin/py.eval.mxo")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/njazz/Documents/github/max-py/bin" TYPE DIRECTORY FILES "/Users/njazz/Documents/github/max-py/build/py.eval.mxo" USE_SOURCE_PERMISSIONS)
  if(EXISTS "$ENV{DESTDIR}/Users/njazz/Documents/github/max-py/bin/py.eval.mxo/Contents/MacOS/py.eval" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/njazz/Documents/github/max-py/bin/py.eval.mxo/Contents/MacOS/py.eval")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/njazz/Documents/github/max-py/build/libpy_shared.dylib" "libpy_shared.dylib"
      "$ENV{DESTDIR}/Users/njazz/Documents/github/max-py/bin/py.eval.mxo/Contents/MacOS/py.eval")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/njazz/Documents/github/max-py/bin/py.exec.mxo")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/njazz/Documents/github/max-py/bin" TYPE DIRECTORY FILES "/Users/njazz/Documents/github/max-py/build/py.exec.mxo" USE_SOURCE_PERMISSIONS)
  if(EXISTS "$ENV{DESTDIR}/Users/njazz/Documents/github/max-py/bin/py.exec.mxo/Contents/MacOS/py.exec" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/njazz/Documents/github/max-py/bin/py.exec.mxo/Contents/MacOS/py.exec")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/njazz/Documents/github/max-py/build/libpy_shared.dylib" "libpy_shared.dylib"
      "$ENV{DESTDIR}/Users/njazz/Documents/github/max-py/bin/py.exec.mxo/Contents/MacOS/py.exec")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/njazz/Documents/github/max-py/bin/py.exec.mxo")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/njazz/Documents/github/max-py/bin" TYPE DIRECTORY FILES "/Users/njazz/Documents/github/max-py/build/py.exec.mxo" USE_SOURCE_PERMISSIONS)
  if(EXISTS "$ENV{DESTDIR}/Users/njazz/Documents/github/max-py/bin/py.exec.mxo/Contents/MacOS/py.exec" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/njazz/Documents/github/max-py/bin/py.exec.mxo/Contents/MacOS/py.exec")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/njazz/Documents/github/max-py/build/libpy_shared.dylib" "libpy_shared.dylib"
      "$ENV{DESTDIR}/Users/njazz/Documents/github/max-py/bin/py.exec.mxo/Contents/MacOS/py.exec")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/njazz/Documents/github/max-py/bin/py.import.mxo")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/njazz/Documents/github/max-py/bin" TYPE DIRECTORY FILES "/Users/njazz/Documents/github/max-py/build/py.import.mxo" USE_SOURCE_PERMISSIONS)
  if(EXISTS "$ENV{DESTDIR}/Users/njazz/Documents/github/max-py/bin/py.import.mxo/Contents/MacOS/py.import" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/njazz/Documents/github/max-py/bin/py.import.mxo/Contents/MacOS/py.import")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/njazz/Documents/github/max-py/build/libpy_shared.dylib" "libpy_shared.dylib"
      "$ENV{DESTDIR}/Users/njazz/Documents/github/max-py/bin/py.import.mxo/Contents/MacOS/py.import")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/njazz/Documents/github/max-py/bin/py.func.mxo")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/njazz/Documents/github/max-py/bin" TYPE DIRECTORY FILES "/Users/njazz/Documents/github/max-py/build/py.func.mxo" USE_SOURCE_PERMISSIONS)
  if(EXISTS "$ENV{DESTDIR}/Users/njazz/Documents/github/max-py/bin/py.func.mxo/Contents/MacOS/py.func" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/njazz/Documents/github/max-py/bin/py.func.mxo/Contents/MacOS/py.func")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/njazz/Documents/github/max-py/build/libpy_shared.dylib" "libpy_shared.dylib"
      "$ENV{DESTDIR}/Users/njazz/Documents/github/max-py/bin/py.func.mxo/Contents/MacOS/py.func")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/njazz/Documents/github/max-py/bin/py.bindfunc.mxo")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/njazz/Documents/github/max-py/bin" TYPE DIRECTORY FILES "/Users/njazz/Documents/github/max-py/build/py.bindfunc.mxo" USE_SOURCE_PERMISSIONS)
  if(EXISTS "$ENV{DESTDIR}/Users/njazz/Documents/github/max-py/bin/py.bindfunc.mxo/Contents/MacOS/py.bindfunc" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/njazz/Documents/github/max-py/bin/py.bindfunc.mxo/Contents/MacOS/py.bindfunc")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -change "/Users/njazz/Documents/github/max-py/build/libpy_shared.dylib" "libpy_shared.dylib"
      "$ENV{DESTDIR}/Users/njazz/Documents/github/max-py/bin/py.bindfunc.mxo/Contents/MacOS/py.bindfunc")
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/njazz/Documents/github/max-py/build/pybind11/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/njazz/Documents/github/max-py/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
