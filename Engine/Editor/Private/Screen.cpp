#include "pch.h"
#include "Editor/Public/Screen.h"
#include "Scene/Components/Public/Camera.h"

namespace WEngine
{

	unsigned int Screen::g_width = 960;

	unsigned int Screen::g_height = 720;

	bool Screen::g_isSizeChanged = false;

	Camera* Screen::g_displayingCamera = nullptr;

	Screen::Screen()
	{
	}

	void Screen::SetWidth(unsigned int width)
	{
		g_isSizeChanged = (width != g_width);
		g_width = width;
	}

	void Screen::SetHeight(unsigned int height)
	{
		g_isSizeChanged = (height != g_height);
		g_height = height;
	}

	unsigned int Screen::GetWidth()
	{
		return g_width;
	}

	unsigned int Screen::GetHeight()
	{
		return g_height;
	}

	bool Screen::SizeChanged()
	{
		return g_isSizeChanged;
	}

	void Screen::ResetState()
	{
		g_isSizeChanged = false;
	}

	void Screen::SetDisplayCamera(Camera* pCamera)
	{
		g_displayingCamera = pCamera;
	}



}