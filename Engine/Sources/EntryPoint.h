#include "Utils/Reflection/Public/Register.h"
extern WEngine::Application* WEngine::CreateApplication();

enum class Color
{
	RED = 0,
	GREEN,
	BLUE,
};

[[Custom]]
int main(int argc, char** argv)
{
	//WEngine::Application* g_pApp = WEngine::CreateApplication();
	//g_pApp->Tick();
	std::cout<<WEngine::GetEnumName(Color::RED)<<std::endl;
	std::cout<<(int)WEngine::GetEnumFromName<Color>("RED")<<std::endl;
	
	std::string name = "Test";
	WEngine::SRefl::BaseValue<float> a(name, 1.0f);
	WEngine::SRefl::BaseValue<float> b(name, 1);
	if (a == b)
	{
		RE_LOG("Same");
	}
	return 0;
}