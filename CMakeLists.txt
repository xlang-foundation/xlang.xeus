# CMakeList.txt : CMake project for xlang jupyter kernel, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("xlang_kernel")

# depends
set(NLOHMANN_JSON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/nlohmann_json)
set(XTL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/xtl)

include_directories(${NLOHMANN_JSON_DIR}/include)
include_directories(${XTL_DIR}/include)

set(XEUS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/xeus)
set(XEUS_INCLUDE_DIR ${XEUS_DIR}/include)
set(XEUS_SOURCE_DIR ${XEUS_DIR}/src)

include_directories(${XEUS_INCLUDE_DIR})

set(XEUS_HEADERS
    ${XEUS_INCLUDE_DIR}/xeus/xcomm.hpp
    ${XEUS_INCLUDE_DIR}/xeus/xcontrol_messenger.hpp
    ${XEUS_INCLUDE_DIR}/xeus/xdebugger.hpp
    ${XEUS_INCLUDE_DIR}/xeus/xeus.hpp
    ${XEUS_INCLUDE_DIR}/xeus/xeus_context.hpp
    ${XEUS_INCLUDE_DIR}/xeus/xguid.hpp
    ${XEUS_INCLUDE_DIR}/xeus/xhistory_manager.hpp
    ${XEUS_INCLUDE_DIR}/xeus/xinput.hpp
    ${XEUS_INCLUDE_DIR}/xeus/xinterpreter.hpp
    ${XEUS_INCLUDE_DIR}/xeus/xkernel.hpp
    ${XEUS_INCLUDE_DIR}/xeus/xkernel_configuration.hpp
    ${XEUS_INCLUDE_DIR}/xeus/xlogger.hpp
    ${XEUS_INCLUDE_DIR}/xeus/xmessage.hpp
    ${XEUS_INCLUDE_DIR}/xeus/xhelper.hpp
    ${XEUS_INCLUDE_DIR}/xeus/xserver.hpp
    ${XEUS_INCLUDE_DIR}/xeus/xstring_utils.hpp
    ${XEUS_INCLUDE_DIR}/xeus/xsystem.hpp
)

set(XEUS_SOURCES
    ${XEUS_SOURCE_DIR}/xcomm.cpp
    ${XEUS_SOURCE_DIR}/xcontrol_messenger.cpp
    ${XEUS_SOURCE_DIR}/xdebugger.cpp
    ${XEUS_SOURCE_DIR}/xguid.cpp
    ${XEUS_SOURCE_DIR}/xhistory_manager.cpp
    ${XEUS_SOURCE_DIR}/xinput.cpp
    ${XEUS_SOURCE_DIR}/xin_memory_history_manager.hpp
    ${XEUS_SOURCE_DIR}/xin_memory_history_manager.cpp
    ${XEUS_SOURCE_DIR}/xinterpreter.cpp
    ${XEUS_SOURCE_DIR}/xkernel.cpp
    ${XEUS_SOURCE_DIR}/xkernel_configuration.cpp
    ${XEUS_SOURCE_DIR}/xkernel_core.cpp
    ${XEUS_SOURCE_DIR}/xkernel_core.hpp
    ${XEUS_SOURCE_DIR}/xlogger.cpp
    ${XEUS_SOURCE_DIR}/xlogger_impl.hpp
    ${XEUS_SOURCE_DIR}/xlogger_impl.cpp
    ${XEUS_SOURCE_DIR}/xmessage.cpp
    ${XEUS_SOURCE_DIR}/xhelper.cpp
    ${XEUS_SOURCE_DIR}/xmock_interpreter.cpp
    ${XEUS_SOURCE_DIR}/xmock_interpreter.hpp
    ${XEUS_SOURCE_DIR}/xserver.cpp
    ${XEUS_SOURCE_DIR}/xsystem.cpp
)

include_directories("lib/")

FILE(GLOB SRC "src/*.cpp")

add_compile_definitions(XEUS_STATIC_LIB)

add_executable (${PROJECT_NAME}
	${XEUS_SOURCES}
	${SRC}
)

if (MSVC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DNOMINMAX)
    target_compile_options(${PROJECT_NAME} PUBLIC /DGUID_WINDOWS /MP /bigobj)
    target_compile_options(${PROJECT_NAME} PUBLIC /wd4251 /wd4996)
elseif (APPLE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DGUID_CFUUID)
else ()
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DGUID_LIBUUID)
endif ()
