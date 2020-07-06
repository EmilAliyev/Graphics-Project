#pragma once
#include <vector>
#include "Polygon.h"

class Projection
{
	//Project from NDC into xy, xz or yz planes

	Poly* polygons; //the polygon vector
	int projectionplane; //plane to be projected to (1 = xy, 2 = xz, 3 = yz)
	int numPolygons; //Number of polygons
public:
	Projection(Poly* polygonarray, int projplane, int numberPolygons);
	~Projection();
	void Project();
};

