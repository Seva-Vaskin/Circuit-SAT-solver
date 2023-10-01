# Install script for directory: /home/vsevolod/spbu/Circuit-SAT-solver/third_party/cryptominisat

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
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptominisat5" TYPE FILE FILES "/home/vsevolod/spbu/Circuit-SAT-solver/cmake-build-debug/third_party/cryptominisat//CMakeFiles/cryptominisat5ConfigVersion.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptominisat5" TYPE FILE FILES "/home/vsevolod/spbu/Circuit-SAT-solver/cmake-build-debug/third_party/cryptominisat//CMakeFiles/cryptominisat5Config.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptominisat5/cryptominisat5Targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptominisat5/cryptominisat5Targets.cmake"
         "/home/vsevolod/spbu/Circuit-SAT-solver/cmake-build-debug/third_party/cryptominisat/CMakeFiles/Export/c8c9f24f8142e9876a9665f205974935/cryptominisat5Targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptominisat5/cryptominisat5Targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptominisat5/cryptominisat5Targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptominisat5" TYPE FILE FILES "/home/vsevolod/spbu/Circuit-SAT-solver/cmake-build-debug/third_party/cryptominisat/CMakeFiles/Export/c8c9f24f8142e9876a9665f205974935/cryptominisat5Targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptominisat5" TYPE FILE FILES "/home/vsevolod/spbu/Circuit-SAT-solver/cmake-build-debug/third_party/cryptominisat/CMakeFiles/Export/c8c9f24f8142e9876a9665f205974935/cryptominisat5Targets-debug.cmake")
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/vsevolod/spbu/Circuit-SAT-solver/cmake-build-debug/third_party/cryptominisat/cmsat5-src/cmake_install.cmake")
  include("/home/vsevolod/spbu/Circuit-SAT-solver/cmake-build-debug/third_party/cryptominisat/tests/cmake_install.cmake")
  include("/home/vsevolod/spbu/Circuit-SAT-solver/cmake-build-debug/third_party/cryptominisat/scripts/fuzz/cmake_install.cmake")
  include("/home/vsevolod/spbu/Circuit-SAT-solver/cmake-build-debug/third_party/cryptominisat/utils/minimal_cms/cmake_install.cmake")
  include("/home/vsevolod/spbu/Circuit-SAT-solver/cmake-build-debug/third_party/cryptominisat/utils/lingeling-ala/cmake_install.cmake")

endif()

