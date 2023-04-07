#pragma once

class GameObject;

class GWorld
{
public:

	GWorld() = default;

	~GWorld() = default;

	GameObject* CreateGameObject(const WEngine::WString& name = "GameObject");

	static GWorld* GetWorld();

	const WEngine::WArray<GameObject*>& GetGameObjects() const;

	const WEngine::WArray<WEngine::WString>& GetObjectNames() const;

public:

	class RScene *Scene;

private:

	WEngine::WArray<GameObject*> m_pEntities;

	WEngine::WArray<WEngine::WString> m_pNames;

};