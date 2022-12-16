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
#include <functional>
#include <tuple>

//------------------------OS SDK-------------------------------
#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
//-------------------------------------------------------------

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
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imconfig.h"
#include "imgui/imstb_rectpack.h"
#include "imgui/imstb_textedit.h"
#include "imgui/imstb_truetype.h"
#include "imgui/backends/imgui_impl_vulkan.h"
#include "imgui/backends/imgui_impl_glfw.h"
//-------------------------------------------------------------

#include "Core/Core.h"
#include "Math/Common.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Log/Public/Logger.h"
#include "Platform/Vulkan/Allocator/Public/VulkanAllocator.h"
#include "Utils/Public/Synchronizer.h"
#include "Platform/Vulkan/Public/VulkanMacro.h"
#include "Render/Public/RenderCore.h"
#include "Render/Public/RenderResource.h"
#include "RHI/Public/RHICore.h"
#include "Utils/Public/ShaderCodeBlob.h"
#include "Utils/Public/FileLoader.h"
#include "Event/Public/TimeStep.h"
#include "Utils/Reflection/Public/Register.h"
#include "Utils/Container/Public/WString.h"
#include "Utils/Public/WGuid.h"
#include "Utils/Container/Public/WArray.h"
#include "Utils/Container/Public/WPair.h"
#include "Utils/Public/NamingSystem.h"

#define ENABLE_REFLECTION template<typename T> friend struct WEngine::SRefl::TypeInfo;

#endif