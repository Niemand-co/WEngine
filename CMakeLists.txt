cmake_minimum_required(VERSION 3.8)

project(SoftRenderer)

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT WEngine)

add_definitions(-DOPENGL)

set(GLAD_INCLUDE ${CMAKE_SOURCE_DIR}/deps/glad/include)

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
    "Render/RenderAPI.h"
    "Render/RenderAPI.cpp"
    "Render/Framebuffer.h"
    "Render/Framebuffer.cpp"
    "Render/Vertex.h"
    "Render/RenderContext.h"
    "Render/RenderContext.cpp")

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

set(OGL_SRC
    "Platform/OpenGL/glad.c"
    "Platform/OpenGL/OGLWindow.h"
    "Platform/OpenGL/OGLWindow.cpp"
    "Platform/OpenGL/OGLRenderer.h"
    "Platform/OpenGL/OGLRenderer.cpp"
    "Platform/OpenGL/OGLRenderContext.h"
    "Platform/OpenGL/OGLRenderContext.cpp")

set(WIN_SRC
    "Platform/Windows/WindowsWindow.h"
    "Platform/Windows/WindowsWindow.cpp"
    "Platform/Windows/WindowsRenderer.h"
    "Platform/Windows/WindowsRenderer.cpp")

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



source_group(Core FILES ${CORE_SRC})
source_group(Math FILES ${MATH_SRC})
source_group(Render FILES ${RENDERER_SRC})
source_group(Utils FILES ${UTILS_SRC})
source_group(Src FILES ${ROOT_SRC})
source_group(Components FILES ${COMP_SRC})
source_group(Scene FILES ${SCENE_SRC})
source_group(Camera FILES ${CAMERA_SRC})
source_group(Event FILES ${EVENT_SRC})
source_group(Platform/OpenGL FILES ${OGL_SRC})
source_group(Platform/Windows FILES ${WIN_SRC})

add_executable(WEngine ${SR_SRC})

set(GLFW_DIR ${CMAKE_SOURCE_DIR}/glfw)

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