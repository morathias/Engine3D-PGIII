#ifndef BSPNODE_H
#define BSPNODE_H

#include <vector>
#include "DLLExport.h"
#include "Mesh.h"

using namespace std;
//====================================================================
struct D3DXPLANE;
typedef D3DXPLANE* Plane;
//====================================================================
class BspPlane{
public:
	BspPlane();
	~BspPlane();

	void create(float pointA[3], float pointB[3], float pointC[3], Mesh& mesh);

	Plane plane;
	Vector3 position;
};
//====================================================================
class BspNode{
public:
	BspNode();
	~BspNode();

	void buildPlane(float pointA[3], float pointB[3], float pointC[3], Mesh& planeMesh);

	void addFrontNode(BspNode& frontNode);
	void addBackNode(BspNode& backNode);

	vector<BspNode*>& getFrontNodes();
	vector<BspNode*>& getBackNodes();

	void checkBspNodes(vector<BspNode*>& nodes);
	void checkEntity(Mesh& mesh, Camera& camera);

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

