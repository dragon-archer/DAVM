cmake_minimum_required(VERSION 3.16)
project(DAVM VERSION 0.1 LANGUAGES CXX)

list(FIND CMAKE_CXX_COMPILE_FEATURES cxx_std_17 HAS_CXX17)
list(FIND CMAKE_CXX_COMPILE_FEATURES cxx_std_20 HAS_CXX20)
if(HAS_CXX20)
	set(CMAKE_CXX_STANDARD 20)
elseif(HAS_CXX17)
	set(CMAKE_CXX_STANDARD 17)
else()
	message(FATAL_ERROR "DAVM requires at least C++17 support")
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
if(DEFINED WITH_FMTLIB)
	message(STATUS "Use user specified fmtlib at ${WITH_FMTLIB}")
	include_directories(${WITH_FMTLIB})
endif()

set(COMMON_SRC
	common/asm.h
	common/base64.h
	common/log.h
	common/pch.h
	common/type.h
)
set(DAVM_SRC
	vm/main.cpp
	vm/vm.cpp
	vm/vm.h
)
set(DAVM_PCH vm/pch.h)

add_executable(davm ${DAVM_SRC} ${DAVM_PCH} ${COMMON_SRC})
target_precompile_headers(davm PRIVATE ${DAVM_PCH})

if(STATIC_BUILD)
	if(MSVC)
		set_property(GLOBAL PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -static -static-libgcc -static-libstdc++")
	endif()
endif()
