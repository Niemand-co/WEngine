#pragma once
#include "deps/imgui/imgui.h"

class RHIInstance;
class RHIGPU;
class RHIDevice;
class RHIQueue;
class RHIRenderPass;
class RHICommandBuffer;
class RHITextureView;
class RHISampler;
class GameObject;

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

	inline ImGuiIO& GetIO() { return m_io; }

	virtual void BindRenderPass(RHIRenderPass *pRenderPass, unsigned int subpass = 0) = 0;

	virtual ImTextureID LoadTexture(RHITextureView *pTextureView, RHISampler* pSampler) = 0;

	virtual void RemoveTexture(ImTextureID& ID) = 0;

	virtual void RenderGUI(RHICommandBuffer* pCommandBuffer) = 0;

	virtual void ShowInspector() = 0;

	virtual void CollectComponents() = 0;

	virtual void CollectVariables() = 0;

public:

	static Gui* CreateGui(WEngine::Backend backend);

	static void DrawSlider(std::string_view title, float *pValue, float minValue, float maxValue);

	static void DrawColorEdit(std::string_view title, float *pColor, bool hasAlpha = false);

	template<typename T, typename ...Args>
	static void ShowElement(std::string_view title, T &value, Args ...args);

public:
	
	static Gui* g_pGui;

protected:

	ImGuiIO m_io;

};

template<typename T, typename ...Args>
inline void Gui::ShowElement(std::string_view title, T &value, Args ...args)
{
	if (WEngine::is_same<T, float>::isSame)
	{
		DrawSlider(title, (void*)&value, 0.01f, 1.0f);
	}
	else if (WEngine::is_same<T, glm::vec3>::isSame)
	{
		//DrawColorEdit(title, &value[0], false);
	}
	else if (WEngine::is_same<T, glm::vec4>::isSame)
	{
		//DrawColorEdit(title, &value[0], true);
	}
	//else
	//{
	//	RE_ASSERT(false, "No Match Type Element to be Shown in Inspector.");
	//}
}
