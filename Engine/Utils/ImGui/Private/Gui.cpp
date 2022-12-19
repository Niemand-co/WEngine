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

void Gui::DrawSlider(WEngine::WString title, float* pValue, float minValue, float maxValue)
{
	ImGui::Text(title.Data());ImGui::SameLine();
	title = "##" + title;
	ImGui::SliderFloat(title.Data(), pValue, minValue, maxValue);
}

void Gui::DrawColorEdit(WEngine::WString title, float* pColor, bool hasAlpha)
{
	ImGui::Text(title.Data()); ImGui::SameLine();
	title = "##" + title;
	if (hasAlpha)
	{
		ImGui::ColorEdit4(title.Data(), pColor);
	}
	else
	{
		ImGui::ColorEdit3(title.Data(), pColor);
	}
}
