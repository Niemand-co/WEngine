#pragma once
#include "deps/imgui/imgui.h"

class RHIInstance;
class RHIGPU;
class RHIDevice;
class RHIQueue;
class RHIRenderPass;
class RHICommandBuffer;

struct GuiConfigure
{
	RHIInstance *pInstance;
	RHIGPU *pGPU;
	RHIDevice *pDevice;
	RHIQueue *pQueue;
	unsigned int imageCount;
};

struct Menu
{

};

struct MenuBar
{

};

class Gui
{
public:

	virtual ~Gui() = default;

	virtual void Init(GuiConfigure *pConfigure) = 0;

	virtual void BindRenderPass(RHIRenderPass *pRenderPass, unsigned int subpass = 0) = 0;

	virtual void RenderGUI(RHICommandBuffer* pCommandBuffer) = 0;

public:

	static Gui* CreateGui(WEngine::Backend backend);

	static void DrawSlider(std::string title, float *pValue, float minValue, float maxValue);

	static void DrawColorEdit(std::string title, float *pColor, bool hasAlpha = false);

public:
	
	static Gui* g_pGui;

protected:

	ImGuiIO m_io;

};