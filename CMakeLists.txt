# CMakeList.txt : CMake project for MrBoboGraphicsEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 20)
project ("MrBoboGraphicsEngine")


# TODO: Add tests and install targets if needed.

#Variablar
set(MBL ${CMAKE_SOURCE_DIR}/../MyLibrary/)
set(EXTLIB ${CMAKE_SOURCE_DIR}/../ExternaLibrarys/)
#---------------------------------------------------

#Saker för att kunna linka med glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#setter buildet i vårt, antar att det är bättre så vi inte fuckar upp massa andra projekt om vi alltid gör det i samma?
add_subdirectory(${EXTLIB}/glfw ${CMAKE_SOURCE_DIR}/glfw_out)
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
)
target_include_directories(MrBoboGraphicsEngine PRIVATE ${MBL} ${CMAKE_SOURCE_DIR} ${EXTLIB} ${INCLUDE_DIRS})
#---------------------------------------------------
#Link grejer
#glfw
target_link_libraries(MrBoboGraphicsEngine glfw)