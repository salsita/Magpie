cmake_minimum_required (VERSION 2.8)
project (libmagpie)

set(${PROJECT_NAME}_GENERATED_FILES_DIR ${${PROJECT_NAME}_BINARY_DIR})
set(${PROJECT_NAME}_INC_DIRS 
	${${PROJECT_NAME}_SOURCE_DIR}
	${${PROJECT_NAME}_GENERATED_FILES_DIR}
	)
set(${PROJECT_NAME}_SRC_DIR ${${PROJECT_NAME}_SOURCE_DIR})

include_directories(
	${${PROJECT_NAME}_INC_DIRS}
	${libbhohelper_INC_DIR}
	)


set(${PROJECT_NAME}_HEADERS
	"${${PROJECT_NAME}_INC_DIR}/resource.h"
  "${${PROJECT_NAME}_INC_DIR}/dllmain.h"
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

set(${PROJECT_NAME}_MIDL_TLB_FILE "${${PROJECT_NAME}_GENERATED_FILES_DIR}/${PROJECT_NAME}.tlb")

set(${PROJECT_NAME}_MIDL_I_H_FILE	"${${PROJECT_NAME}_GENERATED_FILES_DIR}/${PROJECT_NAME}_i.h")

set(${PROJECT_NAME}_MIDL_I_C_FILE	"${${PROJECT_NAME}_GENERATED_FILES_DIR}/${PROJECT_NAME}_i.c")

set(${PROJECT_NAME}_MIDL_P_C_FILE	"${${PROJECT_NAME}_GENERATED_FILES_DIR}/${PROJECT_NAME}_p.c")

set(${PROJECT_NAME}_GENERATED_FILES
	"${${PROJECT_NAME}_MIDL_TLB_FILE}"
	"${${PROJECT_NAME}_MIDL_I_H_FILE}"
  "${${PROJECT_NAME}_MIDL_I_C_FILE}"
  "${${PROJECT_NAME}_MIDL_P_C_FILE}"
	)

set (${PROJECT_NAME}_IDL_FILE
  "${${PROJECT_NAME}_SOURCE_DIR}/Magpie.idl"
)

set_source_files_properties(
	${${PROJECT_NAME}_MIDL_TLB_FILE}
	${${PROJECT_NAME}_MIDL_I_H_FILE}
	${${PROJECT_NAME}_IDL_FILE}
	PROPERTIES
		HEADER_FILE_ONLY 1
		GENERATED 1
	)

add_custom_command(OUTPUT ${${PROJECT_NAME}_GENERATED_FILES}
                   COMMAND midl.exe "${${PROJECT_NAME}_IDL_FILE}"
                     /D "_DEBUG"
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
                   WORKING_DIRECTORY "${${PROJECT_NAME}_GENERATED_FILES_DIR}"
                   DEPENDS "${${PROJECT_NAME}_IDL_FILE}"
                   )

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_GENERATED_FILES})

# set UNICODE
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS _UNICODE,UNICODE)

# exclude default libraries
set_target_properties(${PROJECT_NAME} PROPERTIES STATIC_LIBRARY_FLAGS /NODEFAULTLIB)

set(${PROJECT_NAME}_INC_DIRS ${${PROJECT_NAME}_INC_DIRS} PARENT_SCOPE)

message(STATUS "Generated ${PROJECT_NAME}")
