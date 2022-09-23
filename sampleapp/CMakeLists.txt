include(../envWindows.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local envWindows.cmake: ${LOCAL_ENV}")

cmake_minimum_required(VERSION 3.11.0)
 
set(NAME SampleApp)
project(${NAME} VERSION 0.1.0) 

# Set VULKAN_SDK_PATH in .env.cmake to target specific vulkan version
if (DEFINED VULKAN_SDK_PATH)
  set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") # 1.1 Make sure this include path is correct
  set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib") # 1.2 Make sure lib path is correct
  set(Vulkan_FOUND "True")
else()
  find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
  message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif()

if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
else()
    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
endif()
 
# Set GLFW_PATH in .env.cmake to target specific glfw
if (DEFINED GLFW_PATH)
  message(STATUS "Using GLFW path specified in .env")
  set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")
  if (MSVC)
    set(GLFW_LIB "${GLFW_PATH}/lib-vc2022")
  endif()
else()
  find_package(glfw3 3.3 REQUIRED)
  set(GLFW_LIB glfw)
  message(STATUS "Found GLFW")
endif()

if (NOT GLFW_LIB)
    message(FATAL_ERROR "Could not find glfw library!")
else()
    message(STATUS "Using glfw lib at: ${GLFW_LIB}")
endif()

# Find spdlog
if (DEFINED SPDLOG_PATH)
  message(STATUS "Using SPDLOG path specified in .env")
  set(SPDLOG_INCLUDE_DIRS "${SPDLOG_PATH}/include")
  if (MSVC)
    set(SPDLOG_LIB "${SPDLOG_PATH}/build/Debug")
  endif()
else()
  find_package(spdlog REQUIRED)
  set(SPDLOG_LIB spdlog)
  message(STATUS "Found SPDLOG")
endif()

if (NOT SPDLOG_LIB)
    message(FATAL_ERROR "Could not find spdlog library!")
else()
    message(SPDLOG_LIB "Using spdlog lib at: ${SPDLOG_LIB}")
endif()
 
include_directories(external)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# add_compile_definitions(EM_IMPORT)
target_compile_definitions(${PROJECT_NAME} PRIVATE EM_IMPORT)
target_compile_definitions(${PROJECT_NAME} PRIVATE EM_ENABLE_ASSERTS)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build/debug")

if (WIN32)
  message(STATUS "CREATING BUILD FOR WINDOWS")
 
  target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${SPDLOG_INCLUDE_DIRS}
    "../emberlily/src"
    )

  target_link_directories(${PROJECT_NAME} PUBLIC
    ${Vulkan_LIBRARIES}
    ${GLFW_LIB}
    ${SPDLOG_LIB}
    "../emberlily/build/vs2022/Debug"
  )

  target_link_libraries(${PROJECT_NAME} glfw3 vulkan-1 spdlogd EmberLily)
elseif (UNIX)
    message(STATUS "CREATING BUILD FOR UNIX")
    target_include_directories(${PROJECT_NAME} PUBLIC
      ${PROJECT_SOURCE_DIR}/src
      ${TINYOBJ_PATH}
    )
    target_link_libraries(${PROJECT_NAME} glfw spdlog ${Vulkan_LIBRARIES})
endif()
