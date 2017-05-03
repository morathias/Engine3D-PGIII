#ifndef ENTITY3D_H
#define ENTITY3D_H
//========================================================================================
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0]))) //dinamic array size
//========================================================================================
#include "Camera.h"
#include <list>
//========================================================================================
class Nodo;
class BoundingBox;
class BspNode;
//========================================================================================
class Entity3D
{
public:
	DllExport Entity3D();
	DllExport ~Entity3D();

	//-------------------------------------Transformations---------------------------------
	DllExport void setPosX(float fPosX);
	DllExport void setPosY(float fPosY);
	DllExport void setPosZ(float fPosZ);

	DllExport void setRotation(float rotationX, float rotationY, float rotationZ, float rotationW);

	DllExport void setScale(float fScaleX, float fScaleY);
	DllExport void setScale(float fScaleX, float fScaleY, float scaleZ);

	DllExport float posX() const;
	DllExport float posY() const;
	DllExport float posZ() const;

	DllExport float previusPosX() const;
	DllExport float previusPosY() const;

	DllExport float rotationX() const;
	DllExport float rotationY() const;
	DllExport float rotationZ() const;
	DllExport float rotationW() const;

	DllExport float scaleX() const;
	DllExport float scaleY() const;
	DllExport float scaleZ() const;

	DllExport virtual void updateWorldTransformation();
	//-------------------------------------SceneGraph--------------------------------------
	DllExport void setName(std::string name);
	DllExport std::string getName() const;

	DllExport void setParent(Nodo* parent);
	DllExport const Matrix& worldMatrix() const;

	DllExport virtual Entity3D& findEntity(std::string name);
	//-------------------------------------BBH---------------------------------------------
	const AABB& getAABB() const;
	const Vector3* getPoints() const;
	DllExport virtual void updateBV() = 0;
	//-------------------------------------BSP---------------------------------------------
	virtual void testBsp(BspNode* node, Camera& camera) = 0;
	//-------------------------------------Physics-----------------------------------------
	DllExport virtual void updatePhysics() = 0;
	//-------------------------------------Render------------------------------------------
	DllExport virtual void draw(Renderer& renderer, CollisionResult parentResult,
		const Frustum& frustum) = 0;
	//-------------------------------------Debugger----------------------------------------
	virtual void getNames(std::vector<std::string>& names, std::vector<int>& lvlDeep, int lvl) = 0;
	virtual void updateNames(std::vector<std::string>& names, int& entityIndex) = 0;

	virtual void updatePolygons(int& meshPolygons) = 0;

protected:
	void updateLocalTransformation();

	float _posX, _posY, _posZ;
	float _previusPosX, _previusPosY;
	std::string _name;
	bool _isDrawn;

	float _rotationX, _rotationY, _rotationZ, _rotationW;

	float _scaleX, _scaleY, _scaleZ;

	AABB _aabb;
	
	BoundingBox* _boundingBox;

	Matrix _transformationMatrix;
	Matrix _worldTransformationMatrix;

	Nodo* _parent;
};
//========================================================================================
#endif

