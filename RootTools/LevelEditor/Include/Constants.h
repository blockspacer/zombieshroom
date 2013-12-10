#pragma once

#include <glm/glm.hpp>
#include <string>
#include <iostream>
using namespace std;
const int g_maxSceneObjects = 1000;
const int g_maxMeshes = 50;
const int g_maxCameras = 15;
const int g_maxLights = 10;

const int g_maxVerticesPerMesh = 10000;

struct Transform
{
	int index;
	char name[30];
	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;
};

struct Mesh
{
	Transform transformation;
	int nrOfVertices;
	glm::vec3 vertex[g_maxVerticesPerMesh];
	glm::vec2 UV[g_maxVerticesPerMesh];
	glm::vec3 normal[g_maxVerticesPerMesh];
	char texturePath[100];
	char normalPath[100];
	int MaterialID; //Added so that a mesh can get a material on 3d viewers side.
	//int indices[g_maxVerticesPerMesh*3];
	char materialName[30];
};

struct Material
{
	char materialName[30];
	char texturePath[100];
	char normalPath[100];	
};

struct Camera
{
	Transform transformation;

	float verticalFieldOfView;
	float horizontalFieldOfView;
	float aspectRatio;
	float nearClippingPlane;
	float farClippingPlane;

	glm::vec3 upDirection;
	glm::vec3 rightDirection;
	glm::vec3 viewDirection;

	glm::vec3 eyePoint;

};

struct Light
{
	Transform transformation;
};

