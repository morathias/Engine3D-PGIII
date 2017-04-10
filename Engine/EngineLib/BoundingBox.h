#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Renderer.h"

class BoundingBox{
public:
	BoundingBox(Renderer& renderer);
	~BoundingBox();

	void buildBox(const AABB& aabb);
	void updateBox(const AABB& aabb, float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ);

	void draw(Renderer& renderer);

private:
	Renderer& _renderer;
	pg2::IndexBuffer* _indexBuffer;
	pg2::VertexBuffer* _vertexBuffer;

	Vertex _vertices[8];
};

#endif

