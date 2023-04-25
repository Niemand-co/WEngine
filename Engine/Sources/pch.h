#pragma once
#ifndef NOT_INCLUDE

#include <iostream>
#include <stdint.h>
#include <sstream>
#include <fstream>
#include <math.h>
#include <float.h>
#include <functional>

//------------------------OS SDK-------------------------------
#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#include <io.h>
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

//------------------------Assimp-------------------------------
#include "Assimp/Include/config.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
//-------------------------------------------------------------

#include "Core/Core.h"
#include "Math/Common.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Log/Public/Logger.h"
#include "Platform/Vulkan/Allocator/Public/VulkanAllocator.h"
#include "Platform/Vulkan/Public/VulkanMacro.h"
#include "Render/Public/RenderCore.h"
#include "Render/Public/RenderResource.h"
#include "RHI/Public/RHIResource.h"
#include "RHI/Public/RHIContext.h"
#include "Render/Public/RHICommandList.h"
#include "RHI/Public/RHICore.h"
#include "Utils/Public/ShaderCodeBlob.h"
#include "Utils/Public/FileLoader.h"
#include "Event/Public/TimeStep.h"
#include "Utils/Reflection/Public/Register.h"
#include "Utils/Public/WGuid.h"
#include "Utils/Container/Public/WString.h"
#include "Utils/Container/Public/WArray.h"
#include "Utils/Container/Public/WSet.h"
#include "Utils/Container/Public/WPair.h"
#include "Utils/Container/Public/WHashMap.h"
#include "Utils/Container/Public/WSharedPtr.h"
#include "Utils/Container/Public/WDeque.h"
#include "Utils/Container/Public/WQueue.h"
#include "Utils/Container/Public/WStack.h"
#include "Utils/Container/Public/WBitArray.h"
#include "Utils/Container/Public/WStaticArray.h"
#include "Utils/Public/NamingSystem.h"
#include "HAL/Public/TaskGraph.h"
#include "HAL/Public/WRWScopeLock.h"
#include "Render/Public/RenderDependencyGraphDefinitions.h"
#include "RHI/Public/RHIStaticState.h"

//-----------------------Vulkan-------------------------------
#include "Platform/Vulkan/Public/VulkanResource.h"
//------------------------------------------------------------

#define ENABLE_REFLECTION template<typename T> friend struct WEngine::SRefl::TypeInfo;

#endif