cmake_minimum_required (VERSION 2.8)
project (MagpieProject)

find_path(libbhohelper_INC_DIR libbhohelper.h
  PATHS 
  "${${PROJECT_NAME}_SOURCE_DIR}/../libbhohelper/libbhohelper"
  "${${PROJECT_NAME}_SOURCE_DIR}/../../libbhohelper/libbhohelper"
  )

find_path(libbhohelper_LIB_DIR ../libbhohelper.lib)

link_directories(${${PROJECT_NAME}_SOURCE_DIR})

include_directories(
  ${libbhohelper_INC_DIR}
  )

add_subdirectory(libmagpie)

add_subdirectory(Magpie)
add_dependencies(Magpie libmagpie)

set(${PROJECT_NAME}_SRC_DIR ${${PROJECT_NAME}_SOURCE_DIR})
set(${PROJECT_NAME}_INC_DIRS 
  ${Magpie_INC_DIR}
  ${Magpie_GEN_INC_DIR}
  ${libmagpie_INC_DIR}
  ${libmagpie_GEN_INC_DIR}
  )

set(${PROJECT_NAME}_INC_DIRS ${${PROJECT_NAME}_INC_DIRS} PARENT_SCOPE)

message(STATUS "Configuring ${PROJECT_NAME}")
