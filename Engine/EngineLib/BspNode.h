#ifndef BSPNODE_H
#define BSPNODE_H

#include <vector>
#include "DLLExport.h"
#include "Entity3D.h"

using namespace std;
//====================================================================
struct D3DXPLANE;
typedef D3DXPLANE* Plane;
//====================================================================
class BspPlane{
public:
	BspPlane();
	~BspPlane();

	void create(float pointA[3], float pointB[3], float pointC[3]);

	Plane plane;
};
//====================================================================
class BspNode{
public:
	BspNode();
	~BspNode();

	void buildPlane(float pointA[3], float pointB[3], float pointC[3]);

	void addFrontNode(BspNode& frontNode);
	void addBackNode(BspNode& backNode);

	void checkBspNodes(vector<BspNode*>& nodes);

	BspResult checkEntity(Entity3D& entity);

	BspPlane& getBspPlane();

	void setName(std::string name);
	std::string getName();

private:
	BspPlane* _bspPlane;
	vector<BspNode*> _frontNodes;
	vector<BspNode*> _backNodes;

	std::string _name;
};
//====================================================================
#endif

