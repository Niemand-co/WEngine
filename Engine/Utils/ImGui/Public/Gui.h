#pragma once

class RHIInstance;
class RHIGPU;
class RHIDevice;

struct GuiConfigure
{
	RHIInstance *pInstance;
	RHIGPU *pGPU;
	RHIDevice *pDevice;
};

class Gui
{
public:

	Gui();

	virtual ~Gui() = default;

	virtual void Init(GuiConfigure *pConfigure) = 0;

private:

	ImGuiIO m_io;

};