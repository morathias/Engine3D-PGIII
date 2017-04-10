#include "BoundingBox.h"
#include "pg2_indexbuffer.h"
#include "pg2_vertexbuffer.h"
#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")
#include<iostream>

BoundingBox::BoundingBox(Renderer& renderer) :
_renderer(renderer)
{}

BoundingBox::~BoundingBox(){
	delete _vertexBuffer;
	_vertexBuffer = NULL;
	delete _indexBuffer;
	_indexBuffer = NULL;
}

void BoundingBox::buildBox(const AABB& aabb){
	_vertices[0].x = aabb.minPointX;	_vertices[0].y = aabb.maxPointY;	_vertices[0].z = aabb.minPointZ;	_vertices[0].color = D3DCOLOR_ARGB(255, 0, 255, 0);
	_vertices[1].x = aabb.maxPointX;	_vertices[1].y = aabb.maxPointY;	_vertices[1].z = aabb.minPointZ;	_vertices[1].color = D3DCOLOR_ARGB(255, 0, 255, 0);
	_vertices[2].x = aabb.minPointX;	_vertices[2].y = aabb.minPointY;	_vertices[2].z = aabb.minPointZ;	_vertices[2].color = D3DCOLOR_ARGB(255, 0, 255, 0);
	_vertices[3].x = aabb.maxPointX;	_vertices[3].y = aabb.minPointY;	_vertices[3].z = aabb.minPointZ;	_vertices[3].color = D3DCOLOR_ARGB(255, 0, 255, 0);
	_vertices[4].x = aabb.minPointX;	_vertices[4].y = aabb.maxPointY;	_vertices[4].z = aabb.maxPointZ;	_vertices[4].color = D3DCOLOR_ARGB(255, 0, 255, 0);
	_vertices[5].x = aabb.maxPointX;	_vertices[5].y = aabb.maxPointY;	_vertices[5].z = aabb.maxPointZ;	_vertices[5].color = D3DCOLOR_ARGB(255, 0, 255, 0);
	_vertices[6].x = aabb.minPointX;	_vertices[6].y = aabb.minPointY;	_vertices[6].z = aabb.maxPointZ;	_vertices[6].color = D3DCOLOR_ARGB(255, 0, 255, 0);
	_vertices[7].x = aabb.maxPointX;	_vertices[7].y = aabb.minPointY;	_vertices[7].z = aabb.maxPointZ;	_vertices[7].color = D3DCOLOR_ARGB(255, 0, 255, 0);

	unsigned short indices[] = {
		0, 1, 1, 3, 3, 2, 2, 0,
		1, 5, 5, 7, 7, 3, 3, 1,
		5, 4, 4, 6, 6, 7, 7, 5,
		0, 4, 4, 6, 6, 2, 2, 0,
		0, 1, 1, 5, 5, 4, 4, 0,
		2, 3, 3, 7, 7, 6, 6, 2
	};

	_indexBuffer = _renderer.createIndexBuffer();
	_vertexBuffer = _renderer.createVertexBuffer(sizeof(Vertex), 1);

	_vertexBuffer->setVertexData(_vertices, 8);
	_indexBuffer->setIndexData(indices, 48);

	_renderer.setCurrentIndexBuffer(_indexBuffer);
	_renderer.setCurrentVertexBuffer(_vertexBuffer);
}

void BoundingBox::updateBox(const AABB& aabb, float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ){
	for (size_t i = 0; i < 8; i++){
		_vertices[i].x = (aabb.points[i]->x - posX) / scaleX;
		_vertices[i].y = (aabb.points[i]->y - posY) / scaleY;
		_vertices[i].z = (aabb.points[i]->z - posZ) / scaleZ;
		_vertices[i].color = D3DCOLOR_ARGB(255, 0, 255, 0);
	}

	_vertexBuffer->setVertexData(_vertices, 8);
	_renderer.setCurrentVertexBuffer(_vertexBuffer);
}

void BoundingBox::draw(Renderer& renderer){
	_vertexBuffer->bind();
	_indexBuffer->bind();

	_renderer.drawCurrentBuffers(LINELIST);
}
