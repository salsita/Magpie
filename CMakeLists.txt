cmake_minimum_required (VERSION 2.8)
project (MagpieProject)

include_directories(
	${libbhohelper_INC_DIR}
	)

add_subdirectory(libmagpie)

add_subdirectory(Magpie)
add_dependencies(Magpie libmagpie)

set(${PROJECT_NAME}_SRC_DIR ${${PROJECT_NAME}_SOURCE_DIR})
set(${PROJECT_NAME}_INC_DIRS 
	${Magpie_INC_DIRS}
	${libmagpie_INC_DIRS}
	"${${PROJECT_NAME}_SOURCE_DIR}/include"
	)

set(${PROJECT_NAME}_INC_DIRS ${${PROJECT_NAME}_INC_DIRS} PARENT_SCOPE)

message(STATUS "Generated ${PROJECT_NAME}")
