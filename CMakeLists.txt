# 最顶层的cmake
# 需要定义：
# CMAKE_BUILD_TYPE=Debug/Release

IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Debug)	
ENDIF()

cmake_minimum_required(VERSION 2.6)
project(OrderSystem)

message(STATUS "project dir:" ${PROJECT_SOURCE_DIR})
message(STATUS "build type is:" ${CMAKE_BUILD_TYPE})
message(STATUS "platform is:" ${PLAT})

# 指定编译
if(WIN32)
	add_definitions(-DWIN32_LEAN_AND_MEAN)
elseif(UNIX)
	set(CMAKE_SHARED_LINKER_FLAGS 	"${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath=.")
	set(CMAKE_EXE_LINKER_FLAGS 		"${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath=.")
	add_definitions(-fPIC -DLINUX)
	if(PLAT STREQUAL "X86")	
		add_definitions(-m32)
		set(CMAKE_C_COMPILER gcc)
		set(CMAKE_CXX_COMPILER g++)
		set(CMAKE_SHARED_LINKER_FLAGS 	"${CMAKE_SHARED_LINKER_FLAGS} -m32")
		set(CMAKE_EXE_LINKER_FLAGS 		"${CMAKE_EXE_LINKER_FLAGS} -m32")
	elseif(PLAT STREQUAL "LINUX-X86_64")	
		add_definitions(-m64 -fpermissive)
		add_definitions(-DLINUX-X86_64)
		set(CMAKE_C_COMPILER gcc)
		set(CMAKE_CXX_COMPILER g++)
		set(CMAKE_SHARED_LINKER_FLAGS 	"${CMAKE_SHARED_LINKER_FLAGS} -m64")
		set(CMAKE_EXE_LINKER_FLAGS 		"${CMAKE_EXE_LINKER_FLAGS} -m64")
		ADD_DEFINITIONS(-DLINUX_X86_64)
	endif()
endif(WIN32)



#指定debug的共同选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DDEBUG)
	if(NOT WIN32)
		add_definitions(-g)
	endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_definitions(-O2)
else()
	message(FATAL_ERROR "请定义 CMAKE_BUILD_TYPE=Debug/Release")		
endif()

#设定全局选项
SET(CMAKE_SKIP_RPATH ON)

# 设置目录
set(DIR_INCLUDE ${PROJECT_SOURCE_DIR}/code/)
#set(DIR_SUB_INCLUDE ${PROJECT_SOURCE_DIR}/code/src)
set(DIR_3RD_PARTY ${PROJECT_SOURCE_DIR}/3rdparty)

if(WIN32)
	link_directories(${PROJECT_SOURCE_DIR}/lib_d/win-x86 ${PROJECT_SOURCE_DIR}/lib/win-x86 ${PROJECT_SOURCE_DIR}/bin/win-x86 ${PROJECT_SOURCE_DIR}/bin_d/win-x86)##这个还不能按config区分，所以debug和release路径都加上了	
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG 	${PROJECT_SOURCE_DIR}/lib_d/win-x86)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG 	${PROJECT_SOURCE_DIR}/bin_d/win-x86)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${PROJECT_SOURCE_DIR}/bin_d/win-x86)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE 	${PROJECT_SOURCE_DIR}/lib/win-x86)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE 	${PROJECT_SOURCE_DIR}/bin/win-x86)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	${PROJECT_SOURCE_DIR}/bin/win-x86)
elseif(UNIX)
	if(PLAT STREQUAL "X86")
		if(CMAKE_BUILD_TYPE STREQUAL "Debug")
			link_directories(${PROJECT_SOURCE_DIR}/lib_d/linux-x86 ${PROJECT_SOURCE_DIR}/bin_d/linux-x86)
			set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath-link=${PROJECT_SOURCE_DIR}/lib_d/linux-x86 -Wl,-rpath-link=${PROJECT_SOURCE_DIR}/bin_d/linux-x86")
			set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link=${PROJECT_SOURCE_DIR}/lib_d/linux-x86 -Wl,-rpath-link=${PROJECT_SOURCE_DIR}/bin_d/linux-x86")
			set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib_d/linux-x86)
			set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin_d/linux-x86)
			set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin_d/linux-x86)
		elseif(CMAKE_BUILD_TYPE STREQUAL "Release")		
			link_directories(${PROJECT_SOURCE_DIR}/lib/linux-x86 ${PROJECT_SOURCE_DIR}/bin/linux-x86)
			set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath-link=${PROJECT_SOURCE_DIR}/lib/linux-x86 -Wl,-rpath-link=${PROJECT_SOURCE_DIR}/bin/linux-x86")
			set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link=${PROJECT_SOURCE_DIR}/lib/linux-x86 -Wl,-rpath-link=${PROJECT_SOURCE_DIR}/bin/linux-x86")
			set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib/linux-x86)
			set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/linux-x86)
			set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/linux-x86)
		endif()
	elseif(PLAT STREQUAL "LINUX-X86_64")
		if(CMAKE_BUILD_TYPE STREQUAL "Debug")
			link_directories(${PROJECT_SOURCE_DIR}/lib_d/linux-x64 ${PROJECT_SOURCE_DIR}/bin_d/linux-x86_64)
			set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath-link=${PROJECT_SOURCE_DIR}/lib_d/linux-x64 -Wl,-rpath-link=${PROJECT_SOURCE_DIR}/bin_d/linux-x86_64")
			set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link=${PROJECT_SOURCE_DIR}/lib_d/linux-x64 -Wl,-rpath-link=${PROJECT_SOURCE_DIR}/bin_d/linux-x86_64")
			set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib_d/linux-x86_64)
			set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin_d/linux-x86_64)
			set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin_d/linux-x86_64)
		elseif(CMAKE_BUILD_TYPE STREQUAL "Release")		
			link_directories(${PROJECT_SOURCE_DIR}/lib/linux-x64 ${PROJECT_SOURCE_DIR}/bin/linux-x86_64)
			set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath-link=${PROJECT_SOURCE_DIR}/lib/linux-x64 -Wl,-rpath-link=${PROJECT_SOURCE_DIR}/bin/linux-x86_64")
			set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link=${PROJECT_SOURCE_DIR}/lib/linux-x64 -Wl,-rpath-link=${PROJECT_SOURCE_DIR}/bin/linux-x86_64")
			set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib/linux-x86_64)
			set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/linux-x86_64)
			set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/linux-x86_64)
		endif()
	endif()
endif(WIN32)

message(STATUS " Include dir: " ${DIR_INCLUDE})
message(STATUS " Build lib dir " ${LIBRARY_OUTPUT_PATH})
message(STATUS " Build exe dir" ${EXECUTABLE_OUTPUT_PATH})

# 代码所在子目录
if(WIN32)
	#add_subdirectory(code/src/VtMongoClient)
else()
endif()

add_subdirectory(code/main)
#add_subdirectory(code/sqlitepp)
add_subdirectory(code/commonModule)
add_subdirectory(code/communicationModule)
add_subdirectory(code/databaseModule)
add_subdirectory(code/simpleSockets)




