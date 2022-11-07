cmake_minimum_required(VERSION 3.8)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

project(WEngine)

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT WEngine)

#add_definitions(-DOPENGL)

set(GLAD_INCLUDE ${CMAKE_SOURCE_DIR}/Engine/deps/glad/include)

file(GLOB MATH_SRC Engine/Math/*.cpp)
file(GLOB_RECURSE GLM_SRC Engine/deps/GLM *.h *.hpp *.inl)

file(GLOB ICON_SRC Icons/*.*)

file(GLOB PASS_SRC Engine/Render/Passes/Private/*.*)
file(GLOB PIPELINE_SRC Engine/Render/RenderPipeline/Private/*.*)
file(GLOB DESCR_SRC Engine/Render/Descriptor/Private/*.*)
file(GLOB MESH_SRC Engine/Render/Mesh/Private/*.*)
file(GLOB RENDER_SRC Engine/Render/Private/*.*)

file(GLOB FRAME_SRC Engine/Framework/Private/*.*)
file(GLOB COMP_SRC Engine/Scene/Components/Private/*.*)
file(GLOB SCENE_SRC Engine/Scene/Private/*.*)
file(GLOB UTILS_SRC Engine/Utils/Private/*.*)
file(GLOB ALLOC_SRC Engine/Utils/Allocator/Private/*.*)
file(GLOB EVENT_SRC Engine/Event/Private/*.*)
file(GLOB EDITOR_SRC Engine/Editor/Private/*.*)
file(GLOB ROOT_SRC Engine/Sources/*.cpp Engine/Sources/*.h)
file(GLOB LOG_SRC Engine/Log/Private/*.*)
file(GLOB RHI_SRC Engine/RHI/Private/*.*)
file(GLOB RHI_ENCODER_SRC Engine/RHI/Encoder/Private/*.*)
file(GLOB GUI_SRC Engine/Utils/ImGui/Private/*.*)
file(GLOB REFLECTION_SRC Engine/Utils/Reflection/Private/*.*)
file(GLOB GLFW_SRC Platform/GLFWWindow/Private/*.*)
file(GLOB WIN_SRC Platform/Windows/Private/*.*)
file(GLOB VULKAN_SRC Platform/Vulkan/Private/*.*)
file(GLOB VULKAN_ENCODER_SRC Platform/Vulkan/Encoder/Private/*.*)
file(GLOB VULKAN_ALLOC_SRC Platform/Vulkan/Allocator/Private/*.*)
file(GLOB VULKAN_GUI_SRC Platform/Vulkan/ImGui/Private/*.*)

file(GLOB PASS_INCLUDE Engine/Render/Passes/Public/*.h)
file(GLOB PIPELINE_INCLUDE Engine/Render/RenderPipeline/Public/*.h)
file(GLOB DESCR_INCLUDE Engine/Render/Descriptor/Public/*.h)
file(GLOB MESH_INCLUDE Engine/Render/Mesh/Public/*.h)
file(GLOB RENDER_INCLUDE Engine/Render/Public/*.h)

file(GLOB CORE_INCLUDE Engine/Core/*.h)
file(GLOB FRAME_INCLUDE Engine/Framework/Public/*.h)
file(GLOB COMP_INCLUDE Engine/Scene/Components/Public/*.h)
file(GLOB SCENE_INCLUDE Engine/Scene/Public/*.h)
file(GLOB UTILS_INCLUDE Engine/Utils/Public/*.h)
file(GLOB ALLOC_INCLUDE Engine/Utils/Allocator/Public/*.h)
file(GLOB EVENT_INCLUDE Engine/Event/Public/*.h)
file(GLOB EDITOR_INCLUDE Engine/Editor/Public/*.h)
file(GLOB LOG_INCLUDE Engine/Log/Public/*.h)
file(GLOB RHI_INCLUDE Engine/RHI/Public/*.h)
file(GLOB RHI_ENCODER_INCLUDE Engine/RHI/Encoder/Public/*.h)
file(GLOB GUI_INCLUDE Engine/Utils/ImGui/Public/*.h)
file(GLOB REFLECTION_INCLUDE Engine/Utils/Reflection/Public/*.h)
file(GLOB GLFW_INCLUDE Platform/GLFWWindow/Public/*.h)
file(GLOB WIN_INCLUDE Platform/Windows/Public/*.h)
file(GLOB VULKAN_INCLUDE Platform/Vulkan/Public/*.h)
file(GLOB VULKAN_ENCODER_INCLUDE Platform/Vulkan/Encoder/Public/*.h)
file(GLOB VULKAN_ALLOC_INCLUDE Platform/Vulkan/Allocator/Public/*.h)
file(GLOB VULKAN_GUI_INCLUDE Platform/Vulkan/ImGui/Public/*.h)

file(GLOB EXAMPLE_INCLUDE Example/Public/*.h)
file(GLOB EXAMPLE_SRC Example/Private/*.cpp)
file(GLOB EXAMPLE_RESOURCE_INCLUDE Example/Resources/Public/*.h)
file(GLOB EXAMPLE_RESOURCE_SRC Example/Resources/Private/*.cpp)

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

set(PLATFORM_SRC
    ${GLFW_SRC}
    ${VULKAN_SRC}
    ${VULKAN_ENCODER_SRC}
    ${VULKAN_ALLOC_SRC}
    ${VULKAN_GUI_SRC})

set(PLATFORM_INCLUDE
    ${GLFW_INCLUDE}
    ${WIN_INCLUDE}
    ${VULKAN_INCLUDE}
    ${VULKAN_ENCODER_INCLUDE}
    ${VULKAN_ALLOC_INCLUDE}
    ${VULKAN_GUI_INCLUDE})

set(WE_SRC
    ${FRAME_SRC}
    ${MATH_SRC}
    ${RENDER_SRC}
    ${DESCR_SRC}
    ${COMP_SRC}
    ${SCENE_SRC}
    ${UTILS_SRC}
    ${ALLOC_SRC}
    ${EVENT_SRC}
    ${EDITOR_SRC}
    ${ROOT_SRC}
    ${PLATFORM_SRC}
    ${PASS_SRC}
    ${PIPELINE_SRC}
    ${MESH_SRC}
    ${LOG_SRC}
    ${RHI_SRC}
    ${RHI_ENCODER_SRC}
    ${GUI_SRC}
    ${REFLECTION_SRC}
    ${GLM_SRC})

set(WE_INCLUDE
    ${CORE_INCLUDE}
    ${FRAME_INCLUDE}
    ${MATH_INCLUDE}
    ${RENDER_INCLUDE}
    ${DESCR_INCLUDE}
    ${COMP_INCLUDE}
    ${SCENE_INCLUDE}
    ${UTILS_INCLUDE}
    ${ALLOC_INCLUDE}
    ${EVENT_INCLUDE}
    ${EDITOR_INCLUDE}
    ${PLATFORM_INCLUDE}
    ${PASS_INCLUDE}
    ${PIPELINE_INCLUDE}
    ${MESH_INCLUDE}
    ${LOG_INCLUDE}
    ${RHI_INCLUDE}
    ${RHI_ENCODER_INCLUDE}
    ${GUI_INCLUDE}
    ${REFLECTION_INCLUDE})

set(EXA_INCLUDE
    ${EXAMPLE_INCLUDE}
    ${EXAMPLE_RESOURCE_INCLUDE})

set(EXA_SRC
    ${EXAMPLE_SRC}
    ${EXAMPLE_RESOURCE_SRC})

list(APPEND WENGINE_INCLUDE ${GLAD_INCLUDE})
list(APPEND WENGINE_INCLUDE ${DEPS_INCLUDE})

source_group(Engine/Framework/Private FILES ${FRAME_SRC})
source_group(Engine/Math FILES ${MATH_SRC})
source_group(Engine/Render/Private FILES ${RENDER_SRC})
source_group(Engine/Render/Descriptor/Private FILES ${DESCR_SRC})
source_group(Engine/Render/Passes/Private FILES ${PASS_SRC})
source_group(Engine/Render/RenderPipeline/Private FILES ${PIPELINE_SRC})
source_group(Engine/Utils/Private FILES ${UTILS_SRC})
source_group(Engine/Utils/Allocator/Private FILES ${ALLOC_SRC})
source_group(Engine/Sources FILES ${ROOT_SRC})
source_group(Engine/Scene/Components/Private FILES ${COMP_SRC})
source_group(Engine/Scene/Private FILES ${SCENE_SRC})
source_group(Engine/Event/Private FILES ${EVENT_SRC})
source_group(Engine/Editor/Private FILES ${EDITOR_SRC})
source_group(Engine/Log/Private FILES ${LOG_SRC})
source_group(Engine/Render/Pass/Private FILES ${PASS_SRC})
source_group(Engine/Render/Mesh/Private FILES ${MESH_SRC})
source_group(Engine/RHI/Private FILES ${RHI_SRC})
source_group(Engine/RHI/Encoder/Private FILES ${RHI_ENCODER_SRC})
source_group(Engine/Utils/ImGui/Private FILES ${GUI_SRC})
source_group(Engine/Utils/Reflection/Private FILES ${REFLECTION_SRC})
source_group(Platform/GLFWWindow/Private FILES ${GLFW_SRC})
source_group(Platform/Vulkan/Private FILES ${VULKAN_SRC})
source_group(Platform/Vulkan/Encoder/Private FILES ${VULKAN_ENCODER_SRC})
source_group(Platform/Vulkan/Allocator/Private FILES ${VULKAN_ALLOC_SRC})
source_group(Platform/Vulkan/ImGui/Private FILES ${VULKAN_GUI_SRC})

source_group(Engine/Core FILES ${CORE_INCLUDE})
source_group(Engine/Framework/Public FILES ${FRAME_INCLUDE})
source_group(Engine/Math/Public FILES ${MATH_INCLUDE})
source_group(Engine/Render/Public FILES ${RENDER_INCLUDE})
source_group(Engine/Render/Descriptor/Public FILES ${DESCR_INCLUDE})
source_group(Engine/Render/Passes/Public FILES ${PASS_INCLUDE})
source_group(Engine/Render/RenderPipeline/Public FILES ${PIPELINE_INCLUDE})
source_group(Engine/Utils/Public FILES ${UTILS_INCLUDE})
source_group(Engine/Utils/Allocator/Public FILES ${ALLOC_INCLUDE})
source_group(Engine/Scene/Components/Public FILES ${COMP_INCLUDE})
source_group(Engine/Scene/Public FILES ${SCENE_INCLUDE})
source_group(Engine/Event/Public FILES ${EVENT_INCLUDE})
source_group(Engine/Editor/Public FILES ${EDITOR_INCLUDE})
source_group(Engine/Render/Pass/Public FILES ${PASS_INCLUDE})
source_group(Engine/Render/Mesh/Public FILES ${MESH_INCLUDE})
source_group(Engine/Log/Public FILES ${LOG_INCLUDE})
source_group(Engine/RHI/Public FILES ${RHI_INCLUDE})
source_group(Engine/RHI/Encoder/Public FILES ${RHI_ENCODER_INCLUDE})
source_group(Engine/Utils/ImGui/Public FILES ${GUI_INCLUDE})
source_group(Engine/Utils/Reflection/Public FILES ${REFLECTION_INCLUDE})
source_group(Platform/GLFWWindow/Public FILES ${GLFW_INCLUDE})
source_group(Platform/Vulkan/Public FILES ${VULKAN_INCLUDE})
source_group(Platform/Vulkan/Encoder/Public FILES ${VULKAN_ENCODER_INCLUDE})
source_group(Platform/Vulkan/Allocator/Public FILES ${VULKAN_ALLOC_INCLUDE})
source_group(Platform/Vulkan/ImGui/Public FILES ${VULKAN_GUI_INCLUDE})

source_group(Example/Public FILES ${EXAMPLE_INCLUDE})
source_group(Example/Private FILES ${EXAMPLE_SRC})
source_group(Example/Resources/Public FILES ${EXAMPLE_RESOURCE_INCLUDE})
source_group(Example/Resources/Private FILES ${EXAMPLE_RESOURCE_SRC})

link_directories(Engine/deps/Vulkan/Lib)
link_directories(${CMAKE_BINARY_DIR}/Engine/deps/imgui/Debug)

add_subdirectory(Engine/deps/imgui)

add_executable(WEngine ${WE_SRC} ${EXA_SRC} ${WE_INCLUDE} ${EXA_INCLUDE})
use_precompiled_header(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/Sources/pch.h" "${CMAKE_SOURCE_DIR}/Sources/pch.cpp")

set(GLFW_DIR ${CMAKE_SOURCE_DIR}/Engine/deps/glfw)

include_directories(${CMAKE_SOURCE_DIR}/Platform)
include_directories(${CMAKE_SOURCE_DIR}/Engine)
include_directories(${CMAKE_SOURCE_DIR}/Engine/Sources)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/Engine/deps)
include_directories(${CMAKE_SOURCE_DIR}/Engine/deps/imgui)
include_directories(${CMAKE_SOURCE_DIR}/Engine/deps/Vulkan/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/assets)
add_subdirectory(${CMAKE_SOURCE_DIR}/assets)
add_subdirectory(${GLFW_DIR})

list(APPEND WENGINE_INCLUDE ${GLFW3_DIR}/include)

target_include_directories(WEngine PUBLIC ${WENGINE_INCLUDE})
target_link_libraries(WEngine glfw)
target_link_libraries(WEngine "vulkan-1.lib")
target_link_libraries(WEngine "ImGui.lib")

file(GLOB_RECURSE ASSET_LIST_PATH ${CMAKE_SOURCE_DIR}/assets/*.*)

add_custom_target(
    copy_asset_files
    ${CMAKE_COMMAND} -E copy_if_different ${ASSET_LIST_PATH} ${CMAKE_CURRENT_BINARY_DIR}/assets)

add_dependencies(WEngine copy_asset_files)