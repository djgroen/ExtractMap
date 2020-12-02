find_package(PkgConfig)
pkg_search_module(PC_PROJ4 QUIET proj)

# # Try to use OSGeo4W installation
# if(WIN32)
#   set(PROJ4_OSGEO4W_HOME "C:/OSGeo4W") 

#   if($ENV{OSGEO4W_HOME})
#     set(PROJ4_OSGEO4W_HOME "$ENV{OSGEO4W_HOME}") 
#   endif()
# endif()

find_path(Proj4_INCLUDE_DIR proj_api.h
  PATH_SUFFIXES proj proj4 include/proj include/proj4 include
  HINTS ${PROJ4_OSGEO4W_HOME} ${PC_PROJ4_INCLUDEDIR} ${PC_PROJ4_INCLUDE_DIRS}
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /opt/local
  /opt)

# if (Proj4_INCLUDE_DIR)
#   # Extract version from proj_api.h (ex: 480)
#   file(STRINGS ${Proj4_INCLUDE_DIR}/proj_api.h
#     _PROJ4_VERSIONSTR
#     REGEX "#define[ ]+PJ_VERSION[ ]+[0-9]+")
#   string(REGEX MATCH "[0-9]+" _PROJ4_VERSIONSTR ${_PROJ4_VERSIONSTR})	

#   set(Proj4_VERSION ${_PROJ4_VERSIONSTR})
#   unset(_PROJ4_VERSIONSTR)
# endif()

find_library(Proj4_STATIC_LIBRARY Names proj.lib libproj.a
  PATH_SUFFIXES proj lib/proj lib
  HINTS ${PC_PROJ4_LIBDIR} ${PC_PROJ4_LIBRARY_DIRS}
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /opt/local
  /opt)

find_library(Proj4_DYNAMIC_LIBRARY Names libproj.dylib libproj.so
  PATH_SUFFIXES proj lib/proj lib
  HINTS ${PC_PROJ4_LIBDIR} ${PC_PROJ4_LIBRARY_DIRS}
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /opt/local
  /opt)

# message(STATUS "PROJ4: ${Proj4_INCLUDE_DIR} ${Proj4_STATIC_LIBRARY} ${Proj4_DYNAMIC_LIBRARY} ${PC_PROJ4_VERSION}")

set(Proj4_DEPENDENT_LIBRARIES ${PC_PROJ4_STATIC_LDFLAGS})
set(Proj4_LIBRARIES ${PC_PROJ4_LDFLAGS})
set(Proj4_VERSION ${PC_PROJ4_VERSION})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Proj4 DEFAULT_MSG
  Proj4_INCLUDE_DIR Proj4_STATIC_LIBRARY
  # VERSION_VAR Proj4_VERSION
  )
