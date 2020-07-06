#pragma once
#include "Vertex.h"
#include "Line.h"
#include <vector>

//projection plane constants
const int DEPTH_X = 1;
const int DEPTH_Y = 2;
const int DEPTH_Z = 3;

class Triangle
{

	//vertex indices
	int v1index;
	int v2index;
	int v3index;

	//Vertices
	Vertex v1;
	Vertex v2;
	Vertex v3;

	Line lines[3];

	glm::vec3 depth; //Depth in each direction

	float projdepth; //Depth in one direction (x, y, or z)

	glm::vec3 normal;

	
public:
	Triangle();
	~Triangle();

	void InitializeTriangle(ifstream &inputfile, vector<Vertex> vertices);

	void CreateLines();

	void WriteToOutput(ofstream &outputfile);

	//World Plane

	//Calculating the depth
	void calculateDepth(vector<Vertex> vertices);
	float MinX(vector<Vertex> vertices);
	float MinY(vector<Vertex> vertices);
	float MinZ(vector<Vertex> vertices);

	glm::vec3 getNormal();

	//Misc
	bool containsVertex(int vertexindex);

	//NDC Plane

	//Projection Plane

	//Pixel Plane
	void setPixel(float *buffer, int x, int y, int height, glm::vec3 color);

	//Rasterization
	glm::vec3 GouraudShading(int x, int y, Vertex v1, Vertex v2, Vertex v3);
	float calculateArea(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);

	//Triangle-by-triangle rendering

	//Drawing the vertices
	void drawVertices(float *buffer, int bufferheight);

	//Drawing the lines
	void drawLinesDDA(float *buffer, int bufferHeight);

	//Rasterization
	void Rasterize(int xresolution, int yresolution, float *polygonBuffer);
	bool RasterizeException(int x, int y, int xresolution, int yresolution, float *polygonBuffer);
	bool RastHorizontalLine(int x, int y, int xresolution, int yresolution, float *polygonBuffer);
	bool RastVertex(int x, int y, int yresolution, float *polygonBuffer);
	bool loneHorizontalLine(int y, int xresolution, int yresolution, float *polygonBuffer);

	//Misc
	bool isZero(int x, int y, int height, float *buffer);
	void shadePixel(int x, int y, int height, float *buffer, Vertex v1, Vertex v2, Vertex v3);
	void setVertices(vector<Vertex> vertices);
	void verticesToLines(vector<Vertex> vertices);
	void setProjDepth(int depthaxis);

	friend bool operator <(Triangle t1, Triangle t2);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~Experimental Functions~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//Experimental Rasterization algorithm
	
	void RasterizeTriangle(int xresolution, int yresolution, float *buffer);
	int numPixelsInRow(float *buffer, int y, int xresolution, int yresolution);
	int leftMostX(float *buffer, int y, int xresolution, int yresolution);
	int rightMostX(float *buffer, int y, int xresolution, int yresolution);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~Unused functions~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	//Calculate the normal (incomplete)
	void calculateNormal(vector <Vertex> vertices, int vertexnum);

	//Polygon by polygon rendering

	//Line Drawing (polygon by polgon)
	void DrawLinesDDA(float *buffer, int yresolution, vector<Vertex> vertices);
};

