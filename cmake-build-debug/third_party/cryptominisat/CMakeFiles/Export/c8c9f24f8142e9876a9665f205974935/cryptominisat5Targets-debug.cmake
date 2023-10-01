#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "cryptominisat5" for configuration "Debug"
set_property(TARGET cryptominisat5 APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(cryptominisat5 PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libcryptominisat5.so.5.11"
  IMPORTED_SONAME_DEBUG "libcryptominisat5.so.5.11"
  )

list(APPEND _cmake_import_check_targets cryptominisat5 )
list(APPEND _cmake_import_check_files_for_cryptominisat5 "${_IMPORT_PREFIX}/lib/libcryptominisat5.so.5.11" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
