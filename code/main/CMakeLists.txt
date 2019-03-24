cmake_minimum_required(VERSION 2.6)

SET(PROJECT OrderServer)
IF(DEFINED  OPS_SOFT_VERSION )
	ADD_DEFINITIONS(-DOPS_VER_NUM="${OPS_SOFT_VERSION}")
	MESSAGE("OPS_VER_NUM  is " ${OPS_SOFT_VERSION})
	ELSE()
	MESSAGE("OPS_VER_NUM  is not set")
ENDIF()
# 库文件的输出路径
#源码
#aux_source_directory(. SRC_LIST)
FILE(GLOB_RECURSE SRC_LIST "*.c" "*.cpp" "*.h")

# 头文件
include_directories(
	.
	${DIR_INCLUDE}
	${DIR_INCLUDE}/commonModule

	)

message(STATUS "Current Dir: " ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "DIR_INCLUDE Dir: " ${DIR_INCLUDE})
#平台无关库
SET(COMMON_LIBS 
	debug	commonModule
	#debug   sqlitepp
	optimized	commonModule
	#optimized 	sqlitepp
	#commonModule
	)
	
IF(WIN32)
	#LIST(APPEND COMMON_LIBS 	
	#				
	#			)	
ELSE()
	LIST(APPEND COMMON_LIBS 
						#debug		curl

						#optimized	curl
				)	
ENDIF()


#平台特定库
IF(WIN32)	
	if(MSVC_VERSION EQUAL 1600)
		set(PLATFORM_LIBS commonModule)	
	elseif(MSVC_VERSION EQUAL 1800)
		set(PLATFORM_LIBS commonModule)
	elseif(MSVC_VERSION EQUAL 1911)
		set(PLATFORM_LIBS commonModule)
	endif()		
ELSEIF(UNIX)
	#SET(PLATFORM_LIBS pthread ssl crypto json)
ENDIF()

#编译
add_executable(${PROJECT} ${SRC_LIST})

#Debug版本后缀
set_target_properties(${PROJECT} PROPERTIES DEBUG_POSTFIX "d")

#连接库
target_link_libraries(${PROJECT}  ${COMMON_LIBS} ${PLATFORM_LIBS} ${PLATFORM_HW_LIBS})




