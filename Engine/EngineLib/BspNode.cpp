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
void BspPlane::create(float pointA[3], float pointB[3], float pointC[3]){
	D3DXVECTOR3 vecA, vecB, vecC;
	vecA.x = pointA[0];	vecA.y = pointA[1]; vecA.z = pointA[2];
	vecB.x = pointB[0];	vecB.y = pointB[1]; vecB.z = pointB[2];
	vecC.x = pointC[0];	vecC.y = pointC[1]; vecC.z = pointC[2];

	plane = new D3DXPLANE();

	plane = D3DXPlaneFromPoints(plane, &vecA, &vecB, &vecC);
	//D3DXPlaneNormalize(plane, plane);
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
void BspNode::buildPlane(float pointA[3], float pointB[3], float pointC[3]){
	_bspPlane->create(pointA, pointB, pointC);
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
BspResult BspNode::checkEntity(Entity3D& entity){
	return Back;
}
//==================================================================
void BspNode::checkBspNodes(vector<BspNode*>& nodes){
	//if is not a leaf node
	if (nodes.size() < 2)
		return;

	//get normal direction of current plane
	D3DXVECTOR3 thisNormal;
	thisNormal.x = abs(_bspPlane->plane->a);
	thisNormal.y = abs(_bspPlane->plane->b);
	thisNormal.z = abs(_bspPlane->plane->c);
	D3DXVec3Normalize(&thisNormal, &thisNormal);

	//start checking with other planes
	for (size_t i = 1; i < nodes.size(); i++){

		//get other plane normal direction
		D3DXVECTOR3 normal;
		normal.x = abs(nodes[i]->getBspPlane().plane->a);
		normal.y = abs(nodes[i]->getBspPlane().plane->b);
		normal.z = abs(nodes[i]->getBspPlane().plane->c);
		D3DXVec3Normalize(&normal, &normal);

		//if they are not parallel, then they are intersecting so add node to front and back
		if (normal != thisNormal){
			addBackNode(*nodes[i]);
			addFrontNode(*nodes[i]);
		}

		else{
			float dist = D3DXPlaneDotNormal(_bspPlane->plane, &normal);

			if (dist > 0)
				addFrontNode(*nodes[i]);
			else
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