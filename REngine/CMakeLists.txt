cmake_minimum_required (VERSION 3.18)

project(REngine)

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT REngine)


file(GLOB GLFW_SRC deps/GLFW/include/*.h)
file(GLOB_RECURSE GLM_SRC deps/GLM *.h *.hpp *.inl)
file(GLOB WINDOW_SRC Engine/Window/*.*)
file(GLOB RENDERER_SRC Engine/Renderer/*.*)
file(GLOB APPLICATION_SRC Engine/Application/*.*)
file(GLOB LOG_SRC Engine/Log/*.*)
file(GLOB OBJECT_SRC Engine/Object/*.*)
file(GLOB UTILS_SRC Engine/Utils/*.*)
file(GLOB SRC Engine/Sources/*.*)

file(GLOB WINDOW_INCLUDE Engine/Window/*.h)
file(GLOB RENDERER_INCLUDE Engine/Renderer/*.h)
file(GLOB APPLICATION_INCLUDE Engine/Application/*.h)
file(GLOB_RECURSE VULKAN_INCLUDE deps/Vulkan/include/*.h)

set(R_SRC
    ${GLFW_SRC}
    ${GLM_SRC}
    ${APPLICATION_SRC}
    ${WINDOW_SRC}
    ${RENDERER_SRC}
    ${LOG_SRC}
    ${OBJECT_SRC}
    ${UTILS_SRC}
    ${SRC})

set(R_INCLUDE
    ${APPLICATION_INCLUDE}
    ${WINDOW_INCLUDE}
    ${RENDERER_INCLUDE})

source_group(Engine/Application FILES ${APPLICATION_SRC})
source_group(Engine/Window FILES ${WINDOW_SRC})
source_group(Engine/Renderer FILES ${RENDERER_SRC})
source_group(Deps/GLFW FILES ${GLFW_SRC})
source_group(Engine/Log FILES ${LOG_SRC})
source_group(Engine/Object FILES ${OBJECT_SRC})
source_group(Engine/Utils FILES ${UTILS_SRC})
source_group(Engine/Sources FILES ${SRC})

macro(use_precompiled_header TARGET HEADER_FILE SRC_FILE)
  get_filename_component(HEADER ${HEADER_FILE} NAME)
  # Use MSVC_IDE to exclude NMake from using PCHs
  if (MSVC AND NOT NMAKE AND NOT OGRE_UNITY_BUILD)
    set_target_properties(${TARGET} PROPERTIES COMPILE_FLAGS /Yc"${HEADER}")
    set_source_files_properties(${SRC_FILE}
      PPROPERTIES COMPILE_FLAGS /Yu"${HEADER}"
    )
  elseif (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
  endif ()
endmacro(use_precompiled_header)

link_directories(deps/GLFW/lib)
link_directories(deps/Vulkan/Lib)
add_executable(REngine ${R_SRC})
target_link_libraries(REngine "glfw3.lib")
target_link_libraries(REngine "vulkan-1.lib")

use_precompiled_header(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/Sources/pch.h" "${CMAKE_SOURCE_DIR}/Sources/pch.cpp")

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/Engine)
include_directories(${CMAKE_SOURCE_DIR}/Engine/Sources)
include_directories(${CMAKE_SOURCE_DIR}/deps)
include_directories(${CMAKE_SOURCE_DIR}/deps/GLFW/include)
include_directories(${CMAKE_SOURCE_DIR}/deps/Vulkan/include)

file(GLOB SHADER_LISTS ${CMAKE_SOURCE_DIR}/Shaders/*.spv)

add_custom_target(
  copy_shaders
  ${CMAKE_COMMAND} -E copy_if_different ${SHADER_LISTS} ${CMAKE_CURRENT_BINARY_DIR}/Shaders)

add_dependencies(REngine copy_shaders)