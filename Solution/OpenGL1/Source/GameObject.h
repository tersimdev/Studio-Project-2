#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <vector>
#include "Mesh.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Vector3.h"

class GameObject
{
public:
	GameObject();
	//init obj
	void Init(std::string filePathOBJ, std::string filePathTGA = "", Vector3 translate = NULL, Vector3 rotate = NULL, Vector3 scale = NULL);
	//init primitive with texture, put "" for filepath if no texture
	void Init(Mesh* mesh, std::string filePathTGA = "", Vector3 translate = NULL, Vector3 rotate = NULL, Vector3 scale = NULL);
	
	//basically just calls the Mesh::Render
	void Render();
	void Render(unsigned offset, unsigned count);
	
	//virtual void RenderChildren(); //put stuff that is under this object's hierachy here

	void AddTexture(std::string filePathTGA); //adds a tga to a list of textures

	//getter setters
	unsigned GetTextureID(); //getter for mesh->textureID
	void ChangeTexture(int textureIndex); //setter for textureID
	Material* GetMaterial(); //getter for mesh->material
	void SetMaterial(Material);
	Vector3 GetTranslate();
	Vector3 GetRotate();
	Vector3 GetScale();
	void SetTranslate(Vector3);
	void SetRotate(Vector3);
	void SetScale(Vector3);
	void IncrementTranslate(Vector3); //add to translate
	void IncrementRotate(Vector3); //add to rotate
	void IncrementScale(Vector3); //add to scale

	~GameObject();
protected:
	Mesh* objectMesh;
	std::vector<unsigned> textureIDs;

	Vector3 translate, rotate, scale;
};

#endif // !GAMEOBJECT_H
