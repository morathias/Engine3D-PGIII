#include "BspNode.h"
#include <cmath>
#include <iostream>

#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib") 
//==================================================================
BspPlane::BspPlane(){}
//==================================================================
BspPlane::~BspPlane(){
	delete plane;
}
//==================================================================
void BspPlane::create(float pointA[3], float pointB[3], float pointC[3], Mesh& mesh){
	D3DXVECTOR3 vecA, vecB, vecC;
	vecA.x = pointA[0];	vecA.y = pointA[1]; vecA.z = pointA[2];
	vecB.x = pointB[0];	vecB.y = pointB[1]; vecB.z = pointB[2];
	vecC.x = pointC[0];	vecC.y = pointC[1]; vecC.z = pointC[2];

	plane = new D3DXPLANE();

	plane = D3DXPlaneFromPoints(plane, &vecA, &vecB, &vecC);

	position = new D3DXVECTOR3();
	position->x = mesh.posX();
	position->y = mesh.posY();
	position->z = mesh.posZ();
	D3DXPlaneNormalize(plane, plane);
}
//==================================================================
BspNode::BspNode():
_bspPlane(new BspPlane())
{}
//==================================================================
BspNode::~BspNode(){
	delete _bspPlane;
}
//==================================================================
void BspNode::buildPlane(float pointA[3], float pointB[3], float pointC[3], Mesh& planeMesh){
	_bspPlane->create(pointA, pointB, pointC, planeMesh);
}
//==================================================================
void BspNode::addFrontNode(BspNode& frontNode){
	_frontNodes.push_back(&frontNode);
}
//==================================================================
void BspNode::addBackNode(BspNode& backNode){
	_backNodes.push_back(&backNode);
}
//==================================================================
void BspNode::checkBspNodes(vector<BspNode*>& nodes){
	//if is not a leaf node
	if (nodes.size() < 2)
		return;

	//get normal direction of current plane
	D3DXVECTOR3 thisNormal, nonAbThisNormal;
	thisNormal.x = abs(_bspPlane->plane->a);
	thisNormal.y = abs(_bspPlane->plane->b);
	thisNormal.z = abs(_bspPlane->plane->c);
	D3DXVec3Normalize(&thisNormal, &thisNormal);

	nonAbThisNormal.x = _bspPlane->plane->a;
	nonAbThisNormal.y = _bspPlane->plane->b;
	nonAbThisNormal.z = _bspPlane->plane->c;
	D3DXVec3Normalize(&nonAbThisNormal, &nonAbThisNormal);

	//start checking with other planes
	for (size_t i = 1; i < nodes.size(); i++){

		//get other plane normal direction
		D3DXVECTOR3 normal, nonAbNormal;

		normal.x = abs(nodes[i]->getBspPlane().plane->a);
		normal.y = abs(nodes[i]->getBspPlane().plane->b);
		normal.z = abs(nodes[i]->getBspPlane().plane->c);
		D3DXVec3Normalize(&normal, &normal);

		//if they are not parallel, then they are intersecting so add node to front and back
		if (normal != thisNormal){
			BspNode* newNode = new BspNode();

			*newNode = *nodes[i];
			newNode->setName(nodes[i]->getName() + std::to_string(1));
			addBackNode(*newNode);
			addFrontNode(*nodes[i]);
		}

		else{
			D3DXVECTOR3 direction;
			direction = *_bspPlane->position - *nodes[i]->getBspPlane().position;

			float dist = D3DXVec3Dot(&direction, &nonAbThisNormal);

			if (dist > 0.001)
				addFrontNode(*nodes[i]);
			else if (dist < -0.001)
				addBackNode(*nodes[i]);
		}
	}
	//all front and back nodes of the current node were checked
	cout << "name: " << _name << " front nodes: " << _frontNodes.size() << " backNodes: " << _backNodes.size() << endl;
	//start recursivity for the front nodes of the current node
	if (!_frontNodes.empty())
		_frontNodes[0]->checkBspNodes(_frontNodes);

	//start recursivity for the back nodes of the current node
	if (!_backNodes.empty())
		_backNodes[0]->checkBspNodes(_backNodes);
}
//==================================================================
BspPlane& BspNode::getBspPlane(){
	return *_bspPlane;
}
//==================================================================
void BspNode::setName(string name){
	_name = name;
}
//==================================================================
string BspNode::getName(){
	return _name;
}
//==================================================================
vector<BspNode*>& BspNode::getFrontNodes(){
	return _frontNodes;
}
//==================================================================
vector<BspNode*>& BspNode::getBackNodes(){
	return _backNodes;
}
//==================================================================
void BspNode::checkEntity(Mesh& mesh, Camera& camera){
	D3DXVECTOR3 entityPositionMin, directionMin, entityPositionMax, directionMax;
	entityPositionMin.x = mesh.getAABB().min[0];
	entityPositionMin.y = mesh.getAABB().min[2];
	entityPositionMin.z = mesh.getAABB().min[1];

	directionMin = *_bspPlane->position - entityPositionMin;

	entityPositionMax.x = mesh.getAABB().max[0];
	entityPositionMax.y = mesh.getAABB().max[2];
	entityPositionMax.z = mesh.getAABB().max[1];

	directionMax = *_bspPlane->position - entityPositionMax;

	D3DXVECTOR3 cameraPosition, cameraDirection;
	cameraPosition.x = camera.posX();
	cameraPosition.y = camera.posZ();
	cameraPosition.z = camera.posY();

	cameraDirection = *_bspPlane->position - cameraPosition;

	D3DXVECTOR3 thisNormal;
	thisNormal.x = _bspPlane->plane->a;
	thisNormal.y = _bspPlane->plane->b;
	thisNormal.z = _bspPlane->plane->c;
	D3DXVec3Normalize(&thisNormal, &thisNormal);

	float distMin = D3DXVec3Dot(&directionMin, &thisNormal);
	float distMax = D3DXVec3Dot(&directionMax, &thisNormal);
	float cameraDist = D3DXVec3Dot(&cameraDirection, &thisNormal);

	if (_frontNodes.empty() && _backNodes.empty()){
		if (distMin > 0 && distMax > 0){
			if (cameraDist < 0){
				mesh.bspKill();
				return;
			}

			return;
		}

		else if (distMin < 0 && distMax < 0){
			if (cameraDist > 0){
				mesh.bspKill();
				return;
			}

			return;
		}
	}

	if (distMin > 0 && distMax > 0){

		if (cameraDist > 0){
			if (_frontNodes.empty()){
				return;
			}
			else
				_frontNodes[0]->checkEntity(mesh, camera);
		}

		else{
			mesh.bspKill();
			return;
		}
	}

	else if (distMin < 0 && distMax < 0){

		if (cameraDist < 0){
			if (_backNodes.empty()){
				return;
			}
			else
				_backNodes[0]->checkEntity(mesh, camera);
		}

		else{
			mesh.bspKill();
			return;
		}
	}
}
//==================================================================