#include "RigidBody.h"
#include "btBulletDynamicsCommon.h"
#include <iostream>
RigidBody::RigidBody():
_btRigidBody(NULL)
{}

RigidBody::~RigidBody(){
	delete _btRigidBody;
}

void RigidBody::create(float mass, float posX, float posY, float posZ, 
								   float width, float height, float depth)
{
	btCollisionShape* shape = new btBoxShape(btVector3(btScalar(width), btScalar(height), btScalar(depth)));

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(posX, posY, posZ));

	btScalar btMass(mass);

	btVector3 localInertia(0, 0, 0);
	if (mass > 0)
		shape->calculateLocalInertia(btMass, localInertia);

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo(btMass, motionState, shape, localInertia);
	_btRigidBody = new btRigidBody(rigidBodyInfo);
}

btRigidBody* RigidBody::getBtRigidBody(){
	return _btRigidBody;
}

float RigidBody::getPosX() const{
	btTransform transformX;
	_btRigidBody->getMotionState()->getWorldTransform(transformX);
	//std::cout << "physic X: " << transformX.getOrigin().getX();
	return transformX.getOrigin().getX();
}

float RigidBody::getPosY() const{
	btTransform transformY;
	_btRigidBody->getMotionState()->getWorldTransform(transformY);
	//std::cout << " physic Y: " << transformY.getOrigin().getY();
	return transformY.getOrigin().getY();
}

float RigidBody::getPosZ() const{
	btTransform transformZ;
	_btRigidBody->getMotionState()->getWorldTransform(transformZ);
	//std::cout << " physic Z: " << transformZ.getOrigin().getZ() << std::endl;
	return transformZ.getOrigin().getZ();
}