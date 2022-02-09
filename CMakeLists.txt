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
    "Renderer/Rasterizer.h"
    "Renderer/Rasterizer.cpp"
    "Renderer/Vertex.h")

set(UTILS_SRC
    "Utils/Shader.h"
    "Utils/Shader.cpp"
    "Utils/Window.h"
    "Utils/Window.cpp")

set(ROOT_SRC
    "main.cpp")

set(SR_SRC
    ${CORE_SRC}
    ${MATH_SRC}
    ${RENDERER_SRC}
    ${UTILS_SRC}
    ${ROOT_SRC})

source_group(Core FILES ${CORE_SRC})
source_group(Math FILES ${MATH_SRC})
source_group(Renderer FILES ${RENDERER_SRC})
source_group(Utils FILES ${UTILS_SRC})
source_group(Src FILES ${ROOT_SRC})

add_executable(SoftRenderer ${SR_SRC})

include_directories(${CMAKE_SOURCE_DIR})