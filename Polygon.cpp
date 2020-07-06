#include "Polygon.h"

//Create vertices for polygon using number of vertices and coordinates from input file
void Poly::InitializeVertices(ifstream& inputfile)
{
	//Determine number of vertices
	inputfile >> numVertices;

	Vertex *vertexptr; //Vertex pointer
	
	//Initialize new vertices and add to vertex vector
	for (int i = 0; i < numVertices; i++)
	{
		vertexptr = new Vertex();
		vertices.push_back(*vertexptr);
	}

	//Set coordinates of new vertices
	for (int i = 0; i < numVertices; i++)
	{
		vertices[i].setInitialCoordinates(inputfile);
	}
}

//Initialize the lines using the input file
void Poly::InitializeTriangles(ifstream & inputfile)
{
	//Get number of lines
	inputfile >> numTriangles;

	Triangle *trptr;

	//Create new lines and add to vector
	for (int i = 0; i < numTriangles; i++)
	{
		trptr = new Triangle();
		triangles.push_back(*trptr);
	}

	//Set vertices of new lines
	for (int i = 0; i < numTriangles; i++)
	{
		triangles[i].InitializeTriangle(inputfile, vertices);
	}
}

//Get minimum x value among vertices
float Poly::getXmin()
{
	int numVertices = vertices.size();
	float minx = vertices[0].getX(); //Initialize the minimum x to the first vertex's x value

	//Loop through vertices to find minimum x
	for (int i = 0; i < numVertices; i++)
	{
		if (minx > vertices[i].getX())
		{
			minx = vertices[i].getX();
		}
	}

	return minx; //Return the minimum x found
}

//Get maximum x value among vertices
float Poly::getXmax()
{
	int numVertices = vertices.size();
	float maxx = vertices[0].getX(); //Initialize the maximum x to the first vertex's x value

	//Loop through vertices to find maximum x
	for (int i = 0; i < numVertices; i++)
	{
		if (maxx < vertices[i].getX())
		{
			maxx = vertices[i].getX();
		}
	}

	return maxx; //Return the maximum x found
}

//get minimum y value among vertices
float Poly::getYmin()
{
	int numVertices = vertices.size();
	float miny = vertices[0].getY(); //Initialize the minimum y to the first vertex's y value
	
	//Loop through vertices to find minimum x
	for (int i = 0; i < numVertices; i++)
	{
		if (miny > vertices[i].getY())
		{
			miny = vertices[i].getY();
		}
	}

	return miny; //Return the minimum x found
}

//get maximum y value among vertices
float Poly::getYmax()
{
	int numVertices = vertices.size();
	float maxy = vertices[0].getY(); //Initialize the maximum y to the first vertex's y value

	//Loop through vertices to find minimum x
	for (int i = 0; i < numVertices; i++)
	{
		if (maxy < vertices[i].getY())
		{
			maxy = vertices[i].getY();
		}
	}

	return maxy; //Return the minimum x found
}

//Get minimum z among vertices
float Poly::getZmin()
{
	int numVertices = vertices.size();
	float minz = vertices[0].getZ(); //Initialize the minimum z to the first vertex's z value

	//Loop through vertices to find minimum z
	for (int i = 0; i < numVertices; i++)
	{
		if (minz > vertices[i].getZ())
		{
			minz = vertices[i].getZ();
		}
	}

	return minz; //Return the minimum z found
}

//Get maximum z among vertices
float Poly::getZmax()
{
	int numVertices = vertices.size();
	float maxz = vertices[0].getZ(); //Initialize the maximum z to the first vertex's y value

	//Loop through vertices to find maximum z
	for (int i = 0; i < numVertices; i++)
	{
		if (maxz < vertices[i].getZ())
		{
			maxz = vertices[i].getZ();
		}
	}

	return maxz; //Return the minimum x found
}

//Write polygon info to output file
void Poly::writeToOutput(ofstream &output)
{
	//First line for polygon = number of vertices
	output << numVertices << endl;

	//For each vertex, write x, y and z coordinates
	for (int i = 0; i < numVertices; i++)
	{
		vertices[i].WriteToOutput(output);
	}

	//Write number of lines
	output << numTriangles << endl;

	//For each line, write connecting vertices
	for (int i = 0; i < numTriangles; i++)
	{
		triangles[i].WriteToOutput(output);
	}
}

//Calculate the normal at each vertex (not used)
void Poly::calculateVertexNormals()
{
	//Calculate the normal for each vertex
	for (int i = 0; i < numVertices; i++)
	{
		vector<Triangle> TrianglesContainingVertex; //vector of triangles containing the vertex

		//Check each triangle to see if it contains vertex i. If so, add to the vector
		for (int j = 0; j < numTriangles; j++)
		{
			if (triangles[j].containsVertex(i))
			{
				TrianglesContainingVertex.push_back(triangles[j]);
			}
		}

		vector<glm::vec3> TriangleNormals; //vector of triangle normals

		int numtrianglesContainingVertex = TrianglesContainingVertex.size();

		//Calculate the normal of each triangle containing the vertex
		//and push that normal to the vector of normals
		for (int j = 0; j < numtrianglesContainingVertex; j++)
		{
			TrianglesContainingVertex[j].calculateNormal(vertices, i);
			TriangleNormals.push_back(TrianglesContainingVertex[j].getNormal());
		}
	}

	normal = glm::vec3(0, 0, 0);
}

//Set the color of each vertex using the phong model
void Poly::setVertexColors(glm::vec3 source, glm::vec3 from, glm::vec3 amb, glm::vec3 spec, glm::vec3 diff,
	float Iamb, float Ilight, int phongCons, float distance)
{
	//Set the color of each vertex
	for (int i = 0; i < numVertices; i++)
	{
		vertices[i].setColor(amb, diff, spec, from, source, Iamb, Ilight, phongCons, distance);
	}
}

//Convert coordinates of all vertices to NDC
void Poly::convertToNDC(float scaleFactor, float xmin, float ymin, float zmin)
{
	for (int i = 0; i < numVertices; i++)
	{
		vertices[i].convertToNDC(scaleFactor, xmin, ymin, zmin);
	}
}

//Convert all vertices to pixel plane
void Poly::convertToPixel(int xresolution, int yresolution)
{
	//Loop through each vertex and convert its coordinates to pixel plane
	for (int i = 0; i < numVertices; i++)
	{
		vertices[i].convertToPixel(xresolution, yresolution);
	}
}

//Return the triangles
vector<Triangle> Poly::getTriangles()
{
	return triangles;
}

//Constructor
Poly::Poly()
{
}

//Destructor
Poly::~Poly()
{
	//Delete the vertices
	vertices.clear();
	

	//Delete lines
	lines.clear();
	
	//Delete triangles
	triangles.clear();
}

//Initialize polygon using file
void Poly::InitializePolygon(ifstream & inputfile)
{
	//Initialize vertices
	InitializeVertices(inputfile);

	//Initialize lines
	InitializeTriangles(inputfile);
}

//Translate all vertices according to translation vector (translatex, translatey)
void Poly::Translate(glm::vec3 translate)
{
	//Loop through every vertex and translate
	for (int i = 0; i < numVertices; i++)
	{
		vertices[i].Translate(translate);
	}
}

//Scale each vertex with respect to centroid
void Poly::Scale(float scalex, float scaley, float scalez)
{
	//Calculate centroid
	CalculateCentroid();

	for (int i = 0; i < numVertices; i++)
	{
		vertices[i].Scale(scalex, scaley, scalez, centroid);
	}
}

//Rotate polygon around axis defined by points p1 and p2
//by the rotation angle
void Poly::Rotate(float rotationangle, glm::vec3 p1, glm::vec3 p2)
{
	//Rotate each vertex
	for (int i = 0; i < numVertices; i++)
	{
		vertices[i].Rotate(rotationangle, p1, p2);
	}
}

//Get the number of vertices
int Poly::getNumVertices()
{
	return numVertices;
}

//Set pixel (x, y) to given color
void Poly::setPixel(float * buffer, int x, int y, int height, glm::vec3 color)
{
	buffer[3 * (x + y * height)] = color[0]; //R
	buffer[3 * (x + y * height) + 1] = color[1]; //G 
	buffer[3 * (x + y * height) + 2] = color[2]; //B
}

//Calculate the centroid
void Poly::CalculateCentroid()
{
	float totalx = 0;
	float totaly = 0;
	float totalz = 0;

	for (int i = 0; i < numVertices; i++)
	{
		totalx += vertices[i].getX();
		totaly += vertices[i].getY();
		totalz += vertices[i].getZ();
	}

	centroidx = totalx / numVertices;
	centroidy = totaly / numVertices;
	centroidz = totalz / numVertices;

	//Vectorize centroid
	centroid[0] = centroidx;
	centroid[1] = centroidy;
	centroid[2] = centroidz;
}

//Update the number of vertices in polygon
void Poly::UpdateNumVertices()
{
	numVertices = vertices.size();
}

//Give vertex data to triangles
void Poly::verticesToTriangles()
{
	//For each triangle, have it save the data for all its vertices
	for (int i = 0; i < numTriangles; i++)
	{
		triangles[i].setVertices(vertices);
	}
}

//Project each vertex into xy, xz or yz planes
void Poly::Project(int projectionplane)
{
	for (int i = 0; i < numVertices; i++)
	{
		vertices[i].Project(projectionplane);
	}
}












//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~UNUSED FUNCTIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//Place vertices in pixel buffer
void Poly::verticesToBuffer(float *buffer, int yResolution)
{
	//Loop through all vertices
	for (int i = 0; i < numVertices; i++)
	{
		//Find x and y coordinates of vertex in pixel plane
		int x = vertices[i].getXPixel();
		int y = vertices[i].getYPixel();
		glm::vec3 color = vertices[i].getColor();
		//Set corresponding element of pixelBuffer to 1:
		setPixel(buffer, x, y, yResolution, color);

	}

}

//Draw lines using the DDA algorithm
void Poly::DrawLinesDDA(float *buffer, int yresolution)
{
	//For each line, draw using DDA algorithm
	for (int i = 0; i < numTriangles; i++)
	{
		triangles[i].DrawLinesDDA(buffer, yresolution, vertices);
	}
}

//Set the color of each vertex using the phong model (distance is not a constant here)
void Poly::setVertexColors(glm::vec3 source, glm::vec3 from, glm::vec3 amb, glm::vec3 spec, glm::vec3 diff,
	float Iamb, float Ilight, int phongCons)
{
	//Set the color of each vertex
	for (int i = 0; i < numVertices; i++)
	{
		vertices[i].setColor(amb, diff, spec, from, source, Iamb, Ilight, phongCons);
	}
}