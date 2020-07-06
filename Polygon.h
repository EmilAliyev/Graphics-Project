#pragma once
#include "Vertex.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <algorithm>
#include "Line.h"
#include "Triangle.h"

using namespace std;

//A polygon
class Poly
{
	int numVertices; //The number of vertices
	int numTriangles; //The number of lines

	vector<Vertex> vertices; //vector of vertices
	vector<Line> lines; //vector of lines
	vector<Triangle> triangles; //Triangles in the polygon
	float centroidx; //The x value of the centroid
	float centroidy; //The y value of the centroid
	float centroidz; //The z value of the centroid

	glm::vec3 normal; //The normal of the polygon

	glm::vec3 centroid; //The centroid
public:

	//World plane operations
	Poly();
	~Poly();
	void InitializePolygon(ifstream& inputfile);
	void InitializeVertices(ifstream& inputfile);
	void InitializeTriangles(ifstream& inputfile);
	float getXmin();
	float getXmax();
	float getYmin();
	float getYmax();
	float getZmin();
	float getZmax();
	void writeToOutput(ofstream& output);
	void calculateVertexNormals();
	
	void setVertexColors(glm::vec3 source, glm::vec3 from, glm::vec3 amb, glm::vec3 spec, glm::vec3 diff, float Iamb, float Ilight,
		int phongCons, float distance);

	//Transformation operations
	void Translate(glm::vec3 translate);
	void Scale(float scalex, float scaley, float scalez);
	void Rotate(float rotationangle, glm::vec3 p1, glm::vec3 p2);
	void CalculateCentroid();

	//NDC plane operations
	void convertToNDC(float scaleFactor, float xmin, float ymin, float zmin);

	//Pixel plane operations
	void convertToPixel(int xresolution, int yresolution);

	//For painter's algorithm
	vector<Triangle> getTriangles();

	//Projection operations
	void Project(int projectionplane);

	//Misc operations
	int getNumVertices();
	void setPixel(float *buffer, int x, int y, int height, glm::vec3 color);
	void UpdateNumVertices();
	void verticesToTriangles();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//Unused functions
	void verticesToBuffer(float *buffer, int yResolution);

	//Line drawing (in pixel plane)
	void DrawLinesDDA(float *buffer, int yresolution);

	//Phong model without constant distance
	void setVertexColors(glm::vec3 source, glm::vec3 from, glm::vec3 amb, glm::vec3 spec, glm::vec3 diff, float Iamb, float Ilight,
		int phongCons);
};

