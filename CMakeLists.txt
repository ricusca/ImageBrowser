cmake_minimum_required (VERSION 3.6)

# currently the project supports only Windows operating system
SET(CMAKE_SYSTEM_NAME Windows)

project (ImageBrowser)


# Microsoft compiler 
MESSAGE(WARNING "Currently the only supported version of C++ compiler si Visual C++ 2015 due curl being compiled with it")
MESSAGE( STATUS "MSVC: " ${CMAKE_CXX_COMPILER_VERSION} )
MESSAGE( STATUS "MSVC: " ${MSVC14} )


# Set compiler flags and options. 
# Visual Studio warning level to 4 and unwind semantics
SET (CMAKE_CXX_FLAGS "/W4")
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

#add preprocessor macros
if(WIN32)
  add_definitions(-DWIN32)
endif(WIN32)
 add_definitions(-D_CONSOLE -DCURL_STATICLIB)

#include directories 
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ImageBrowser/curl/builds/libcurl-vc14-x86-release-static-ipv6-sspi-winssl/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ImageBrowser/jsoncpp/dist/)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ImageBrowser/SDL2-2.0.4/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ImageBrowser/SDL2_image-2.0.1/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ImageBrowser/SDL2_ttf-2.0.14/include)

#include source files
ADD_SUBDIRECTORY( ${PROJECT_SOURCE_DIR}/ImageBrowser )
ADD_SUBDIRECTORY (${PROJECT_SOURCE_DIR}/ImageBrowser/jsoncpp/dist)


file(GLOB_RECURSE jsoncpp     ${PROJECT_SOURCE_DIR}/ImageBrowser/jsoncpp/dist/*.h
							  ${PROJECT_SOURCE_DIR}/ImageBrowser/jsoncpp/dist/*.cpp
							  )

file(GLOB headers               ${PROJECT_SOURCE_DIR}/ImageBrowser/*.h)							  
							  
file(GLOB sources             ${PROJECT_SOURCE_DIR}/ImageBrowser/*.cpp)

#group jsoncpp in a separate filter
SOURCE_GROUP(jsoncpp FILES ${jsoncpp})

#include directories for libs
SET (CURL_LIB "/ImageBrowser/curl/builds/libcurl-vc14-x86-release-static-ipv6-sspi-winssl/lib")
if ("${MSVC11}" STREQUAL "1")
	SET(CURL_LIB, "/ImageBrowser/curl/builds/libcurl-vc11-x86-release-static-ipv6-sspi-winssl/lib")
elseif("${MSVC12}" STREQUAL "1")
	SET (CURL_LIB "/ImageBrowser/curl/builds/libcurl-vc12-x86-release-static-ipv6-sspi-winssl/lib")
endif()

MESSAGE( STATUS "CURL_LIB: " "${CURL_LIB}" )

LINK_DIRECTORIES(
				 ${PROJECT_SOURCE_DIR}${CURL_LIB}
				 ${PROJECT_SOURCE_DIR}/ImageBrowser/SDL2-2.0.4/lib/x86
				 ${PROJECT_SOURCE_DIR}/ImageBrowser/SDL2_image-2.0.1/lib/x86
				 ${PROJECT_SOURCE_DIR}/ImageBrowser/SDL2_ttf-2.0.14/lib/x86				 				
				)
				
#change output directory to bin
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
				
#and add the specified sources 
ADD_EXECUTABLE(ImageBrowser ${sources} ${headers} ${jsoncpp})				

#ignore linker warning because of curl
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ignore:4098")
				
#link libraries
TARGET_LINK_LIBRARIES(ImageBrowser libcurl_a.lib
								   SDL2.lib
								   SDL2main.lib
								   SDL2_image.lib
								   SDL2_ttf.lib
								   )
								   								 
MESSAGE( STATUS "XXX" ${PROJECT_BINARY_DIR} ) 
MESSAGE( STATUS "CMAKE_BUILD_TYPE" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ) 

								   
ADD_CUSTOM_COMMAND(TARGET ImageBrowser
                   POST_BUILD
                   COMMAND xcopy /C /F /R /Y /I \"$(ProjectDir)\\ImageBrowser\\SDL2-2.0.4\\lib\\x86\\SDL2.dll\" \"$(OutputPath)\"
				   COMMAND xcopy /C /F /R /Y /I \"$(ProjectDir)\\ImageBrowser\\SDL2_image-2.0.1\\lib\\x86\\SDL2_image.dll\" \"$(OutputPath)\"
				   COMMAND xcopy /C /F /R /Y /I \"$(ProjectDir)\\ImageBrowser\\SDL2_image-2.0.1\\lib\\x86\\libpng16-16.dll\" \"$(OutputPath)\"
				   COMMAND xcopy /C /F /R /Y /I \"$(ProjectDir)\\ImageBrowser\\SDL2_ttf-2.0.14\\lib\\x86\\*.dll\" \"$(OutputPath)\"
				   COMMAND xcopy /C /F /R /Y /I \"$(ProjectDir)\\fonts\\*.ttf\" \"$(OutputPath)\\fonts\"
				   )		   

#set startup project. Available since version 3.6
SET_PROPERTY(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ImageBrowser)