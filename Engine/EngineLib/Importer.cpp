#include "Importer.h"
#include "Utility.h"
#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include <stack>
#include <iostream>
//=============================================================================================================
Importer::Importer(Renderer& renderer):
_renderer(renderer){}
//=============================================================================================================
Importer::~Importer(){}
//=============================================================================================================
bool Importer::importScene(const std::string& fileName, Nodo& rootNode){
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) return false;

	processNode(rootNode, *scene->mRootNode, *scene);
	rootNode.setRotation(0, rootNode.rotationY(), rootNode.rotationZ());
	return true;
}
//=============================================================================================================
bool Importer::processNode(Nodo& nodo, aiNode& assimpNode, const aiScene& scene){
	nodo.setName(assimpNode.mName.C_Str());
	cout << nodo.getName() << endl;

	aiVector3t<float> position, scaling;
	aiQuaterniont<float> rotation;
	assimpNode.mTransformation.Decompose(scaling, rotation, position);

	nodo.setPosX(position.x);
	nodo.setPosY(position.y);
	nodo.setPosZ(position.z);
	nodo.setScale(scaling.x, scaling.y, scaling.z);
	nodo.setRotation(rotation.x, rotation.y, rotation.z);

	for (size_t j = 0; j < assimpNode.mNumMeshes; j++){
		Mesh& mesh = processMesh(*scene.mMeshes[assimpNode.mMeshes[j]], assimpNode, scene);
		mesh.setParent(&nodo);
		nodo.addChild(mesh);
	}

	for (size_t i = 0; i < assimpNode.mNumChildren; i++)
	{
		Nodo* nodoHijo = new Nodo();

		nodo.addChild(*nodoHijo);
		nodoHijo->setParent(&nodo);

		processNode(*nodoHijo, *assimpNode.mChildren[i], scene);
	}

	return true;
}
//=============================================================================================================
Mesh& Importer::processMesh(aiMesh& assimpMesh, aiNode& assimpNode, const aiScene& scene){
	Mesh* mesh = new Mesh(_renderer);
	string name = assimpNode.mName.C_Str();
	mesh->setName(name + "_mesh");
	cout << mesh->getName() << endl;
	TexturedVertex* verts = new TexturedVertex[assimpMesh.mNumVertices];
	if (assimpMesh.HasTextureCoords(0)){
		for (size_t i = 0; i < assimpMesh.mNumVertices; i++)
		{
			verts[i] = { assimpMesh.mVertices[i].x,
				assimpMesh.mVertices[i].y,
				assimpMesh.mVertices[i].z,
				assimpMesh.mTextureCoords[0][i].x,
				assimpMesh.mTextureCoords[0][i].y
			};
		}
	}

	else{
		for (size_t i = 0; i < assimpMesh.mNumVertices; i++){
			verts[i] = { assimpMesh.mVertices[i].x,
				assimpMesh.mVertices[i].y,
				assimpMesh.mVertices[i].z,
			};
		}
	}

	unsigned int numIndices = assimpMesh.mNumFaces * 3;
	unsigned short* indices = new unsigned short[numIndices];
	int index = 0;

	for (unsigned int i = 0; i < assimpMesh.mNumFaces; i++)
	{
		indices[i * 3 + 0] = assimpMesh.mFaces[i].mIndices[0];
		indices[i * 3 + 1] = assimpMesh.mFaces[i].mIndices[1];
		indices[i * 3 + 2] = assimpMesh.mFaces[i].mIndices[2];
	}

	mesh->setMeshData(verts, TRIANGLELIST, assimpMesh.mNumVertices, indices, numIndices);
	mesh->buildAABB();
	aiVector3t<float> position, scaling;
	aiQuaterniont<float> rotation;
	assimpNode.mTransformation.Decompose(scaling, rotation, position);
	
	mesh->setPosX(position.x);
	mesh->setPosY(position.y);
	mesh->setPosZ(position.z);
	mesh->setScale(scaling.x, scaling.y, scaling.z);
	mesh->setRotation(rotation.x, rotation.y, rotation.z);
	cout << mesh->posX() << " " << mesh->posY() << " " << mesh->posZ() << endl;
	
	mesh->updateWorldTransformation();
	mesh->updateBV();

	aiString path;
	std::stack<char> _stack;
	if (scene.mMaterials[assimpMesh.mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
	{
		string fullPath = path.data;
		for (size_t i = fullPath.length(); i > 0; i--){
			if (fullPath[i] == '/')
				break;

			_stack.push(fullPath[i - 1]);
		}

		fullPath.clear();
		while (!_stack.empty()){
			fullPath += _stack.top();
			_stack.pop();
		}
		mesh->setTextureId(0, _renderer.loadTexture("Assets/" + fullPath, D3DCOLOR_XRGB(255, 0, 255)));
	}

	return *mesh;
}
//=============================================================================================================
void Importer::showNodeNames(aiNode& assimpNode, const aiScene& scene){
	cout << assimpNode.mName.C_Str() << endl;
	for (size_t i = 0; i < assimpNode.mNumMeshes; i++)
	{
		showMeshNames(*scene.mMeshes[assimpNode.mMeshes[i]], scene);
	}

	for (size_t i = 0; i < assimpNode.mNumChildren; i++)
	{
		showNodeNames(*assimpNode.mChildren[i], scene);
	}
}
//=============================================================================================================
void Importer::showMeshNames(aiMesh& assimpMesh, const aiScene& scene){
	cout << assimpMesh.mName.C_Str()<<endl;
}
//=============================================================================================================