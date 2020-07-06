#pragma once


#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <cmath>
#include <vector>

using namespace std;
const float pi = 3.141592f;

//Projection planes
const int XY = 1;
const int XZ = 2;
const int YZ = 3;

class Vertex
{
	
	glm::vec3 world; //World coordinates

	glm::vec3 NDC; //NDC coordinates

	glm::vec2 proj; //Projection coordiantes

	int xPixel; //x value of vertex as pixel
	int yPixel; //y value of vertex as pixel

	glm::vec3 color; //The color of the pixel

	glm::vec3 normal; //The normal of the vertex

public:
	Vertex();
	Vertex(glm::vec3 coordinates);
	~Vertex();

	//World operations

	float getX();
	float getY();
	float getZ();
	void setInitialCoordinates(ifstream& inputfile);

	//Phong Illumination
	void setColor(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, glm::vec3 from, glm::vec3 source,
		float Ia, float Il, int n, float distance);
	glm::vec3 getColor();

	void calculateNormal(vector<glm::vec3> trianglenormals);

	void normalizeColor();

	float maxRGB();

	//Transformations
	void Translate(glm::vec3 translate);
	void Scale(float scalex, float scaley, float scalez, glm::vec3 center);
	void Rotate(float rotationangle, glm::vec3 p1, glm::vec3 p2);
	glm::mat3 RotateToXZ(glm::vec3 rot);
	glm::mat3 RotateToZ(glm::vec3 rot);

	void WriteToOutput(ofstream& outputfile);

	//NDC Operations
	void convertToNDC(float scaleFactor, float xmin, float ymin, float zmin);

	//Pixel Operations

	int getXPixel();
	int getYPixel();
	void convertToPixel(int xresolution, int yresolution);

	//Projection
	void Project(int plane);



	//~~~~~~~~~~~~~~~~~~~~~~~~Unused Functions~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	//Illumination (distance is not a constant here)
	void setColor(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, glm::vec3 from, glm::vec3 source,
		float Ia, float Il, int n);

	void Shift(glm::vec3 shift);
};

