cmake_minimum_required(VERSION 3.8)

project(SoftRenderer)

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SoftRenderer)

set(CORE_SRC
    "Core/Application.h"
    "Core/Application.cpp")

set(MATH_SRC
    "Math/Matrix.h"
    "Math/Vector.h")

set(RENDERER_SRC
    "Render/Rasterizer.h"
    "Render/Rasterizer.cpp"
    "Render/Renderer.h"
    "Render/Renderer.cpp"
    "Render/Framebuffer.h"
    "Render/Framebuffer.cpp"
    "Render/Vertex.h")

set(COMP_SRC
    "Components/Component.h"
    "Components/Material.h"
    "Components/Material.cpp"
    "Components/Mesh.h"
    "Components/Mesh.cpp"
    "Components/Transformer.h"
    "Components/Transformer.cpp")

set(SCENE_SRC
    "Scene/World.h"
    "Scene/World.cpp"
    "Scene/Entity.h"
    "Scene/Entity.cpp"
    "Scene/Light.h"
    "Scene/Light.cpp")

set(CAMERA_SRC
    "Camera/Camera.h"
    "Camera/Camera.cpp")

set(UTILS_SRC
    "Utils/Shader.h"
    "Utils/Shader.cpp"
    "Utils/Window.h"
    "Utils/Window.cpp"
    "Utils/OBJLoader.h"
    "Utils/OBJLoader.cpp"
    "Utils/Tools.h")

set(EVENT_SRC
    "Event/Event.h"
    "Event/Event.cpp"
    "Event/Input.h")

set(ROOT_SRC
    "main.cpp")

set(SR_SRC
    ${CORE_SRC}
    ${MATH_SRC}
    ${RENDERER_SRC}
    ${COMP_SRC}
    ${SCENE_SRC}
    ${CAMERA_SRC}
    ${UTILS_SRC}
    ${EVENT_SRC}
    ${ROOT_SRC})

source_group(Core FILES ${CORE_SRC})
source_group(Math FILES ${MATH_SRC})
source_group(Render FILES ${RENDERER_SRC})
source_group(Utils FILES ${UTILS_SRC})
source_group(Src FILES ${ROOT_SRC})
source_group(Components FILES ${COMP_SRC})
source_group(Scene FILES ${SCENE_SRC})
source_group(Camera FILES ${CAMERA_SRC})
source_group(Event FILES ${EVENT_SRC})

add_executable(SoftRenderer ${SR_SRC})

include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(${CMAKE_SOURCE_DIR}/assets)

file(GLOB_RECURSE ASSET_LIST_PATH ${CMAKE_SOURCE_DIR}/assets/*.*)

add_custom_target(
    copy_asset_files
    ${CMAKE_COMMAND} -E copy_if_different ${ASSET_LIST_PATH} ${CMAKE_CURRENT_BINARY_DIR}/assets)

add_dependencies(SoftRenderer copy_asset_files)