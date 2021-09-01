# CMakeList.txt : CMake project for MrBoboGraphicsEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 20)
project ("MrBoboGraphicsEngine")


# TODO: Add tests and install targets if needed.

#Variablar
set(MBL ${CMAKE_SOURCE_DIR}/../MrBoboLibrary/)
set(EXTLIB ${CMAKE_SOURCE_DIR}/../MBExternalDependencies/)
set(EXTLIB_OUTDIR ${CMAKE_SOURCE_DIR}/ExternalOuts)
if(WIN32)
	set(EXTLIB_OUTDIR ${EXTLIB_OUTDIR}/Windows)
elseif(UNIX AND NOT APPLE)
	set(EXTLIB_OUTDIR ${EXTLIB_OUTDIR}/Linux)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(EXTLIB_OUTDIR ${EXTLIB_OUTDIR}/Debug)
else()
	set(EXTLIB_OUTDIR ${EXTLIB_OUTDIR}/Release)
endif()
#---------------------------------------------------

#Saker för att kunna linka med glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${EXTLIB}/glfw ${EXTLIB_OUTDIR}/glfw_out)
#för att builda med assimp
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) 
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE) 
add_subdirectory(${EXTLIB}/assimp-5.0.1 ${EXTLIB_OUTDIR}/assimp_out)
#---------------------------------------------------

#HeaderFiles
set(EXTHEADER_FILES
	${EXTLIB}/gl3w/include/GL/gl3w.h
)
set(HEADER_FILES 
	${MBL}/MBMatrix.h
	${CMAKE_SOURCE_DIR}
	${EXTHEADER_FILES}
	${EXTLIB}/stb/stb_image.h
)
#---------------------------------------------------

#HeaderFiles
set(EXTSOURCE_FILES
	${EXTLIB}/gl3w/src/gl3w.c	
	${EXTLIB}/stb/stb_image.cpp
)
set(SRC_FILES 
	${EXTSOURCE_FILES}
)
#---------------------------------------------------

# Add source to this project's executable.
add_executable (MrBoboGraphicsEngine "MrBoboGraphicsEngine.cpp" "MrBoboGraphicsEngine.h" "MBGE.h" "MBGE.cpp" ${HEADER_FILES} ${SRC_FILES})
#---------------------------------------------------

#Include Directorys
set (INCLUDE_DIRS
	${EXTLIB}/gl3w/include
	${EXTLIB}/assimp-5.0.1/include/assimp
)
target_include_directories(MrBoboGraphicsEngine PRIVATE ${MBL} ${CMAKE_SOURCE_DIR} ${EXTLIB} ${INCLUDE_DIRS})
#---------------------------------------------------
#Link grejer
#glfw
target_link_libraries(MrBoboGraphicsEngine glfw)
target_link_libraries(MrBoboGraphicsEngine assimp)