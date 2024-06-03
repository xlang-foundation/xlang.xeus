# CMakeList.txt : CMake project for xlang jupyter kernel, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

#set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_CONFIGURATION_TYPES Release)

project (xlang_kernel)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
configure_file(${CMAKE_SOURCE_DIR}/dll/comerr64.dll ${CMAKE_BINARY_DIR}/bin COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/dll/gssapi64.dll ${CMAKE_BINARY_DIR}/bin COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/dll/k5sprt64.dll ${CMAKE_BINARY_DIR}/bin COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/dll/krb5_64.dll ${CMAKE_BINARY_DIR}/bin COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/dll/libcrypto-3-x64.dll ${CMAKE_BINARY_DIR}/bin COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/dll/libsodium.dll ${CMAKE_BINARY_DIR}/bin COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/dll/libxeus.dll ${CMAKE_BINARY_DIR}/bin COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/dll/libxeus-zmq.dll ${CMAKE_BINARY_DIR}/bin COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/dll/libzmq-mt-4_3_5.dll ${CMAKE_BINARY_DIR}/bin COPYONLY)

FILE(GLOB SRC src/*.cpp src/*.h)

add_executable (${PROJECT_NAME}	${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party
    ${CMAKE_SOURCE_DIR}/third_party/xeus/include
    ${CMAKE_SOURCE_DIR}/third_party/xeus-zmq/include
    ${CMAKE_SOURCE_DIR}/third_party/cppzmq/include
    ${CMAKE_SOURCE_DIR}/third_party/zeromq/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/third_party/xeus/lib/libxeus.lib
    ${CMAKE_SOURCE_DIR}/third_party/xeus-zmq/lib/libxeus-zmq.lib
    ${CMAKE_SOURCE_DIR}/third_party/zeromq/lib/libzmq.lib
    Ws2_32.lib
)

if (MSVC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DNOMINMAX)
    target_compile_options(${PROJECT_NAME} PUBLIC /DGUID_WINDOWS /MP /bigobj)
    target_compile_options(${PROJECT_NAME} PUBLIC /wd4251 /wd4996)
    target_compile_options(${PROJECT_NAME} PUBLIC /ZI /Od) #RelWithDebInfo for debug
    target_link_options(${PROJECT_NAME} PRIVATE /DEBUG /OPT:REF /OPT:ICF) #RelWithDebInfo for debug
elseif (APPLE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DGUID_CFUUID)
else ()
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DGUID_LIBUUID)
endif ()
