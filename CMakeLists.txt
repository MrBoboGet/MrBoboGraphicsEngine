project(MBGraphicsEngine)
set(PROJECT_NAME "MBGraphicsEngine")
##BEGIN MBPM_VARIABLES
set(MBPM_DEPENDENCIES 
	stb
	assimp
	irrxml
	zlib
	glfw
	MBUtility
	gl3w
)
set(MBPM_TARGET_EXTPACKET_LIBRARIES )
set(MBPM_TARGET_COMPILE_OPTIONS )
set(MBPM_TARGET_LINK_OPTIONS )
#MBPM_Functions#variables MBPM_DEPENDENCIES
#MBPM_TARGET_EXTPACKET_LIBRARIES
#MBPM_TARGET_COMPILE_OPTIONS
#MBPM_TARGET_LINK_OPTIONS
#Environment variables MBPM_PACKETS_INSTALL_DIRECTORY
set(MBPM_CXX_FLAGS ${CMAKE_CXX_FLAGS})
set(MBPM_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
set(MBPM_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
macro(MBPM_UpdateLibraries TargetToUpdate LinkType BuildType)
	set(MBPM_TARGET_EXTPACKET_LIBRARIES "")
	foreach(PACKET IN LISTS MBPM_DEPENDENCIES)
		set(NewLibraryName "")
		set(NewLibraryToAdd "$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/${PACKET}/MBPM_Builds/")
		if(${LinkType} STREQUAL "STATIC")
			set(NewLibraryName "${PACKET}_S")
		elseif(${LinkType} STREQUAL "DYNAMIC")
			set(NewLibraryName "${PACKET}_D")
		endif()
		if(${BuildType} STREQUAL "DEBUG")
			set(NewLibraryName "${NewLibraryName}D")
		elseif(${BuildType} STREQUAL "RELEASE")
			set(NewLibraryName "${NewLibraryName}R")
		endif()
		if(UNIX)
			set(NewLibraryName "lib${NewLibraryName}.")
			if(${LinkType} STREQUAL "STATIC")
				set(NewLibraryName "${NewLibraryName}a")
			else()
				set(NewLibraryName "${NewLibraryName}so")
			endif()
		else()
			if(${LinkType} STREQUAL "STATIC")
				set(NewLibraryName "${NewLibraryName}.lib")
			else()
				set(NewLibraryName "${NewLibraryName}.dll")				
			endif()
		endif()
		set(NewLibraryToAdd "${NewLibraryToAdd}${NewLibraryName}")
		list(APPEND MBPM_TARGET_EXTPACKET_LIBRARIES ${NewLibraryToAdd})
	endforeach()
endmacro()

macro(MBPM_UpdateTargetVariables TargetToUpdate LinkType BuildType)
	MBPM_UpdateLibraries(${TargetToUpdate} ${LinkType} ${BuildType})
endmacro()

macro(MBPM_UpdateOutput TargetToUpdate)
	set_target_properties(${TargetToUpdate} 
	PROPERTIES 
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/MBPM_Builds/"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/MBPM_Builds/"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/MBPM_Builds/"
	)
	#set_target_properties(${TargetToUpdate} PROPERTIES PREFIX "")
	#set_target_properties(${TargetToUpdate} PROPERTIES SUFFIX "")
	set_target_properties(${TargetToUpdate} PROPERTIES OUTPUT_NAME ${TargetToUpdate})
endmacro()

macro(MBPM_SetTargetCompileOptions TargetToUpdate LinkType BuildType)
	set(CMAKE_CXX_FLAGS " ")
	#set(NEW_TARGET_COMPILE_OPTIONS "")
	if(${BuildType} STREQUAL "DEBUG" AND ${LinkType} STREQUAL "STATIC")
		set(NEW_TARGET_COMPILE_OPTIONS "${MBPM_CXX_FLAGS} ${MBPM_CXX_FLAGS_DEBUG}")
		set(CMAKE_CXX_FLAGS_DEBUG " ")
	else()
		set(NEW_TARGET_COMPILE_OPTIONS "${MBPM_CXX_FLAGS} ${MBPM_CXX_FLAGS_RELEASE}")
		set(CMAKE_CXX_FLAGS_RELEASE " ")
	endif()
	string(REPLACE " " ";" NEW_TARGET_COMPILE_OPTIONS_LIST ${NEW_TARGET_COMPILE_OPTIONS})
	target_compile_options(${TargetToUpdate} PRIVATE ${NEW_TARGET_COMPILE_OPTIONS_LIST})
	message(${TargetToUpdate} " Compiled with follwing flags: ""${NEW_TARGET_COMPILE_OPTIONS_LIST}")
endmacro()

macro(MBPM_LinkTargetPacketLibraries Target)
	target_link_libraries(${Target} ${MBPM_TARGET_EXTPACKET_LIBRARIES})
endmacro()

macro(MBPM_SetTargetIncludeDirectories Target)
	target_include_directories(${Target} PUBLIC 
	"${CMAKE_CURRENT_SOURCE_DIR}/"
	"$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/"
	"$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/MBUtility/"
	)
endmacro()

macro(MBPM_ApplyTargetConfiguration TargetToUpdate LinkType BuildType)
	MBPM_UpdateTargetVariables(${TargetToUpdate} ${LinkType} ${BuildType})
	MBPM_UpdateOutput(${TargetToUpdate})
	MBPM_SetTargetCompileOptions(${TargetToUpdate} ${LinkType} ${BuildType})
	MBPM_SetTargetIncludeDirectories(${TargetToUpdate})
endmacro()
##END MBPM_VARIABLES
set(PROJECT_SOURCES 
	"${CMAKE_CURRENT_SOURCE_DIR}//MBGE.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}//MrBoboGraphicsEngine.cpp"
)
set(PROJECT_HEADERS 
	"${CMAKE_CURRENT_SOURCE_DIR}//MBGE.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}//MrBoboGraphicsEngine.h"
)
set(EXECUTABLE_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}//MrBoboGraphicsEngine.h"	
	"${CMAKE_CURRENT_SOURCE_DIR}//MrBoboGraphicsEngine.cpp"

)
set(COMMON_FILES ${PROJECT_SOURCES} ${PROJECT_HEADERS})
set(COMMON_DYNAMIC_LIBRARIES 
)
set(COMMON_STATIC_LIBRARIES 
	opengl32
)
set(COMMON_LIBRARIES ${COMMON_STATIC_LIBRARIES} ${COMMON_DYNAMIC_LIBRARIES})

add_library(MBGraphicsEngine_SD STATIC ${COMMON_FILES})
MBPM_ApplyTargetConfiguration("MBGraphicsEngine_SD" "STATIC" "DEBUG")
target_compile_features("MBGraphicsEngine_SD" PRIVATE cxx_std_17)
target_link_libraries("MBGraphicsEngine_SD" ${COMMON_LIBRARIES})
target_include_directories("MBGraphicsEngine_SD" PRIVATE "$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/assimp/include/")
target_include_directories("MBGraphicsEngine_SD" PRIVATE "$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/gl3w/include/")

add_library(MBGraphicsEngine_SR STATIC ${COMMON_FILES})
MBPM_ApplyTargetConfiguration("MBGraphicsEngine_SR" "STATIC" "RELEASE")
target_compile_features("MBGraphicsEngine_SR" PRIVATE cxx_std_17)
target_link_libraries("MBGraphicsEngine_SR" ${COMMON_LIBRARIES})
target_include_directories("MBGraphicsEngine_SR" PRIVATE "$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/assimp/include/")
target_include_directories("MBGraphicsEngine_SR" PRIVATE "$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/gl3w/include/")

add_executable(MBGETest ${COMMON_FILES} ${EXECUTABLE_FILES})
target_include_directories("MBGETest" PRIVATE "$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/assimp/include/")
target_include_directories("MBGETest" PRIVATE "$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/gl3w/include/")
MBPM_ApplyTargetConfiguration("MBGETest" "STATIC" "RELEASE")
target_compile_features("MBGETest" PRIVATE cxx_std_17)
target_link_libraries("MBGETest" ${COMMON_LIBRARIES})
MBPM_LinkTargetPacketLibraries("MBGETest")
