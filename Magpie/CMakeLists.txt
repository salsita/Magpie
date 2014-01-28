cmake_minimum_required (VERSION 2.8.10) #required for extended generator expressions in add_custom_command
project (Magpie)

set(CMAKE_CONFIGURATION_TYPES Debug Release )
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Reset the configurations to those we need" FORCE)

#Variables describing project layout
set(${PROJECT_NAME}_GENERATED_FILES_DIR ${${PROJECT_NAME}_BINARY_DIR})
set(${PROJECT_NAME}_INC_DIR ${${PROJECT_NAME}_SOURCE_DIR} CACHE PATH "Path to ${PROJECT_NAME} include directory")
set(${PROJECT_NAME}_SRC_DIR ${${PROJECT_NAME}_SOURCE_DIR})
set(${PROJECT_NAME}_GEN_INC_DIR ${${PROJECT_NAME}_GENERATED_FILES_DIR} CACHE PATH "Path to ${PROJECT_NAME} include directory with generated files")
set(${PROJECT_NAME}_RC_DIR ${${PROJECT_NAME}_SOURCE_DIR})

include_directories(
  ${${PROJECT_NAME}_INC_DIR}
  ${libmagpie_INC_DIR}
  ${libmagpie_GEN_INC_DIR}
  ${libbhohelper_INC_DIR}
  ${${PROJECT_NAME}_GENERATED_FILES_DIR}
  )


set(${PROJECT_NAME}_HEADERS
  "${${PROJECT_NAME}_INC_DIR}/resource.h"
  "${${PROJECT_NAME}_INC_DIR}/dllmain.h"
  "${${PROJECT_NAME}_INC_DIR}/stdafx.h"
  "${${PROJECT_NAME}_INC_DIR}/targetver.h"
  )

set(${PROJECT_NAME}_SOURCES
  #"${${PROJECT_NAME}_SRC_DIR}/dlldata.c"
  "${${PROJECT_NAME}_SRC_DIR}/dllmain.cpp"
  "${${PROJECT_NAME}_SRC_DIR}/stdafx.cpp"
  "${${PROJECT_NAME}_SRC_DIR}/Magpie.cpp"
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
  "${${PROJECT_NAME}_SRC_DIR}/../libmagpie/Magpie.idl"
)
set_source_files_properties(
  ${${PROJECT_NAME}_IDL_FILE}
  PROPERTIES
    HEADER_FILE_ONLY 1
  )
source_group("IDL" FILES ${${PROJECT_NAME}_IDL_FILE})

################# GENERATED FILES #########################
set(${PROJECT_NAME}_MIDL_TLB_FILE "${${PROJECT_NAME}_GENERATED_FILES_DIR}/Magpie.tlb")

set(${PROJECT_NAME}_MIDL_I_H_FILE  "${${PROJECT_NAME}_GENERATED_FILES_DIR}/Magpie_i.h")

set(${PROJECT_NAME}_MIDL_I_C_FILE  "${${PROJECT_NAME}_GENERATED_FILES_DIR}/Magpie_i.c")

set(${PROJECT_NAME}_MIDL_P_C_FILE  "${${PROJECT_NAME}_GENERATED_FILES_DIR}/Magpie_p.c")

set(${PROJECT_NAME}_GENERATED_FILES
  "${${PROJECT_NAME}_MIDL_TLB_FILE}"
  "${${PROJECT_NAME}_MIDL_I_H_FILE}"
  "${${PROJECT_NAME}_MIDL_I_C_FILE}"
  #  "${${PROJECT_NAME}_MIDL_P_C_FILE}"
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
set(${PROJECT_NAME}_MIDL_ARGUMENTS
                     "${${PROJECT_NAME}_IDL_FILE}"
                     /D $<$<CONFIG:Debug>:"_DEBUG"> $<$<CONFIG:Release>:"NDEBUG"> $<$<CONFIG:RelWithDebInfo>:"NDEBUG">
                     /D "MODULES_HAVE_EXPORT_PROPERTY"
                     /nologo
                     /char signed
                     /env win32
                     /Oicf
                     /tlb "${${PROJECT_NAME}_MIDL_TLB_FILE}"
                     /h "${${PROJECT_NAME}_MIDL_I_H_FILE}"
                     /iid "${${PROJECT_NAME}_MIDL_I_C_FILE}"
                     /proxy "${${PROJECT_NAME}_MIDL_P_C_FILE}"
                     /error stub_data
                     )
#add_custom_command(OUTPUT ${${PROJECT_NAME}_GENERATED_FILES}
                   #COMMAND midl.exe ${${PROJECT_NAME}_MIDL_ARGUMENTS}
                   #WORKING_DIRECTORY "${${PROJECT_NAME}_GENERATED_FILES_DIR}"
                   #DEPENDS "${${PROJECT_NAME}_IDL_FILE}"
                   #)

################# DEF FILE #########################
set (${PROJECT_NAME}_DEF_FILES
  "${${PROJECT_NAME}_SRC_DIR}/Magpie.def"
)
source_group("DEF" FILES ${${PROJECT_NAME}_DEF_FILES})

################# RESOURCES #########################
set (${PROJECT_NAME}_RC_FILES
  ${${PROJECT_NAME}_RC_DIR}/Magpie.rc
  ${${PROJECT_NAME}_RC_DIR}/Magpie.rgs
  ${${PROJECT_NAME}_RC_DIR}/MagpieApplication.rgs
  ${${PROJECT_NAME}_RC_DIR}/MagpieSystem.rgs
)
source_group("Resources" FILES ${${PROJECT_NAME}_RC_FILES})

################# MAIN TARGET SETUP #########################

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_GENERATED_FILES}
  ${${PROJECT_NAME}_DEF_FILES}
  ${${PROJECT_NAME}_RC_FILES}
  )
target_link_libraries(${PROJECT_NAME} libbhohelper libmagpie)

# set UNICODE
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS _UNICODE,UNICODE)

# _WIN64
if (${CMAKE_CL_64})
  add_definitions(-DWIN64 -D_WIN64)
endif (${CMAKE_CL_64})

#Generating files in pre-build step
add_custom_command(TARGET ${PROJECT_NAME}
                   PRE_BUILD
                   COMMAND midl.exe ${${PROJECT_NAME}_MIDL_ARGUMENTS}
                   WORKING_DIRECTORY "${${PROJECT_NAME}_GENERATED_FILES_DIR}"
                   )

set(${PROJECT_NAME}_LIB_DIR "${${PROJECT_NAME}_BINARY_DIR}"  CACHE PATH "Path to ${PROJECT_NAME} lib directory")

message(STATUS "Configuring ${PROJECT_NAME}")

