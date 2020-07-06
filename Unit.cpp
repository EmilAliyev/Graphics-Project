#include "Unit.h"


//Add polygon array and set scale factor
Unit::Unit(Poly* polyarray, float xwindowsize, float ywindowsize, 
	float zwindowsize, float minx, float miny, float minz, int numberPolygons): polygonarray(polyarray)
{
	polygonarray = polyarray;

	//The scale factor is the maximum between xwindowsize, ywindowsize and zwindowsize
	scaleFactor = xwindowsize;
	if (ywindowsize > scaleFactor)
	{
		scaleFactor = ywindowsize;
	}
	if(zwindowsize > scaleFactor)
	{
		scaleFactor = zwindowsize;
	}

	numPolygons = numberPolygons;

	//Set minimum values of x y and z in world plane
	xmin = minx;
	ymin = miny;
	zmin = minz;
}


Unit::~Unit()
{
}

//Convert all vertices of all polygons in array to NDC
void Unit::convertToNDC()
{
	for (int i = 0; i < numPolygons; i++)
	{
		polygonarray[i].convertToNDC(scaleFactor, xmin, ymin, zmin);
	}

}


void Unit::Create()
{
	//Plane is 1 by 1

	//Convert vertices to NDC coordinates
	convertToNDC();
}
