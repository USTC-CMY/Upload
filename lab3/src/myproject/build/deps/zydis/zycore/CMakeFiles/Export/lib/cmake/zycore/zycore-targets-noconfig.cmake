#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Zycore::Zycore" for configuration ""
set_property(TARGET Zycore::Zycore APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(Zycore::Zycore PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libZycore.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Zycore::Zycore )
list(APPEND _IMPORT_CHECK_FILES_FOR_Zycore::Zycore "${_IMPORT_PREFIX}/lib/libZycore.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
