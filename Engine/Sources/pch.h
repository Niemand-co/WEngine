#pragma once
#ifndef NOT_INCLUDE

#include <algorithm>
#include <vector>
#include <unordered_map>
#include <set>
#include <thread>
#include <string>
#include <iostream>
#include <stdint.h>
#include <sstream>
#include <fstream>
#include <math.h>
#include <float.h>

//------------------------OS SDK-------------------------------
#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
//-------------------------------------------------------------

#define STB_IMAGE_IMPLEMENTATION
#include "deps/stb_image.h"

//-------------------------Graphics API------------------------
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define GLFW_VULKAN_INCLUDE
#include "Vulkan/vulkan.h"
//-------------------------------------------------------------

//-------------------------ImGui-------------------------------
#include "imgui.h"
#include "imgui_internal.h"
#include "imconfig.h"
#include "imstb_rectpack.h"
#include "imstb_textedit.h"
#include "imstb_truetype.h"
#include "imgui/backends/imgui_impl_vulkan.h"
#include "imgui/backends/imgui_impl_glfw.h"
//-------------------------------------------------------------

#include "Math/Common.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Log/Public/Logger.h"
#include "Platform/Vulkan/Allocator/Public/VulkanAllocator.h"
#include "Platform/Vulkan/Public/VulkanMacro.h"
#include "Render/Public/RenderCore.h"
#include "RHI/Public/RHICore.h"
#include "Utils/Public/ShaderCodeBlob.h"

#endif