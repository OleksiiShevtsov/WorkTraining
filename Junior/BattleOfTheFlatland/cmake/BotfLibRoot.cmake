# Resolve third-party root: sibling of the WorkTraining repository.
#
# Canonical layout:
#   <parent>/WorkTraining/Junior/BattleOfTheFlatland  →  <parent>/LIB
#
# Fallback (flat Cursor workspace):
#   <repo>/BattleOfTheFlatland  →  <repo>/LIB

if(NOT BOTF_LIB_ROOT)
    get_filename_component(_botf_lib_canonical "${CMAKE_SOURCE_DIR}/../../../LIB" ABSOLUTE)
    get_filename_component(_botf_lib_flat "${CMAKE_SOURCE_DIR}/../LIB" ABSOLUTE)

    if(EXISTS "${_botf_lib_canonical}/SFML" OR EXISTS "${_botf_lib_canonical}/README.md")
        set(_botf_lib_resolved "${_botf_lib_canonical}")
    elseif(EXISTS "${_botf_lib_flat}/SFML" OR EXISTS "${_botf_lib_flat}/README.md")
        set(_botf_lib_resolved "${_botf_lib_flat}")
    else()
        # Default to canonical path; setup.bat/setup.sh will create it.
        set(_botf_lib_resolved "${_botf_lib_canonical}")
    endif()

    set(BOTF_LIB_ROOT "${_botf_lib_resolved}" CACHE PATH
        "Third-party libraries directory (sibling of WorkTraining)")
endif()

message(STATUS "[botf] BOTF_LIB_ROOT=${BOTF_LIB_ROOT}")
