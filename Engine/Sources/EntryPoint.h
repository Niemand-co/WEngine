extern WEngine::Application* WEngine::CreateApplication();

size_t Hashs(WEngine::WGuid<WEngine::WString> key)
{
	return (size_t(key.A) << 32) | (size_t(key.B));
}

int main(int argc, char** argv)
{
	//WEngine::Application* g_pApp = WEngine::CreateApplication();
	//g_pApp->Tick();

	WEngine::WHashMap<WEngine::WGuid<WEngine::WString>, int, Hashs> Shaders;
	WEngine::WString name = "One";
	Shaders.Insert(WEngine::WGuid(name), 1);
	RE_LOG(Shaders[name]);

	return 0;
}