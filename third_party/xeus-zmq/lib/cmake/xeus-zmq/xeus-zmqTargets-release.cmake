#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "xeus-zmq" for configuration "Release"
set_property(TARGET xeus-zmq APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(xeus-zmq PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libxeus-zmq.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libxeus-zmq.dll"
  )

list(APPEND _cmake_import_check_targets xeus-zmq )
list(APPEND _cmake_import_check_files_for_xeus-zmq "${_IMPORT_PREFIX}/lib/libxeus-zmq.lib" "${_IMPORT_PREFIX}/bin/libxeus-zmq.dll" )

# Import target "xeus-zmq-static" for configuration "Release"
set_property(TARGET xeus-zmq-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(xeus-zmq-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libxeus-zmq-static.lib"
  )

list(APPEND _cmake_import_check_targets xeus-zmq-static )
list(APPEND _cmake_import_check_files_for_xeus-zmq-static "${_IMPORT_PREFIX}/lib/libxeus-zmq-static.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
