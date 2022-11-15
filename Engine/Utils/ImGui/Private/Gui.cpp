#include "pch.h"
#include "Utils/ImGui/Public/Gui.h"
#include "Platform/Vulkan/ImGui/Public/VulkanGui.h"

Gui* Gui::g_pGui = nullptr;

Gui* Gui::CreateGui(WEngine::Backend backend)
{
	switch (backend)
	{
	case WEngine::Backend::None:
		break;
	case WEngine::Backend::Vulkan:
	{
		g_pGui = (Gui*)WEngine::Allocator::Get()->Allocate(sizeof(Vulkan::VulkanGui));
		::new (g_pGui) Vulkan::VulkanGui();
		return g_pGui;
	}
	default:
		RE_ASSERT(false, "Error Backend for Gui.");
	}
	return nullptr;
}

void Gui::DrawSlider(std::string title, float* pValue, float minValue, float maxValue)
{
	ImGui::Text(title.c_str());ImGui::SameLine();
	title = "##" + title;
	ImGui::SliderFloat(title.data(), pValue, minValue, maxValue);
}

void Gui::DrawColorEdit(std::string title, float* pColor, bool hasAlpha)
{
	ImGui::Text(title.c_str()); ImGui::SameLine();
	title = "##" + title;
	if (hasAlpha)
	{
		ImGui::ColorEdit4(title.data(), pColor);
	}
	else
	{
		ImGui::ColorEdit3(title.data(), pColor);
	}
}
