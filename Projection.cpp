#include "Projection.h"


//Create projection
Projection::Projection(Poly* polygonarray, int projplane, int numberPolygons)
{
	polygons = polygonarray;
	projectionplane = projplane;
	numPolygons = numberPolygons;
}


Projection::~Projection()
{
}

//Project each polygon into xy, xz or yz plane
void Projection::Project()
{

	for (int i = 0; i < numPolygons; i++)
	{

		polygons[i].Project(projectionplane);
	}
}


