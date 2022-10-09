#include "pch.h"
#include "Utils/ImGui/Public/Gui.h"

Gui::Gui()
{
	ImGui::CreateContext();
	m_io = ImGui::GetIO();
}
