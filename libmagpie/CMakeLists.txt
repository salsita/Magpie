cmake_minimum_required (VERSION 2.8.10) #required for extended generator expressions in add_custom_command
project (libmagpie)

set(CMAKE_CONFIGURATION_TYPES Debug Release )
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Reset the configurations to those we need" FORCE)

#Variables describing project layout
set(${PROJECT_NAME}_GENERATED_FILES_DIR ${${PROJECT_NAME}_BINARY_DIR})
set(${PROJECT_NAME}_INC_DIR ${${PROJECT_NAME}_SOURCE_DIR} CACHE PATH "Path to ${PROJECT_NAME} include directory")
set(${PROJECT_NAME}_GEN_INC_DIR ${${PROJECT_NAME}_GENERATED_FILES_DIR} CACHE PATH "Path to ${PROJECT_NAME} include directory with generated files")
set(${PROJECT_NAME}_SRC_DIR ${${PROJECT_NAME}_SOURCE_DIR})

include_directories(
  ${${PROJECT_NAME}_INC_DIR}
  ${${PROJECT_NAME}_GENERATED_FILES_DIR}
  ${libbhohelper_INC_DIR}
  )


set(${PROJECT_NAME}_HEADERS
  "${${PROJECT_NAME}_INC_DIR}/resource.h"
  "${${PROJECT_NAME}_INC_DIR}/stdafx.h"
  "${${PROJECT_NAME}_INC_DIR}/targetver.h"
  "${${PROJECT_NAME}_INC_DIR}/ActiveScriptDebugT.h"
  "${${PROJECT_NAME}_INC_DIR}/ActiveScriptT.h"
  "${${PROJECT_NAME}_INC_DIR}/atlextensions.h"
  "${${PROJECT_NAME}_INC_DIR}/DispExIdManager.h"
  "${${PROJECT_NAME}_INC_DIR}/MagpieActiveScript.h"
  "${${PROJECT_NAME}_INC_DIR}/MagpieApplication.h"
  "${${PROJECT_NAME}_INC_DIR}/MagpieConsole.h"
  "${${PROJECT_NAME}_INC_DIR}/MagpieFilesystemScriptLoader.h"
  "${${PROJECT_NAME}_INC_DIR}/MagpieModule.h"
  "${${PROJECT_NAME}_INC_DIR}/MagpieModuleExports.h"
  "${${PROJECT_NAME}_INC_DIR}/MagpieRequire.h"
  "${${PROJECT_NAME}_INC_DIR}/MagpieResourceScriptLoader.h"
  "${${PROJECT_NAME}_INC_DIR}/Misc.h"
  "${${PROJECT_NAME}_INC_DIR}/_IMagpieLoggerEvents_CP.H"
  )

set(${PROJECT_NAME}_SOURCES 
  "${${PROJECT_NAME}_SRC_DIR}/DispExIdManager.cpp"
  "${${PROJECT_NAME}_SRC_DIR}/MagpieActiveScript.cpp"
  "${${PROJECT_NAME}_SRC_DIR}/MagpieApplication.cpp"
  "${${PROJECT_NAME}_SRC_DIR}/MagpieConsole.cpp"
  "${${PROJECT_NAME}_SRC_DIR}/MagpieFilesystemScriptLoader.cpp"
  "${${PROJECT_NAME}_SRC_DIR}/MagpieModule.cpp"
  "${${PROJECT_NAME}_SRC_DIR}/MagpieModuleExports.cpp"
  "${${PROJECT_NAME}_SRC_DIR}/MagpieRequire.cpp"
  "${${PROJECT_NAME}_SRC_DIR}/MagpieResourceScriptLoader.cpp"
  "${${PROJECT_NAME}_SRC_DIR}/Misc.cpp"
  "${${PROJECT_NAME}_SRC_DIR}/stdafx.cpp"
  )

# precompiled headers
foreach( src_file ${${PROJECT_NAME}_SOURCES} )
        set_source_files_properties(
            ${src_file}
            PROPERTIES
            COMPILE_FLAGS "/Yustdafx.h"
            )
endforeach( src_file ${${PROJECT_NAME}_SOURCES} )
set_source_files_properties("${${PROJECT_NAME}_SRC_DIR}/stdafx.cpp" PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
################# IDL FILE #########################
set (${PROJECT_NAME}_IDL_FILE
  "${${PROJECT_NAME}_SOURCE_DIR}/Magpie.idl"
)
set_source_files_properties(
  ${${PROJECT_NAME}_IDL_FILE}
  PROPERTIES
    HEADER_FILE_ONLY 1
  )
source_group("IDL" FILES ${${PROJECT_NAME}_IDL_FILE})

################# GENERATED FILES #########################

set(${PROJECT_NAME}_MIDL_TLB_FILE "${${PROJECT_NAME}_GENERATED_FILES_DIR}/${PROJECT_NAME}.tlb")

set(${PROJECT_NAME}_MIDL_I_H_FILE  "${${PROJECT_NAME}_GENERATED_FILES_DIR}/Magpie_i.h")

set(${PROJECT_NAME}_MIDL_I_C_FILE  "${${PROJECT_NAME}_GENERATED_FILES_DIR}/Magpie_i.c")

set(${PROJECT_NAME}_MIDL_P_C_FILE  "${${PROJECT_NAME}_GENERATED_FILES_DIR}/Magpie_p.c")

set(${PROJECT_NAME}_GENERATED_FILES
  "${${PROJECT_NAME}_MIDL_TLB_FILE}"
  "${${PROJECT_NAME}_MIDL_I_H_FILE}"
  "${${PROJECT_NAME}_MIDL_I_C_FILE}"
  "${${PROJECT_NAME}_MIDL_P_C_FILE}"
  )
source_group("Generated files" FILES ${${PROJECT_NAME}_GENERATED_FILES})

set_source_files_properties(
  ${${PROJECT_NAME}_MIDL_TLB_FILE}
  ${${PROJECT_NAME}_MIDL_I_H_FILE}
  PROPERTIES
    HEADER_FILE_ONLY 1
    GENERATED 1
  )

set_source_files_properties(
  ${${PROJECT_NAME}_MIDL_I_C_FILE}
  ${${PROJECT_NAME}_MIDL_P_C_FILE}
  PROPERTIES
    GENERATED 1
  )

# MIDL
set(MIDL_ENV_ARG win32)
if (${CMAKE_CL_64})
  set(MIDL_ENV_ARG win64)
endif (${CMAKE_CL_64})

set(${PROJECT_NAME}_MIDL_ARGUMENTS 
                     "${${PROJECT_NAME}_IDL_FILE}"
                     /D $<$<CONFIG:Debug>:"_DEBUG"> $<$<CONFIG:Release>:"NDEBUG">
                     /D "MODULES_HAVE_EXPORT_PROPERTY"
                     /nologo
                     /char signed
                     /env ${MIDL_ENV_ARG}
                     /Oicf
                     /tlb "${${PROJECT_NAME}_MIDL_TLB_FILE}"
                     /h "${${PROJECT_NAME}_MIDL_I_H_FILE}"
                     /iid "${${PROJECT_NAME}_MIDL_I_C_FILE}"
                     /proxy "${${PROJECT_NAME}_MIDL_P_C_FILE}"
                     /error stub_data
                     )
#add_custom_command(OUTPUT ${${PROJECT_NAME}_GENERATED_FILES}
#                   COMMAND midl.exe ${${PROJECT_NAME}_MIDL_ARGUMENTS}
#                   WORKING_DIRECTORY "${${PROJECT_NAME}_GENERATED_FILES_DIR}"
#                   DEPENDS "${${PROJECT_NAME}_IDL_FILE}"
#                   )

################# MAIN TARGET SETUP #########################

add_library(${PROJECT_NAME} STATIC 
  ${${PROJECT_NAME}_SOURCES} 
  ${${PROJECT_NAME}_HEADERS} 
  ${${PROJECT_NAME}_GENERATED_FILES}
  )

# set UNICODE
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS _UNICODE,UNICODE)

# exclude default libraries
set_target_properties(${PROJECT_NAME} PROPERTIES STATIC_LIBRARY_FLAGS /NODEFAULTLIB)

#Generating files in pre-build step
add_custom_command(TARGET ${PROJECT_NAME}
                   PRE_BUILD
                   COMMAND midl.exe ${${PROJECT_NAME}_MIDL_ARGUMENTS}
                   WORKING_DIRECTORY "${${PROJECT_NAME}_GENERATED_FILES_DIR}"
                   )


set(${PROJECT_NAME}_LIB_DIR "${${PROJECT_NAME}_BINARY_DIR}"  CACHE PATH "Path to ${PROJECT_NAME} lib directory")

message(STATUS "Configuring ${PROJECT_NAME}")
