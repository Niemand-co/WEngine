#pragma once

class Vertex;
class RHIBuffer;

namespace WEngine
{

	class Debug
	{
	public:

		static void Init();

		static void DrawLine(glm::vec3 a, glm::vec3 b, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		static void Update();
		
		static unsigned int GetIndexCount() { return g_lineIndices.Size(); }

		static RHIBuffer* GetLineVertexBuffer() { return g_pLineVertexBuffer; }

		static RHIBuffer* GetLineIndexBuffer() { return g_pLineIndexBuffer; }

	private:

		static WArray<Vertex> g_lineVertices;

		static WArray<unsigned int> g_lineIndices;

		static RHIBuffer* g_pLineVertexBuffer;

		static RHIBuffer *g_pLineIndexBuffer;

	};

}