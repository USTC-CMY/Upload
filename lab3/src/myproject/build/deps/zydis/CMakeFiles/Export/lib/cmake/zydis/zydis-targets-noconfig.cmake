#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Zydis::Zydis" for configuration ""
set_property(TARGET Zydis::Zydis APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(Zydis::Zydis PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libZydis.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Zydis::Zydis )
list(APPEND _IMPORT_CHECK_FILES_FOR_Zydis::Zydis "${_IMPORT_PREFIX}/lib/libZydis.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
