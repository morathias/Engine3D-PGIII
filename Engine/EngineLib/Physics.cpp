#include "Physics.h"
#include "btBulletDynamicsCommon.h"

Physics::Physics(){}

Physics::~Physics(){
	delete _dynamicWorld;
	_dynamicWorld = NULL;

	delete _impulseSolver;
	_impulseSolver = NULL;

	delete _broadPhaseInterface;
	_broadPhaseInterface = NULL;

	delete _collisionDispatcher;
	_collisionConfig = NULL;

	delete _collisionConfig;
	_collisionConfig = NULL;
}

void Physics::init(){
	_collisionConfig = new btDefaultCollisionConfiguration();

	_collisionDispatcher = new btCollisionDispatcher(_collisionConfig);

	_broadPhaseInterface = new btDbvtBroadphase();

	_impulseSolver = new btSequentialImpulseConstraintSolver;

	_dynamicWorld = new btDiscreteDynamicsWorld(_collisionDispatcher, _broadPhaseInterface, _impulseSolver, _collisionConfig);

	_dynamicWorld->setGravity(btVector3(0, -9.80665f, 0));
}

vector<RigidBody*> Physics::_rigidBodies;
void Physics::addRigidBody(RigidBody& rigidBody){
	_rigidBodies.push_back(&rigidBody);
}

void Physics::addBtRigidBodies(){
	for (size_t i = 0; i < _rigidBodies.size(); i++){
		_dynamicWorld->addRigidBody(_rigidBodies[i]->getBtRigidBody());
	}
}

void Physics::update(){
	_dynamicWorld->stepSimulation(1.f / 60.f, 10);
}
RigidBody* Physics::_rigidToRemove;
void Physics::removeRigidBody(RigidBody& rigidBody){
	_rigidToRemove = &rigidBody;
}

void Physics::removeBtRigidBody(){
	if (_rigidToRemove){
		_dynamicWorld->removeCollisionObject(_rigidToRemove->getBtRigidBody());

		for (size_t i = 0; i < _rigidBodies.size(); i++){
			if (_rigidBodies[i] == _rigidToRemove){
				_rigidBodies.erase(_rigidBodies.begin() + i);
			}
		}
	}
}