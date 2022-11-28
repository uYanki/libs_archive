
cmake_minimum_required(VERSION 3.5)
SET(EXTRA_COMPONENT_DIRS ${EXTRA_COMPONENT_DIRS} "./components/drivers")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(MPU_CTL)
