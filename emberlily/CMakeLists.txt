include(../envWindows.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local envWindows.cmake: ${LOCAL_ENV}")
 
cmake_minimum_required(VERSION 3.11.0)
 
set(NAME EmberLily)
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
 
include_directories(external)
 
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
 
add_library(${PROJECT_NAME} ${SOURCES})
target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if (CONFIG STREQUAL "Debug")
    message(STATUS "Creating debug build")
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build/debug")
endif()

 
if (WIN32)
  message(STATUS "CREATING BUILD FOR WINDOWS")
 
  target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    )

  target_link_directories(${PROJECT_NAME} PUBLIC
    ${Vulkan_LIBRARIES}
    ${GLFW_LIB}
  )
 
  target_link_libraries(${PROJECT_NAME} glfw3 vulkan-1)
elseif (UNIX)
    message(STATUS "CREATING BUILD FOR UNIX")
    target_include_directories(${PROJECT_NAME} PUBLIC
      ${PROJECT_SOURCE_DIR}/src
      ${TINYOBJ_PATH}
    )
    target_link_libraries(${PROJECT_NAME} glfw ${Vulkan_LIBRARIES})
endif()
 
 
############## Build SHADERS #######################
 
# Find all vertex and fragment sources within shaders directory
# taken from VBlancos vulkan tutorial
# https://github.com/vblanco20-1/vulkan-guide/blob/all-chapters/CMakeLists.txt
# find_program(GLSL_VALIDATOR glslangValidator HINTS 
#   ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} 
#   /usr/bin 
#   /usr/local/bin 
#   ${VULKAN_SDK_PATH}/Bin
#   ${VULKAN_SDK_PATH}/Bin32
#   $ENV{VULKAN_SDK}/Bin/
#   $ENV{VULKAN_SDK}/Bin32/
# )
 
# # get all .vert and .frag files in shaders directory
# file(GLOB_RECURSE GLSL_SOURCE_FILES
#   "${PROJECT_SOURCE_DIR}/shaders/*.frag"
#   "${PROJECT_SOURCE_DIR}/shaders/*.vert"
# )
 
# foreach(GLSL ${GLSL_SOURCE_FILES})
#   get_filename_component(FILE_NAME ${GLSL} NAME)
#   set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
#   add_custom_command(
#     OUTPUT ${SPIRV}
#     COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
#     DEPENDS ${GLSL})
#   list(APPEND SPIRV_BINARY_FILES ${SPIRV})
# endforeach(GLSL)
 
# add_custom_target(
#     Shaders
#     DEPENDS ${SPIRV_BINARY_FILES}
# )