cmake_minimum_required(VERSION 3.10)

project(2022CGFinalProject)

set(THIRD_PARTY_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/external)

include_directories(${THIRD_PARTY_LIBRARY_PATH}/glm)

include_directories(${THIRD_PARTY_LIBRARY_PATH}/glad/include)

include_directories(${THIRD_PARTY_LIBRARY_PATH}/glfw/include)

include_directories(${THIRD_PARTY_LIBRARY_PATH}/tiny_obj_loader)

include_directories(${THIRD_PARTY_LIBRARY_PATH}/imgui)

include_directories(${THIRD_PARTY_LIBRARY_PATH}/stb)

aux_source_directory(. PROJECT_SRC)

set(PROJECT_HDR ./texture_mapping.h)



file(GLOB BASE_HDR "../base/*.h")

file(GLOB BASE_SRC  "../base/*.cpp")


add_executable(2022CGFinalProject ${PROJECT_SRC} ${PROJECT_HDR} ${BASE_SRC} ${BASE_HDR})

if(WIN32)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
elseif(UNIX)
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Debug")
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Release")
endif()
endif()

target_link_libraries(2022CGFinalProject glm)

target_link_libraries(2022CGFinalProject glad)

target_link_libraries(2022CGFinalProject glfw)

target_link_libraries(2022CGFinalProject tiny_obj_loader)

target_link_libraries(2022CGFinalProject imgui)

target_link_libraries(2022CGFinalProject stb)