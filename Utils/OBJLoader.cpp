#include "OBJLoader.h"
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "Math/Vector.h"
#include "Render/Vertex.h"

Mesh OBJLoader::Load(char* filePath)
{
	std::ifstream obj(filePath);
	std::string line;
	std::vector<Vertex> vertices;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvs;
	std::vector<uint32_t> indices;
	while (std::getline(obj, line))
	{
		if (line.substr(0, 2) == "vt")
		{
			std::istringstream in(line.substr(3));
			float u, v;
			in>>u;
			in>>v;
			uvs.emplace_back(u, v);
		}
		else if (line.substr(0, 2) == "vn")
		{
			std::istringstream in(line.substr(3));
			float x, y, z;
			in>>x;
			in>>y;
			in>>z;
			normals.emplace_back(x, y, z);
		}
		else if (line.substr(0, 1) == "v")
		{
			std::istringstream in(line.substr(2));
			float x, y, z;
			in>>x;
			in>>y;
			in>>z;
			vertices.emplace_back(Vec3(x, y, z));
		}
		else if (line.substr(0, 1) == "f")
		{
			std::istringstream in(line.substr(2));
			for (int i = 0; i < 3; ++i)
			{
				std::string str;
				std::vector<int> index;
				index.reserve(3);
				in>>str;
				for (int l = 0, r = 1; r < str.size(); ++r)
				{
					if (str[r] == '/')
					{
						index.push_back(atoi(str.substr(l, r).c_str()));
						l = r + 1;
					}
					if (r == str.size() - 1)
					{
						index.push_back(atoi(str.substr(l, int(long long(r) + 1)).c_str()));
						break;
					}
				}
				indices.push_back(index[0] - 1);
				vertices[int(long long(index[0]) - 1)].uv = uvs[int(long long(index[1]) - 1)];
				vertices[int(long long(index[0]) - 1)].normal = normals[int(long long(index[2]) - 1)];
			}
		}
		else if (line.substr(0, 1) == "#")
		{

		}
	}
	Mesh mesh;
	mesh.AddVertices(&vertices[0], vertices.size());
	mesh.AddIndices(&indices[0], indices.size());
	return mesh;
}