cmake_minimum_required(VERSION 2.6)

SET(PROJECT communicationModule)
IF(DEFINED  OPS_SOFT_VERSION )
	ADD_DEFINITIONS(-DOPS_VER_NUM="${OPS_SOFT_VERSION}")
	MESSAGE("OPS_VER_NUM  is " ${OPS_SOFT_VERSION})
	ELSE()
	MESSAGE("OPS_VER_NUM  is not set")
ENDIF()

# windows and linux thrift different version
# Attentions!!!! ${DIR_INCLUDE}/${VtThriftInclude} must in fron of ${DIR_INCLUDE}
# 头文件


# 库文件的输出路径
#源码
#aux_source_directory(. SRC_LIST)
FILE(GLOB_RECURSE SRC_LIST 
			"*.c" 
			"*.cpp" 
			"*.h" 
			"*.hpp" 
			"*.inl" 
			)

message(STATUS " CommonModule Include dir: " ${DIR_INCLUDE}/communicationModule)

# 头文件
IF(WIN32)	
	include_directories(
		.
		${DIR_INCLUDE}
		${DIR_INCLUDE}/commonModule
		${DIR_INCLUDE}/communicationModule
		${DIR_3RD_PARTY}
		)
ELSE()
	include_directories(
		.
		${DIR_INCLUDE}
		${DIR_INCLUDE}/commonModule
		${DIR_INCLUDE}/communicationModule
		${DIR_3RD_PARTY}
		)
ENDIF()

message(STATUS "Current Dir: " ${CMAKE_CURRENT_SOURCE_DIR})




#平台无关库
#SET(COMMON_LIBS 
#
#	debug		
#	optimized	 
#	)
	
#平台特定库
IF(WIN32)	
	if(MSVC_VERSION EQUAL 1600)
		set(PLATFORM_LIBS 		
		)	
	elseif(MSVC_VERSION EQUAL 1800)
		set(PLATFORM_LIBS 
			)
	elseif(MSVC_VERSION EQUAL 1911)
		set(PLATFORM_LIBS 
			)
	endif()
ELSE()
	SET(PLATFORM_LIBS )
ENDIF()


#编译
if(WIN32)
ADD_LIBRARY(${PROJECT} STATIC  ${SRC_LIST})
ELSE()
ADD_LIBRARY(${PROJECT} SHARED ${SRC_LIST})
ENDIF()

#连接
target_link_libraries(${PROJECT}  ${COMMON_LIBS} ${PLATFORM_LIBS})

#Debug版本输出文件名后缀
set_target_properties(${PROJECT} PROPERTIES DEBUG_POSTFIX "d")
