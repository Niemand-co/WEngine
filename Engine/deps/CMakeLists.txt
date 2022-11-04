file(GLOB IMGUI_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB IMGUI_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set(IMGUI_VULKAN_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_glfw.h ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_vulkan.h)
set(IMGUI_VULKAN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_glfw.cpp ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp)

set(GUI_SRC
    ${IMGUI_SRC}
    ${IMGUI_VULKAN_SRC})

set(GUI_INCLUDE
    ${IMGUI_INCLUDE}
    ${IMGUI_VULKAN_INCLUDE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/Engine/deps/Vulkan/Include)
include_directories(${PROJECT_SOURCE_DIR}/Engine/deps/glfw/include)

add_library(ImGui STATIC ${GUI_SRC})
