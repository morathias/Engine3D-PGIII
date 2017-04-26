#include "BspTree.h"
#include <iostream>
//==========================================================
BspTree::BspTree(){}
//==========================================================
BspTree::~BspTree(){}
//==========================================================
void BspTree::buildTree(){
	addBspNodes();
	_nodes[0]->checkBspNodes(_nodes);
}
//==========================================================
vector<BspNode*> BspTree::_staticNodes;
void BspTree::addNode(BspNode& node){
	_staticNodes.push_back(&node);
}
//==========================================================
void BspTree::addBspNodes(){
	for (size_t i = 0; i < _staticNodes.size(); i++)
		_nodes.push_back(_staticNodes[i]);
}
//==========================================================
void BspTree::checkEntity(Entity3D& entity){

}
//==========================================================