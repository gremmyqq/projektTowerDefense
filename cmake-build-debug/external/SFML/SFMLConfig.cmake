# This script provides the SFML libraries as imported targets
# ------------------------------------
#
# Usage
# -----
#
# When you try to locate the SFML libraries, you must specify which modules you want to use (System, Window, Graphics, Network, Audio, Main).
# If none is given, no imported target will be created and you won't be able to link to SFML libraries.
# example:
#   find_package(SFML COMPONENTS Graphics Window System) # find the graphics, window and system modules
#
# You can enforce a specific version, either MAJOR.MINOR or only MAJOR.
# If nothing is specified, the version won't be checked (i.e. any version will be accepted).
# example:
#   find_package(SFML COMPONENTS ...)     # no specific version required
#   find_package(SFML 3 COMPONENTS ...)   # any 3.x version
#   find_package(SFML 3.0.1 COMPONENTS ...) # version 3.0.1 or greater with the same major version
#
# By default, the dynamic libraries of SFML will be found. To find the static ones instead,
# you must set the SFML_STATIC_LIBRARIES variable to ON before calling find_package(SFML ...).
# You don't need to deal with SFML's dependencies when linking your targets against SFML libraries,
# they will all be configured automatically, even if you use SFML static libraries.
# example:
#   set(SFML_STATIC_LIBRARIES ON)
#   find_package(SFML 3 COMPONENTS Network System)
#
# When searching for SFML with find_package(), keep in mind that it will also find versions which are
# in development (i.e. between two released versions), if you have them in your search path.
# If you want to make sure that a found SFML package corresponds to an official release, check the
# bool output variable SFML_VERSION_IS_RELEASE, which is true for official releases and false for development versions.
#
# On macOS by default CMake will search for frameworks. If you want to use static libraries and have installed
# both SFML frameworks and SFML static libraries, your must set CMAKE_FIND_FRAMEWORK to "NEVER" or "LAST"
# in addition to setting SFML_STATIC_LIBRARIES to ON. Otherwise CMake will check the frameworks bundle config and
# fail after finding out that it does not provide static libraries. Please refer to CMake documentation for more details.
#
# Additionally, keep in mind that SFML frameworks are only available as release libraries unlike dylibs which
# are available for both release and debug modes.
#
# If SFML is not installed in a standard path, you can use CMAKE_PREFIX_PATH to tell CMake in what directory
# SFML was installed.
#
# Output
# ------
#
# This script defines the following variables:
# - For each specified module XXX (System, Window, Graphics, Network, Audio, Main):
#   - SFML_XXX_FOUND:  true if either the debug or release library of the xxx module is found
# - SFML_FOUND:        true if all the required modules are found
#
# And the following targets:
# - For each specified module XXX (System, Window, Graphics, Network, Audio, Main):
#   - SFML::XXX
# The SFML targets are the same for both Debug and Release build configurations and will automatically provide
# correct settings based on your currently active build configuration. The SFML targets name also do not change
# when using dynamic or static SFML libraries.
#
# When linking against a SFML target, you do not need to specify indirect dependencies. For example, linking
# against SFML::Graphics will also automatically link against SFML::Window and SFML::System.
#
# example:
#   find_package(SFML 3 COMPONENTS Graphics Audio REQUIRED)
#   add_executable(myapp ...)
#   target_link_libraries(myapp PRIVATE SFML::Graphics SFML::Audio)

cmake_policy(VERSION 3.22...3.31)

if(NOT SFML_FIND_COMPONENTS)
    message(FATAL_ERROR "find_package(SFML) called with no component")
endif()

set(SFML_SUPPORTED_COMPONENTS Audio Graphics Main Network System Window)

foreach(component ${SFML_FIND_COMPONENTS})
    if(NOT component IN_LIST SFML_SUPPORTED_COMPONENTS)
        set(SFML_FOUND OFF)
        set(SFML_NOT_FOUND_MESSAGE "Unsupported SFML component: ${component}")
        message(FATAL_ERROR "${SFML_NOT_FOUND_MESSAGE}")
    endif()
endforeach()

set(FIND_SFML_PATHS
    "${CMAKE_CURRENT_LIST_DIR}/../../.."
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)

find_path(SFML_DOC_DIR SFML.tag
          PATH_SUFFIXES SFML/doc share/doc/SFML
          PATHS ${FIND_SFML_PATHS})

# Update requested components (eg. request window component if graphics component was requested)
set(FIND_SFML_COMPONENTS_SORTED "")
if("Audio" IN_LIST SFML_FIND_COMPONENTS)
    list(APPEND FIND_SFML_COMPONENTS_SORTED "System" "Audio")
endif()
if("Graphics" IN_LIST SFML_FIND_COMPONENTS)
    list(APPEND FIND_SFML_COMPONENTS_SORTED "System" "Window" "Graphics")
endif()
if("Main" IN_LIST SFML_FIND_COMPONENTS)
    list(APPEND FIND_SFML_COMPONENTS_SORTED "Main")
endif()
if("Network" IN_LIST SFML_FIND_COMPONENTS)
    list(APPEND FIND_SFML_COMPONENTS_SORTED "System" "Network")
endif()
if("System" IN_LIST SFML_FIND_COMPONENTS)
    list(APPEND FIND_SFML_COMPONENTS_SORTED "System")
endif()
if("Window" IN_LIST SFML_FIND_COMPONENTS)
    list(APPEND FIND_SFML_COMPONENTS_SORTED "System" "Window")
endif()
list(REMOVE_DUPLICATES FIND_SFML_COMPONENTS_SORTED)

# Choose which target definitions must be imported
if(SFML_STATIC_LIBRARIES)
    set(SFML_IS_FRAMEWORK_INSTALL "")
    if(SFML_IS_FRAMEWORK_INSTALL)
        message(WARNING "Static frameworks are not supported by SFML. Clear cache entries, \
and either change SFML_STATIC_LIBRARIES or CMAKE_FIND_FRAMEWORK before calling find_package(SFML)")
    endif()
    set(config_name "Static")
else()
    set(config_name "Shared")
endif()

# Set SFML_FOUND to ON by default, may be overwritten by one of the includes below
set(SFML_FOUND ON)

# Only configure dependencies if we are static linking
if(SFML_STATIC_LIBRARIES)
    # Look for dependencies in "reverse order"
    # This is due to the fact that among other things, resolving the
    # X11 dependency will break our own find_package(Freetype CONFIG)
    # because X11 attempts to find Freetype itself to resolve its Xft dependency.
    set(FIND_SFML_COMPONENTS_REVERSED ${FIND_SFML_COMPONENTS_SORTED})
    list(REVERSE FIND_SFML_COMPONENTS_REVERSED)

    foreach(component ${FIND_SFML_COMPONENTS_REVERSED})
        set(dependencies_file "${CMAKE_CURRENT_LIST_DIR}/SFML${component}Dependencies.cmake")
        if(EXISTS "${dependencies_file}")
            include("${dependencies_file}")
        endif()
    endforeach()
endif()

foreach(component ${FIND_SFML_COMPONENTS_SORTED})
    string(TOUPPER "${component}" UPPER_COMPONENT)
    set(targets_config_file "${CMAKE_CURRENT_LIST_DIR}/SFML${component}${config_name}Targets.cmake")

    # Generate imported targets for SFML components
    if(EXISTS "${targets_config_file}")
        include("${targets_config_file}")

        if(TARGET SFML::${component})
            set(SFML_${UPPER_COMPONENT}_FOUND ON)
        else()
            set(SFML_${UPPER_COMPONENT}_FOUND OFF)
            set(SFML_FOUND OFF)
            if(SFML_FIND_REQUIRED_${component})
                set(FIND_SFML_ERROR "Found SFML but requested component '${component}' is missing in the package configuration.")
            endif()
        endif()
    else()
        set(FIND_SFML_ERROR "Requested SFML configuration (${config_name}) was not found")
        if(config_name STREQUAL "Shared")
            string(APPEND FIND_SFML_ERROR "\nSet SFML_STATIC_LIBRARIES to ON for static libraries")
        elseif(config_name STREQUAL "Static")
            string(APPEND FIND_SFML_ERROR "\nSet SFML_STATIC_LIBRARIES to OFF for shared libraries")
        endif()
        set(SFML_${UPPER_COMPONENT}_FOUND OFF)
        set(SFML_FOUND OFF)
    endif()
endforeach()

if(SFML_FOUND)
    set(SFML_VERSION_IS_RELEASE OFF)
else()
    if(SFML_FIND_REQUIRED)
        # fatal error
        message(FATAL_ERROR "${FIND_SFML_ERROR}")
    elseif(NOT SFML_FIND_QUIETLY)
        # error but continue
        message(STATUS "${FIND_SFML_ERROR}")
    endif()
endif()

if(SFML_FOUND AND NOT SFML_FIND_QUIETLY)
    message(STATUS "Found SFML 3.1.0 in ${CMAKE_CURRENT_LIST_DIR}")
endif()
