#pragma once

struct PrimitiveInfo;

class RScene
{
public:

	RScene();

	~RScene();

	void AddPrimitive(class PrimitiveComponent *primitive);

	void RemovePrimitive(PrimitiveComponent *primitive);

	void UpdatePrimitiveInfosForScene();

public:

	static RScene* GetActiveScene() { return g_activeScene; }

private:

	static RScene *g_activeScene;

private:

	class GWorld* m_pWorld;

	WEngine::WArray<PrimitiveInfo*> m_primitives;

	WEngine::WSet<PrimitiveInfo*> m_addedPrimitives;

	WEngine::WSet<PrimitiveInfo*> m_removedPrimitives;

};