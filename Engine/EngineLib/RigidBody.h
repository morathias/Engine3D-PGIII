#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "DLLExport.h"

class btRigidBody;

class RigidBody{
public:
	DllExport RigidBody();
	DllExport ~RigidBody();

	DllExport void create(float mass, float posX, float posY, float posZ, 
									  float width, float height, float depth);

	DllExport float getPosX() const;
	DllExport float getPosY() const;
	DllExport float getPosZ() const;

public:
	btRigidBody* getBtRigidBody();

private:
	btRigidBody* _btRigidBody;

	float _mass;
	float _posX, posY, posZ;
};

#endif

