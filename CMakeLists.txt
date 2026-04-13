cmake_minimum_required(VERSION 3.11)

project(ESMake LANGUAGES C VERSION 1.0)

option(BUILD_SHARED_LIBS "Build the libraries as shared libraries" ON)
option(BUILD_FOR_I586 "Build ${PROJECT_NAME} for i586 (Intel Pentium) or higher" OFF)
# Windows Compiler Options
option(BUILD_USING_MSVCRT20 "Build ${PROJECT_NAME} using msvcrt20 (Windows Compiler Only)" OFF)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVOARRAY_DEFCONFIG")
set(CMAKE_C_FLAGS_DEBUG "-g -Wall")
set(CMAKE_C_FLAGS_RELEASE "-O2")
if(NOT CMAKE_C_COMPILER_ID MATCHES "Clang")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
endif()

if(BUILD_FOR_I586)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=i586 -fcf-protection=none")
endif()

if(WIN32)
	if(BUILD_USING_MSVCRT20)
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcrtdll=msvcrt20")
	endif()
elseif(NOT WIN32 AND BUILD_USING_MSVCRT20)
	message(FATAL_ERROR "To build using msvcrt20 you must be using a Windows C compiler")
endif()

file(GLOB SOURCE_FILES "src/*.c")

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/src/progInfo.h.in
	${CMAKE_CURRENT_SOURCE_DIR}/src/progInfo.h
	@ONLY
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY BUILD_RPATH ".")
target_include_directories(${PROJECT_NAME} PRIVATE "src")

install(TARGETS ${PROJECT_NAME} DESTINATION bin)