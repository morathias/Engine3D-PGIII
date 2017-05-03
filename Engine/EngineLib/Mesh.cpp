#include "Mesh.h"
#include "pg2_indexbuffer.h"
#include "pg2_vertexbuffer.h"
#include "BoundingBox.h"
#include "Debuger.h"
#include "BspNode.h"
#include "Physics.h"
#include <algorithm>
#include <iostream>

#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib") 
//============================================================================================================
Mesh::Mesh(Renderer& renderer):
	_renderer (renderer),
	_texture (NoTexture),
	_rigidBody(NULL),
	_bspSurvivor(true)
{}
//============================================================================================================
Mesh::~Mesh(){
	delete _vertexBuffer;
	_vertexBuffer = NULL;
	delete _indexBuffer;
	_indexBuffer = NULL;
	delete _boundingBox;
	_boundingBox = NULL;

	Physics::removeRigidBody(*_rigidBody);
}
//============================================================================================================
void Mesh::setMeshData(const TexturedVertex* texVertex, 
					   Primitive ePrimitive, 
					   size_t uiVertexCount, 
					   const unsigned short* pusIndices, 
					   size_t uiIndexCount)
{
	_primitive = ePrimitive;
	_verts = texVertex;

	_indexBuffer = _renderer.createIndexBuffer();
	_vertexBuffer = _renderer.createVertexBuffer(sizeof(TexturedVertex), 0);

	_vertexBuffer->setVertexData(texVertex, uiVertexCount);
	_indexBuffer->setIndexData(pusIndices, uiIndexCount);

	_renderer.setCurrentIndexBuffer(_indexBuffer);
	_renderer.setCurrentVertexBuffer(_vertexBuffer);

	_vertexCount = uiVertexCount;
}
//============================================================================================================
void Mesh::setMeshData(const Vertex* texVertex,
					   Primitive ePrimitive,
					   size_t uiVertexCount,
					   const unsigned short* pusIndices,
					   size_t uiIndexCount)
{
	_primitive = ePrimitive;

	_indexBuffer = _renderer.createIndexBuffer();
	_vertexBuffer = _renderer.createVertexBuffer(sizeof(Vertex), 1);

	_vertexBuffer->setVertexData(texVertex, uiVertexCount);
	_indexBuffer->setIndexData(pusIndices, uiIndexCount);

	_renderer.setCurrentIndexBuffer(_indexBuffer);
	_renderer.setCurrentVertexBuffer(_vertexBuffer);

	_vertexCount = uiVertexCount;
}
//============================================================================================================
void Mesh::draw(Renderer& renderer, CollisionResult parentResult,
	const Frustum& frustum){
	if (_bspSurvivor){
		if (parentResult != CollisionResult::AllOutside){
			_renderer.setCurrentTexture(_texture);
			_renderer.setMatrix(MatrixType::WORLD, _worldTransformationMatrix);

			_vertexBuffer->bind();
			_indexBuffer->bind();

			_renderer.drawCurrentBuffers(_primitive);
			_isDrawn = true;

			Debuger::getBatches();

			if (Debuger::boundingBoxesShown())
				_boundingBox->draw(_renderer);
		}
		else
			_isDrawn = false;
	}
	else
		_isDrawn = false;
	_bspSurvivor = true;
}
//============================================================================================================
void Mesh::setTextureId(int iTextureId, Texture texture){
	_texture = texture;
}
//============================================================================================================
void Mesh::updateBV(){
	D3DXVECTOR3 pos, scale;
	D3DXQUATERNION rot;
	D3DXMatrixDecompose(&scale, &rot, &pos, _worldTransformationMatrix);
	
	float auxMaxX = (_aabb.maxPointX * scale.x) + pos.x;
	float auxMaxY = (_aabb.maxPointY * scale.y) + pos.y;
	float auxMaxZ = (_aabb.maxPointZ * scale.z) + pos.z;

	float auxMinX = (_aabb.minPointX * scale.x) + pos.x;
	float auxMinY = (_aabb.minPointY * scale.y) + pos.y;
	float auxMinZ = (_aabb.minPointZ * scale.z) + pos.z;
	
	_aabb.points[0]->x = auxMinX;		_aabb.points[0]->y = auxMaxY;		_aabb.points[0]->z = auxMinZ;
	_aabb.points[1]->x = auxMaxX;		_aabb.points[1]->y = auxMaxY;		_aabb.points[1]->z = auxMinZ;
	_aabb.points[2]->x = auxMinX;		_aabb.points[2]->y = auxMinY;		_aabb.points[2]->z = auxMinZ;
	_aabb.points[3]->x = auxMaxX;		_aabb.points[3]->y = auxMinY;		_aabb.points[3]->z = auxMinZ;
	_aabb.points[4]->x = auxMinX;		_aabb.points[4]->y = auxMaxY;		_aabb.points[4]->z = auxMaxZ;
	_aabb.points[5]->x = auxMaxX;		_aabb.points[5]->y = auxMaxY;		_aabb.points[5]->z = auxMaxZ;
	_aabb.points[6]->x = auxMinX;		_aabb.points[6]->y = auxMinY;		_aabb.points[6]->z = auxMaxZ;
	_aabb.points[7]->x = auxMaxX;		_aabb.points[7]->y = auxMinY;		_aabb.points[7]->z = auxMaxZ;

	_aabb.max[0] = auxMaxX;		_aabb.max[1] = auxMaxY;		_aabb.max[2] = auxMaxZ;
	_aabb.min[0] = auxMinX;		_aabb.min[1] = auxMinY;		_aabb.min[2] = auxMinZ;

	_boundingBox->updateBox(_aabb, pos.x, pos.y, pos.z, scale.x, scale.y, scale.z);
}
//============================================================================================================
void Mesh::buildAABB(){
	_aabb.minPointX = _verts[0].x; _aabb.minPointY = _verts[0].y; _aabb.minPointZ = _verts[0].z;
	_aabb.maxPointX = _verts[0].x; _aabb.maxPointY = _verts[0].y; _aabb.maxPointZ = _verts[0].z;

	for (size_t i = 1; i < _vertexBuffer->vertexCount(); i++){
		if (_verts[i].x < _aabb.minPointX)
			_aabb.minPointX = _verts[i].x;
		if (_verts[i].y < _aabb.minPointY)
			_aabb.minPointY = _verts[i].y;
		if (_verts[i].z < _aabb.minPointZ)
			_aabb.minPointZ = _verts[i].z;

		if (_verts[i].x > _aabb.maxPointX)
			_aabb.maxPointX = _verts[i].x;
		if (_verts[i].y > _aabb.maxPointY)
			_aabb.maxPointY = _verts[i].y;
		if (_verts[i].z > _aabb.maxPointZ)
			_aabb.maxPointZ = _verts[i].z;
	}

	_boundingBox = new BoundingBox(_renderer);
	_boundingBox->buildBox(_aabb);
}
//============================================================================================================
void Mesh::buildRigidBody(float mass){
	_rigidBody = new RigidBody();
	_rigidBody->create(mass, _posX, _posY, _posZ, _aabb.maxPointX, _aabb.maxPointY, _aabb.maxPointZ);
	Physics::addRigidBody(*_rigidBody);
}
//============================================================================================================
void Mesh::updatePhysics(){
	setPosX(_posX + _rigidBody->getPosX());
	setPosY(_posY + _rigidBody->getPosY());
	setPosZ(_posZ + _rigidBody->getPosZ());

	std::cout << "X: " << _posX + _rigidBody->getPosX() << " Y: " << _posY + _rigidBody->getPosY() << " Z: " << _posZ + _rigidBody->getPosZ()<<std::endl;
}
//============================================================================================================
void Mesh::getNames(vector<string>& names, std::vector<int>& lvlDeep, int lvl){
	names.push_back(getName());
	lvl++;
	lvlDeep.push_back(lvl);
}
//============================================================================================================
void Mesh::updateNames(std::vector<std::string>& names, int& entityIndex){
	names[entityIndex] = getName();
}
//============================================================================================================
void Mesh::updatePolygons(int& meshPolygons) {
	if (_isDrawn)
		meshPolygons += _vertexCount;
	else
		meshPolygons += 0;
}
//============================================================================================================
void Mesh::testBsp(BspNode* node, Camera& camera){
	D3DXVECTOR3 position, direction;
	position.x = _posX;
	position.y = _posY;
	position.z = _posZ;

	direction = *node->getBspPlane().position - position;

	D3DXVECTOR3 cameraPosition, cameraDirection;
	cameraPosition.x = camera.posX();
	cameraPosition.y = camera.posY();
	cameraPosition.z = camera.posZ();

	cameraDirection = *node->getBspPlane().position - cameraPosition;

	D3DXVECTOR3 planeNormal;
	planeNormal.x = node->getBspPlane().plane->a;
	planeNormal.y = node->getBspPlane().plane->b;
	planeNormal.z = node->getBspPlane().plane->c;
	D3DXVec3Normalize(&planeNormal, &planeNormal);

	float dist = D3DXVec3Dot(&direction, &planeNormal);
	float cameraDist = D3DXVec3Dot(&cameraDirection, &planeNormal);

	if (dist > 0.001){

		if (cameraDist > 0){
			if (node->getFrontNodes().empty())
				std::cout << _name << " al frente de " << node->getName() << std::endl;
			else
				node->getFrontNodes()[0]->checkEntity(*this, camera);
		}

		else{
			_bspSurvivor = false;
			return;
		}
	}

	else if (dist < -0.001){

		if (cameraDist < 0){
			if (node->getBackNodes().empty())
				std::cout << _name << " detras de " << node->getName() << std::endl;
			else
				node->getBackNodes()[0]->checkEntity(*this, camera);
		}

		else{
			_bspSurvivor = false;
			return;
		}
	}	
}
//============================================================================================================
void Mesh::bspKill(){
	_bspSurvivor = false;
}
