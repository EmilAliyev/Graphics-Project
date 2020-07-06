#pragma once
#include "Vertex.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Line
{
	//A line of a polygon

	//Vertices this line connects
	Vertex vertex1;
	Vertex vertex2;

	//Vertex indices this line connects
	int v1index;
	int v2index;
public:
	Line();
	Line(int v1, int v2);
	~Line();
	void InitializeLine(ifstream &inputfile, vector<Vertex> vertices);

	//World plane
	void WriteToOutput(ofstream &outputfile);

	//Pixel Plane

	//Line Drawing (triangle-by-triangle rendering)
	void DrawLineDDATri(float *buffer, int bufferheight);
	void ExtremeSlope(int startx, int endx, int starty, int endy, float *buffer, int yresolution);
	glm::vec3 GouraudShading(int x, int y);
	glm::vec2 verticalintersection(float x1, float x2, float y1, float y2, float xvertical);
	glm::vec2 horizontalintersection(float x1, float x2, float y1, float y2, float yhorizontal);
	void setPixel(float *buffer, int x, int y, int height);
	void setPixelColor(float *buffer, int x, int y, int height, glm::vec3 color);

	

	//Misc
	void setVertices(vector<Vertex> vertices);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~UNUSED FUNCTIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//Polygon-by-polygon rendering

	//Line Drawing
	void DrawLineDDA(float * buffer, int yresolution, vector<Vertex> vertices);
	void ExtremeSlope(int startx, int endx, int starty, int endy, float *buffer, int yresolution, vector<Vertex> vertices);
	glm::vec3 GouraudShading(int x, int y, vector<Vertex> vertices);
};

