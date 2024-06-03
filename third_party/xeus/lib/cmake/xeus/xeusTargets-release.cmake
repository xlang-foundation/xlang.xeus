#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "xeus" for configuration "Release"
set_property(TARGET xeus APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(xeus PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libxeus.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libxeus.dll"
  )

list(APPEND _cmake_import_check_targets xeus )
list(APPEND _cmake_import_check_files_for_xeus "${_IMPORT_PREFIX}/lib/libxeus.lib" "${_IMPORT_PREFIX}/bin/libxeus.dll" )

# Import target "xeus-static" for configuration "Release"
set_property(TARGET xeus-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(xeus-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libxeus-static.lib"
  )

list(APPEND _cmake_import_check_targets xeus-static )
list(APPEND _cmake_import_check_files_for_xeus-static "${_IMPORT_PREFIX}/lib/libxeus-static.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
