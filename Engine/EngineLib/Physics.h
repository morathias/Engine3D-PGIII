#ifndef PHYSICS_H
#define PHYSICS_H
//===================================================================
#include "DLLExport.h"
#include "RigidBody.h"
#include <vector>
//===================================================================
using namespace std;
//===================================================================
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
//===================================================================
class Physics{
public:
	Physics();
	~Physics();

	void init();
	DllExport static void addRigidBody(RigidBody& rigidBody);
	DllExport static void removeRigidBody(RigidBody& rigidBody);
	void addBtRigidBodies();
	void removeBtRigidBody();
	void update();

private:
	btDefaultCollisionConfiguration* _collisionConfig;
	btCollisionDispatcher* _collisionDispatcher;
	btBroadphaseInterface* _broadPhaseInterface;
	btSequentialImpulseConstraintSolver* _impulseSolver;
	btDiscreteDynamicsWorld* _dynamicWorld;

	static vector<RigidBody*> _rigidBodies;
	static RigidBody* _rigidToRemove;
};
//===================================================================
#endif

