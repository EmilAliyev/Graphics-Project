#pragma once
#include "Polygon.h"
#include <glm/glm.hpp>
#include <vector>
#include <cmath>

//Megapixel dimensions
const int MEGAPIXEL_WIDTH = 3;
const int MEGAPIXEL_HEIGHT = 3;

//The projection planes
const int PLANE_XY = 1;
const int PLANE_XZ = 2;
const int PLANE_YZ = 3;

//Axis to use for depth
const int X_AXIS = 1;
const int Y_AXIS = 2;
const int Z_AXIS = 3;

class PixelPlane
{
	Poly *polygonarray; //array of polygons to draw
	vector<Triangle> trianglearray; //Triangle array to draw triangle by triangle
	int xresolution; //resolution of the display in x
	int yresolution; //resolution of the display in y
	float *pixelBuffer; //Draw final image here
	int numPolygons; //Number of polygons to draw
	float *polygonBuffer; //Draw polygons here before placing in pixelBuffer
	int algorithm; //Specifies which algorithm should be used to draw lines. 1 for DDA, 2 for Bresenham.
	int projectionPlane; //Specifies the plane being projected to
public:
	PixelPlane();
	PixelPlane(Poly *polygonarray, int xresolution, int yresolution, int numPolygonsinput, int algo, int projplane);
	~PixelPlane();
	void Create();
	void ConvertToPixel();
	float *GetPixelBuffer();
	void PolyToPixelBuffer();
	void ClearPolyBuffer();
	

	//Draw triangle by triangle
	void DrawTriangles();
	void DrawTriangle(Triangle triangle);

	//Sort triangles
	void sortTrianglesByDepth();

	//Half toning
	float* halfTone(float *pixelbuffer, int xresolution, int yresolution);
	glm::vec3 getMegaPixelColor(float *buffer, int x, int y, int height);
	void setMegaPixelColor(glm::vec3 color, float *buffer, int x, int y, int height);
	void MegaPixelOn(int numpixelson, float *buffer, int x, int y, int height, glm::vec3 intensityratios);
	void MegaPixelSetRandPixel(int x, int y, glm::vec3 color, float *buffer, int height);

	glm::vec2 megaPixelToPixel(int x, int y, int i, int j);

	//Misc
	glm::vec3 getPixelColor(float *buffer, int x, int y, int height);
	bool isZero(int x, int y, int height, float *buffer);
	void setPixel(float *buffer, int x, int y, int height, glm::vec3 color);
	
	//Unused functions

	//Polygon-by-polygon rendering
	void DrawLines(int polygonNumber);
	void DrawPolygons();
	void DrawLinesDDA(int polygonNumber);
	void PlaceVertiesInPolygonBuffer(int polygonNumber);

};

