#include "Entity3D.h"
#include "Nodo.h"
#include "BoundingBox.h"
//==================================================================================
#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")
#include <algorithm>
//==================================================================================
using namespace std;
//==================================================================================
Entity3D::Entity3D()
	:
	_transformationMatrix(new D3DXMATRIX()),
	_worldTransformationMatrix(new D3DXMATRIX()),
	_posX(0),
	_posY(0),
	_posZ(0),
	_previusPosX(0),
	_previusPosY(0),
	_rotationX(0),
	_rotationY(0),
	_rotationZ(0),
	_scaleX(1.0f),
	_scaleY(1.0f),
	_scaleZ(1.0f),
	_name(""),
	_parent(NULL),
	_isDrawn(true),
	_boundingBox(NULL)
{
	for (size_t i = 0; i < 8; i++)
		_aabb.points[i] = new D3DXVECTOR3();

	updateLocalTransformation();
}
//==================================================================================
Entity3D::~Entity3D(){
}
//==================================================================================
void Entity3D::setPosX(float fPosX){
	_previusPosX = _posX;

	_posX = fPosX;

	updateLocalTransformation();
}
//==================================================================================
void Entity3D::setPosY(float fPosY){
	_previusPosY = _posY;

	_posY = fPosY;

	updateLocalTransformation();
}
//==================================================================================
void Entity3D::setPosZ(float fPosZ){
	_posZ = fPosZ;

	updateLocalTransformation();
}
//==================================================================================
void Entity3D::setRotation(float rotationX, float rotationY, float rotationZ, float rotationW){
	_rotationX = rotationX;
	_rotationY = rotationY;
	_rotationZ = rotationZ;
	_rotationW = rotationW;

	updateLocalTransformation();
}
//==================================================================================
void Entity3D::setScale(float fScaleX, float fScaleY){
	_scaleX = fScaleX;
	_scaleY = fScaleY;

	updateLocalTransformation();
}
//==================================================================================
void Entity3D::setScale(float fScaleX, float fScaleY, float scaleZ){
	_scaleX = fScaleX;
	_scaleY = fScaleY;
	_scaleZ = scaleZ;

	updateLocalTransformation();
}
//==================================================================================
float Entity3D::posX() const{
	return _posX;
}
//==================================================================================
float Entity3D::posY() const{
	return _posY;
}
//==================================================================================
float Entity3D::posZ() const{
	return _posZ;
}
//==================================================================================
float Entity3D::previusPosX() const{
	return _previusPosX;
}
//==================================================================================
float Entity3D::previusPosY() const{
	return _previusPosY;
}
//==================================================================================
float Entity3D::rotationX() const{
	return _rotationX;
}
//==================================================================================
float Entity3D::rotationY() const{
	return _rotationY;
}
//==================================================================================
float Entity3D::rotationZ() const{
	return _rotationZ;
}
//==================================================================================
float Entity3D::rotationW() const{
	return _rotationW;
}
//==================================================================================
float Entity3D::scaleX() const{
	return _scaleX;
}
//==================================================================================
float Entity3D::scaleY() const{
	return _scaleY;
}
//==================================================================================
float Entity3D::scaleZ() const{
	return _scaleZ;
}
//==================================================================================
void Entity3D::setParent(Nodo* parent){
	if (!_parent) _parent = parent;
}
//==================================================================================
void Entity3D::setName(string name){
	_name = name;
}
//==================================================================================
string Entity3D::getName() const{
	if (_isDrawn)
		return _name;

	return "";
}
//==================================================================================
void Entity3D::updateLocalTransformation(){

	D3DXMATRIX traslatrionMat;
	D3DXMatrixTranslation(&traslatrionMat, _posX, _posY, _posZ);

	D3DXMATRIX rotationMat;
	D3DXQUATERNION quaternion;
	quaternion.x = _rotationX;	quaternion.y = _rotationY;	quaternion.z = _rotationZ;	quaternion.w = _rotationW;
	D3DXMatrixRotationQuaternion(&rotationMat, &quaternion);
	//D3DXMatrixRotationYawPitchRoll(&rotationMat, _rotationY, _rotationX, _rotationZ);

	D3DXMATRIX scaleMat;
	D3DXMatrixScaling(&scaleMat, _scaleX, _scaleY, _scaleZ);

	D3DXMatrixIdentity(_transformationMatrix);
	D3DXMatrixMultiply(_transformationMatrix, &traslatrionMat, _transformationMatrix);
	D3DXMatrixMultiply(_transformationMatrix, &rotationMat, _transformationMatrix);
	D3DXMatrixMultiply(_transformationMatrix, &scaleMat, _transformationMatrix);
}
//==================================================================================
void Entity3D::updateWorldTransformation(){
	updateLocalTransformation();

	if (_parent){
		D3DXMatrixIdentity(_worldTransformationMatrix);
		D3DXMatrixMultiply(_worldTransformationMatrix, _transformationMatrix, _parent->worldMatrix());
	}

	else
		_worldTransformationMatrix = _transformationMatrix;
}
//==================================================================================
const Matrix& Entity3D::worldMatrix() const{
	return _worldTransformationMatrix;
}
//==================================================================================
const Vector3* Entity3D::getPoints() const{
	return _aabb.points;
}
//==================================================================================
const AABB& Entity3D::getAABB() const{
	return _aabb;
}
//==================================================================================
Entity3D& Entity3D::findEntity(string name){
	if (_name == name)
		return *this;
}
