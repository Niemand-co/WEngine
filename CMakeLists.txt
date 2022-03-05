cmake_minimum_required(VERSION 3.8)

project(SoftRenderer)

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT WEngine)

add_definitions(-DOPENGL)

set(GLAD_INCLUDE ${CMAKE_SOURCE_DIR}/WEngine/deps/glad/include)

set(CORE_SRC
    "WEngine/Core/Application.h"
    "WEngine/Core/Application.cpp")

set(MATH_SRC
    "WEngine/Math/Matrix.h"
    "WEngine/Math/Vector.h")

set(RENDERER_SRC
    "WEngine/Render/Rasterizer.h"
    "WEngine/Render/Rasterizer.cpp"
    "WEngine/Render/Renderer.h"
    "WEngine/Render/Renderer.cpp"
    "WEngine/Render/RenderAPI.h"
    "WEngine/Render/RenderAPI.cpp"
    "WEngine/Render/Framebuffer.h"
    "WEngine/Render/Framebuffer.cpp"
    "WEngine/Render/Vertex.h"
    "WEngine/Render/RenderContext.h"
    "WEngine/Render/RenderContext.cpp"
    "WEngine/Render/RenderCommand.h"
    "WEngine/Render/RenderCommand.cpp")

set(COMP_SRC
    "WEngine/Components/Component.h"
    "WEngine/Components/Material.h"
    "WEngine/Components/Material.cpp"
    "WEngine/Components/Mesh.h"
    "WEngine/Components/Mesh.cpp"
    "WEngine/Components/Transformer.h"
    "WEngine/Components/Transformer.cpp")

set(SCENE_SRC
    "WEngine/Scene/World.h"
    "WEngine/Scene/World.cpp"
    "WEngine/Scene/Entity.h"
    "WEngine/Scene/Entity.cpp"
    "WEngine/Scene/Light.h"
    "WEngine/Scene/Light.cpp")

set(CAMERA_SRC
    "WEngine/Camera/Camera.h"
    "WEngine/Camera/Camera.cpp")

set(UTILS_SRC
    "WEngine/Utils/Shader.h"
    "WEngine/Utils/Shader.cpp"
    "WEngine/Utils/Window.h"
    "WEngine/Utils/Window.cpp"
    "WEngine/Utils/OBJLoader.h"
    "WEngine/Utils/OBJLoader.cpp"
    "WEngine/Utils/Tools.h")

set(EVENT_SRC
    "WEngine/Event/Event.h"
    "WEngine/Event/Event.cpp"
    "WEngine/Event/Input.h")

set(ROOT_SRC
    "main.cpp")

set(OGL_SRC
    "Platform/OpenGL/glad.c"
    "Platform/OpenGL/OGLWindow.h"
    "Platform/OpenGL/OGLWindow.cpp"
    "Platform/OpenGL/OGLRenderer.h"
    "Platform/OpenGL/OGLRenderer.cpp"
    "Platform/OpenGL/OGLRenderContext.h"
    "Platform/OpenGL/OGLRenderContext.cpp"
    "Platform/OpenGL/OGLShader.h"
    "Platform/OpenGL/OGLShader.cpp"
    "Platform/OpenGL/OGLRenderAPI.h"
    "Platform/OpenGL/OGLRenderAPI.cpp")

set(WIN_SRC
    "Platform/Windows/WindowsWindow.h"
    "Platform/Windows/WindowsWindow.cpp"
    "Platform/Windows/WindowsRenderer.h"
    "Platform/Windows/WindowsRenderer.cpp"
    "Platform/Windows/WindowsRenderContext.h"
    "Platform/Windows/WindowsRenderContext.cpp"
    "Platform/Windows/WindowsShader.h"
    "Platform/Windows/WindowsShader.cpp"
    "Platform/Windows/WindowsRenderAPI.h"
    "Platform/Windows/WindowsRenderAPI.cpp")

set(PLATFORM_SRC
    ${OGL_SRC}
    ${WIN_SRC})

set(SR_SRC
    ${CORE_SRC}
    ${MATH_SRC}
    ${RENDERER_SRC}
    ${COMP_SRC}
    ${SCENE_SRC}
    ${CAMERA_SRC}
    ${UTILS_SRC}
    ${EVENT_SRC}
    ${ROOT_SRC}
    ${PLATFORM_SRC})


list(APPEND WENGINE_INCLUDE ${GLAD_INCLUDE})
list(APPEND WENGINE_INCLUDE ${DEPS_INCLUDE})



source_group(WEngine/Core FILES ${CORE_SRC})
source_group(WEngine/Math FILES ${MATH_SRC})
source_group(WEngine/Render FILES ${RENDERER_SRC})
source_group(WEngine/Utils FILES ${UTILS_SRC})
source_group(WEngine/Src FILES ${ROOT_SRC})
source_group(WEngine/Components FILES ${COMP_SRC})
source_group(WEngine/Scene FILES ${SCENE_SRC})
source_group(WEngine/Camera FILES ${CAMERA_SRC})
source_group(WEngine/Event FILES ${EVENT_SRC})
source_group(Platform/OpenGL FILES ${OGL_SRC})
source_group(Platform/Windows FILES ${WIN_SRC})

add_executable(WEngine ${SR_SRC})

set(GLFW_DIR ${CMAKE_SOURCE_DIR}/WEngine/deps/glfw)

include_directories(${CMAKE_SOURCE_DIR}/Platform)
include_directories(${CMAKE_SOURCE_DIR}/WEngine)
include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(${CMAKE_SOURCE_DIR}/assets)
add_subdirectory(${GLFW_DIR})

list(APPEND WENGINE_INCLUDE ${GLFW3_DIR}/include)

target_include_directories(WEngine PUBLIC ${WENGINE_INCLUDE})
target_link_libraries(WEngine glfw)

file(GLOB_RECURSE ASSET_LIST_PATH ${CMAKE_SOURCE_DIR}/assets/*.*)

add_custom_target(
    copy_asset_files
    ${CMAKE_COMMAND} -E copy_if_different ${ASSET_LIST_PATH} ${CMAKE_CURRENT_BINARY_DIR}/assets)

add_dependencies(WEngine copy_asset_files)