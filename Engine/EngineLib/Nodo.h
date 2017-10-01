#ifndef NODO_H
#define NODO_H
//=============================================================================
#include "Entity3D.h"
//=============================================================================
using namespace std;
//=============================================================================
class Nodo : public Entity3D
{
public:
	DllExport Nodo();
	DllExport Nodo(const Nodo& origin);
	DllExport ~Nodo();

	DllExport const vector<Entity3D*> childs() const;
	DllExport bool addChild(Entity3D& child);
	DllExport bool removeChild(Entity3D& child);

	DllExport void updateWorldTransformation();
	
	DllExport void draw(Renderer& renderer, CollisionResult parentResult,
						const Frustum& frustum);

	DllExport void updateBV();

	DllExport void updatePhysics();

	void testBsp(BspNode* node, Camera& camera);

	void getNames(vector<std::string>& names, std::vector<int>& lvlDeep, int lvl);
	void updateNames(std::vector<std::string>& names, int& entityIndex);

	void updatePolygons(int& meshPolygons);

	DllExport Entity3D& findEntity(string name);

private:
	vector<Entity3D*> _childs;
};
//=============================================================================
#endif
//=============================================================================