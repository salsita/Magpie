cmake_minimum_required (VERSION 2.8)
project (Magpie)

set(${PROJECT_NAME}_GENERATED_FILES_DIR ${${PROJECT_NAME}_BINARY_DIR})
set(${PROJECT_NAME}_INC_DIRS 
	${${PROJECT_NAME}_SOURCE_DIR}
	${libmagpie_INC_DIRS}
	${${PROJECT_NAME}_GENERATED_FILES_DIR}
	)
set(${PROJECT_NAME}_SRC_DIR ${${PROJECT_NAME}_SOURCE_DIR})
set(${PROJECT_NAME}_RC_DIR ${${PROJECT_NAME}_SOURCE_DIR})

include_directories(
	${${PROJECT_NAME}_INC_DIRS}
	${libbhohelper_INC_DIR}
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

set(${PROJECT_NAME}_MIDL_TLB_FILE "${${PROJECT_NAME}_GENERATED_FILES_DIR}/Magpie.tlb")

set(${PROJECT_NAME}_MIDL_I_H_FILE	"${${PROJECT_NAME}_GENERATED_FILES_DIR}/Magpie_i.h")

set(${PROJECT_NAME}_MIDL_I_C_FILE	"${${PROJECT_NAME}_GENERATED_FILES_DIR}/Magpie_i.c")

set(${PROJECT_NAME}_MIDL_P_C_FILE	"${${PROJECT_NAME}_GENERATED_FILES_DIR}/Magpie_p.c")

set(${PROJECT_NAME}_GENERATED_FILES
	"${${PROJECT_NAME}_MIDL_TLB_FILE}"
	"${${PROJECT_NAME}_MIDL_I_H_FILE}"
  "${${PROJECT_NAME}_MIDL_I_C_FILE}"
	#  "${${PROJECT_NAME}_MIDL_P_C_FILE}"
	)

set (${PROJECT_NAME}_IDL_FILE
  "${${PROJECT_NAME}_SRC_DIR}/../libmagpie/Magpie.idl"
)

set (${PROJECT_NAME}_DEF_FILES
  "${${PROJECT_NAME}_SRC_DIR}/Magpie.def"
)

set (${PROJECT_NAME}_RC_FILES
  ${${PROJECT_NAME}_RC_DIR}/Magpie.rc
  ${${PROJECT_NAME}_RC_DIR}/Magpie.rgs
  ${${PROJECT_NAME}_RC_DIR}/MagpieApplication.rgs
  ${${PROJECT_NAME}_RC_DIR}/MagpieSystem.rgs
)

set_source_files_properties(
	${${PROJECT_NAME}_MIDL_TLB_FILE}
	${${PROJECT_NAME}_MIDL_I_H_FILE}
	${${PROJECT_NAME}_IDL_FILE}
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


add_library(${PROJECT_NAME} SHARED 
	${${PROJECT_NAME}_SOURCES} 
	${${PROJECT_NAME}_GENERATED_FILES}
	${${PROJECT_NAME}_DEF_FILES}
	${${PROJECT_NAME}_RC_FILES}
	)
target_link_libraries(${PROJECT_NAME} libbhohelper libmagpie)

# set UNICODE
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS _UNICODE,UNICODE)

set(${PROJECT_NAME}_INC_DIRS ${${PROJECT_NAME}_INC_DIRS} PARENT_SCOPE)

message(STATUS "Generated ${PROJECT_NAME}")

