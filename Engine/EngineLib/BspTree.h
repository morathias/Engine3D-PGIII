#ifndef BSPTREE_H
#define BSPTREE_H
//==========================================================
#include "BspNode.h"
#include "Nodo.h"
//==========================================================
class BspTree{
public:
	DllExport BspTree();
	DllExport ~BspTree();

	DllExport void buildTree();
	DllExport static void addNode(BspNode& node);
	void addBspNodes();

	DllExport void checkEntity(Entity3D& entity);

private:
	static vector<BspNode*> _staticNodes;
	vector<BspNode*> _nodes;
};
//==========================================================
#endif