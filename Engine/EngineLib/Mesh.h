#ifndef MESH_H
#define MESH_H
//========================================================================================
#include "Entity3D.h"
#include "RigidBody.h"
#include "Material.h"
//========================================================================================
using namespace std;
//========================================================================================
class Mesh : public Entity3D{
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

	DllExport void setMeshData(const ShadedVertex* texVertex,
		Primitive ePrimitive,
		size_t uiVertexCount,
		const unsigned short* pusIndices,
		size_t uiIndexCount);

	DllExport void draw(Renderer& renderer, CollisionResult parentResult,
						const Frustum& frustum);

	DllExport void updateBV();

	DllExport void setTextureId(int iTextureId, Texture texture);
	DllExport void setMaterial(Material& material);
	DllExport Material& getMaterial() const;

	DllExport void buildAABB();

	void testBsp(BspNode* node, Camera& camera);

	DllExport void buildRigidBody(float mass);
	DllExport void updatePhysics();

	void getNames(vector<std::string>& names, std::vector<int>& lvlDeep, int lvl);
	void updateNames(std::vector<std::string>& names, int& entityIndex);
	void updatePolygons(int& meshPolygons);

	void bspKill();

private:
	Renderer& _renderer;

	Primitive _primitive;

	pg2::IndexBuffer* _indexBuffer;
	pg2::VertexBuffer* _vertexBuffer;

	Texture _texture;
	Material* _material;
	const ShadedVertex* _verts;
	int _vertexCount;

	RigidBody* _rigidBody;

	bool _bspSurvivor = true;
};
//========================================================================================
#endif