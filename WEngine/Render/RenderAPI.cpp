#include "RenderAPI.h"

#ifdef OPENGL
RenderAPI::API RenderAPI::s_api = RenderAPI::API::None;
#endif