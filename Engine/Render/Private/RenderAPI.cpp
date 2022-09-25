#include "pch.h"
#include "Render/Public/RenderAPI.h"

#ifdef defined(OPENGL)
RenderAPI::API RenderAPI::s_api = RenderAPI::API::OpenGL;
#elif defined(WIN32) || defined(_WINDOWS)
RenderAPI::API RenderAPI::s_api = RenderAPI::API::None;
#endif