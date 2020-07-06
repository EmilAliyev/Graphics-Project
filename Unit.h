#pragma once
#include "Polygon.h"

//The NDC (unit) plane
class Unit
{
	Poly* polygonarray;
	float scaleFactor;
	int numPolygons;
	float xmin;
	float ymin;
	float zmin;
public:
	Unit(Poly* polyarray, float xwindowsize, float ywindowsize, float zwindowsize,
		float minx, float miny, float minz, int numberPolygons);
	~Unit();
	void convertToNDC();
	void Create();
};

