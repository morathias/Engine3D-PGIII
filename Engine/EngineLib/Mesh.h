#ifndef MESH_H
#define MESH_H
//========================================================================================
#include "Entity3D.h"
#include "RigidBody.h"
//========================================================================================
using namespace std;
//========================================================================================
class Mesh : public Entity3D
{
public:
	DllExport Mesh(Renderer& renderer);
	DllExport ~Mesh();

public:
	DllExport void setMeshData(const TexturedVertex* texVertex, 
							   Primitive ePrimitive, 
							   size_t uiVertexCount, 
							   const unsigned short* pusIndices, 
							   size_t uiIndexCount);

	DllExport void setMeshData(const Vertex* texVertex,
								Primitive ePrimitive,
								size_t uiVertexCount,
								const unsigned short* pusIndices,
								size_t uiIndexCount);

	DllExport void draw(Renderer& renderer, CollisionResult parentResult,
						const Frustum& frustum);

	DllExport void updateBV();

	DllExport void setTextureId(int iTextureId, Texture texture);

	DllExport void buildAABB();

	DllExport void buildRigidBody(float mass);
	DllExport void updatePhysics();

	DllExport void getNames(vector<std::string>& names, std::vector<int>& lvlDeep, int lvl);
	DllExport void updateNames(std::vector<std::string>& names, int& entityIndex);
	DllExport void updatePolygons(int& meshPolygons);

private:
	Renderer& _renderer;
	Primitive _primitive;
	pg2::IndexBuffer* _indexBuffer;
	pg2::VertexBuffer* _vertexBuffer;
	Texture _texture;
	const TexturedVertex* _verts;
	int _vertexCount;
	RigidBody* _rigidBody;
};
//========================================================================================
#endif