#pragma once
#include "Utils/ImGui/Public/Gui.h"

namespace Vulkan
{

	class VulkanGui : public Gui
	{
	public:

		VulkanGui();

		virtual ~VulkanGui();

		virtual void Init(GuiConfigure* pConfigure) override;

	private:

	

	};

}