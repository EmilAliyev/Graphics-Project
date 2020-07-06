#include "Triangle.h"



Triangle::Triangle()
{
}


Triangle::~Triangle()
{
}

//Initialize triangle with data from input file
void Triangle::InitializeTriangle(ifstream & inputfile, vector<Vertex> vertices)
{
	inputfile >> v1index;
	inputfile >> v2index;
	inputfile >> v3index;

	//Set vertex indices to start at 0
	v1index--;
	v2index--;
	v3index--;

	//Set lines
	CreateLines();

	//Set the depth
	calculateDepth(vertices);
	
}

void Triangle::CreateLines()
{
	Line l1(v1index, v2index);
	Line l2(v1index, v3index);
	Line l3(v2index, v3index);

	lines[0] = l1;
	lines[1] = l2;
	lines[2] = l3;
}

void Triangle::WriteToOutput(ofstream & outputfile)
{
	outputfile << v1index + 1 << ' ' << v2index + 1 << ' ' << v3index + 1 << endl;
}

//Calculate the depth of the triangle in each direction
void Triangle::calculateDepth(vector<Vertex> vertices)
{
	//New vector of vertices in the triangle
	vector<Vertex> verticesInTriangle;

	verticesInTriangle.push_back(vertices[v1index]);
	verticesInTriangle.push_back(vertices[v2index]);
	verticesInTriangle.push_back(vertices[v3index]);

	//The depth in x is the minimum x (the closest x to the eye in the yz projection)
	//The depth in y is the minimum y (the closest y to the eye in the xz projection)
	//The depth in z is the minimum z (the closest z to the eye in the xy projection)
	float minx = MinX(verticesInTriangle);
	float miny = MinY(verticesInTriangle);
	float minz = MinZ(verticesInTriangle);

	depth = glm::vec3(minx, miny, minz);
}

//The minimum x among the vertices of the triangle
float Triangle::MinX(vector<Vertex> vertices)
{
	float min = vertices[0].getX();

	//Find the minimum
	for (int i = 1; i < 3; i++)
	{
		if (min > vertices[i].getX())
		{
			min = vertices[i].getX();
		}
	}

	return min;
}

//The minimum y among the vertices of the triangle
float Triangle::MinY(vector<Vertex> vertices)
{
	float min = vertices[0].getY();

	//Find the minimum
	for (int i = 1; i < 3; i++)
	{
		if (min > vertices[i].getY())
		{
			min = vertices[i].getY();
		}
	}

	return min;
}

//The minimum z among the vertices of the triangle
float Triangle::MinZ(vector<Vertex> vertices)
{
	float min = vertices[0].getZ();

	//Find the minimum
	for (int i = 1; i < 3; i++)
	{
		if (min > vertices[i].getZ())
		{
			min = vertices[i].getZ();
		}
	}

	return min;
}

//return the normal
glm::vec3 Triangle::getNormal()
{
	return normal;
}

//Check if triangle contains vertex with index vertexindex
bool Triangle::containsVertex(int vertexindex)
{
	return ((v1index == vertexindex) || (v2index == vertexindex) || (v3index == vertexindex));
}

//Set pixel (x, y) in buffer to the given color
void Triangle::setPixel(float * buffer, int x, int y, int height, glm::vec3 color)
{
	buffer[3 * (x + y * height)] = color[0]; //R
	buffer[3 * (x + y * height) + 1] = color[1]; //G
	buffer[3 * (x + y * height) + 2] = color[2]; //B

}

//Gouraud Shading for rasterizing triangle
glm::vec3 Triangle::GouraudShading(int x, int y, Vertex v1, Vertex v2, Vertex v3)
{
	//Get coordinates of vertices v1, v2 and v3
	glm::vec2 pv1(v1.getXPixel(), v1.getYPixel());
	glm::vec2 pv2(v2.getXPixel(), v2.getYPixel());
	glm::vec2 pv3(v3.getXPixel(), v3.getYPixel());

	//Vector of current point (x, y)
	glm::vec2 p(x, y);

	//Calculate areas
	float area1 = calculateArea(p, pv2, pv3);
	float area2 = calculateArea(p, pv1, pv3);
	float area3 = calculateArea(p, pv1, pv2);
	float areatot = area1 + area2 + area3;

	//I(p) = (a1/a)*I(v1)+(a2/a)*I(v2)+(a3/a)*I(v3)
	//The larger the area, the more weight the corresponding vertex's color has
	glm::vec3 color = (area1 / areatot) * v1.getColor() + (area2 / areatot) * v2.getColor() + (area3 / areatot) * v3.getColor();

	return color;
}

//Calculate area of triangle given points p1, p2 and p3
float Triangle::calculateArea(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
{
	//Use heron's formula: Area = sqrt(s(s-a)(s-b)(s-c))
	//Where s is half the perimeter and a, b and c are the lengths of the sides

	float a = glm::length(p1 - p2);
	float b = glm::length(p1 - p3);
	float c = glm::length(p2 - p3);
	float s = (a + b + c) / 2;
	
	float area = sqrt(s*(s-a)*(s-b)*(s-c));

	return area;

}


//Rasterize the polygon
void Triangle::Rasterize(int xresolution, int yresolution, float *polygonBuffer)
{
	//DO NOT RASTERIZE IF AREA = 0. THAT MEANS THAT IN THIS PROJECTION, THE TRIANGLE ONLY APPEARS AS A SINGLE LINE
	
	//Vectorize vertex coordinates
	glm::vec2 p1(v1.getXPixel(), v1.getYPixel());
	glm::vec2 p2(v2.getXPixel(), v2.getYPixel());
	glm::vec2 p3(v3.getXPixel(), v3.getYPixel());

	//Return if the area is zero
	if (calculateArea(p1, p2, p3) == 0)
	{

		return;
	}

	//Go through each row 
	for (int y = 1; y < (yresolution - 1); y++)
	{
		//If a lone horizontal line exists on current row, move on to next row.
		if (loneHorizontalLine(y, xresolution, yresolution, polygonBuffer))
		{
			continue;
		}


		bool in = false; //When in, shade pixel. When out, do nothing. 
		
		//When line detected (pixel is not zero), flip in/out if not exception. If in, shade the pixel
		for (int x = 0; x < xresolution; x++)
		{
			if ((!isZero(x, y, yresolution, polygonBuffer)) && !RasterizeException(x, y, xresolution, yresolution, polygonBuffer))
			{
				in = !in;

			}

			if (in)
			{
				shadePixel(x, y, yresolution, polygonBuffer, v1, v2, v3);

			}
		}
		in = false;
	}


}

//Detect Rasterization exceptions (vertex or horizontal line)
//Treat pixel as an extrema if a pixel to the left or right is an extrema
bool Triangle::RasterizeException(int x, int y, int xresolution, int yresolution, float *polygonBuffer)
{
	bool extrema = RastVertex(x, y, yresolution, polygonBuffer);
	bool horizontalline = RastHorizontalLine(x, y, xresolution, yresolution, polygonBuffer);

	return extrema || horizontalline;
}

//Detect horizontal line during rasterization
bool Triangle::RastHorizontalLine(int x, int y, int xresolution, int yresolution, float *polygonBuffer)
{
	//Return if at right edge
	if (x == (xresolution - 1))
	{
		return false;
	}

	//Horizontal line = pixel value nonzero followed by another nonzero
	return (!isZero(x, y, yresolution, polygonBuffer)) && (!isZero(x+1, y, yresolution, polygonBuffer));
}

//Detect vertex during rasterization
bool Triangle::RastVertex(int x, int y, int yresolution, float *polygonBuffer)
{
	//Vertex = y-extrema

	if (!isZero(x, y, yresolution, polygonBuffer))
	{

		//If a pixel with value nonzero has no adjacent pixels above it with value nonzero, but does have
		//pixels below it, it is a local maximum
		if ((isZero(x - 1, y + 1, yresolution, polygonBuffer)) &&
			(isZero(x, y + 1, yresolution, polygonBuffer)) &&
			(isZero(x + 1, y + 1, yresolution, polygonBuffer)))
		{
			return true;
		}

		//If a pixel with value nonzero has no adjacent pixels below it with value nonzero, but does have adjacent
		//pixels above it, it is a local minimum
		if ((isZero(x - 1, y - 1, yresolution, polygonBuffer)) &&
			(isZero(x, y - 1, yresolution, polygonBuffer)) &&
			(isZero(x + 1, y - 1, yresolution, polygonBuffer)))
		{
			return true;
		}

	}
	return false;
}

//Check if lone horizontal line exists on row y
bool Triangle::loneHorizontalLine(int y, int xresolution, int yresolution, float *polygonBuffer)
{
	//Go through row until horizontal line found
	for (int x = 0; x < xresolution; x++)
	{
		//If horizontal line (1 followed by 1) found, move to the end of it
		if ((!isZero(x, y, yresolution, polygonBuffer)) && (!isZero(x+1, y, yresolution, polygonBuffer)))
		{
			while (!isZero(x, y, yresolution, polygonBuffer))
			{
				x++;
			}

			//If another 1 pixel is found on the row, the horizontal line is not alone, therefore return false.
			for (x = x; x < xresolution; x++)
			{
				if (!isZero(x, y, yresolution, polygonBuffer))
				{
					return false;
				}
			}

			//Otherwise, return true as the horizontal line is the only one in the row.
			return true;

		}
	}

	return false;
}

//Draw the vertices to the buffer
void Triangle::drawVertices(float * buffer, int bufferheight)
{
	//Draw v1
	setPixel(buffer, v1.getXPixel(), v1.getYPixel(), bufferheight, v1.getColor());

	//Draw v2
	setPixel(buffer, v2.getXPixel(), v2.getYPixel(), bufferheight, v2.getColor());

	//Draw v3
	setPixel(buffer, v3.getXPixel(), v3.getYPixel(), bufferheight, v3.getColor());
}

//Draw each line (triangle-by-triangle rendering)
void Triangle::drawLinesDDA(float * buffer, int bufferHeight)
{
	for (int i = 0; i < 3; i++)
	{
		lines[i].DrawLineDDATri(buffer, bufferHeight);
	}
}

//Check if pixel (x, y) is zero
bool Triangle::isZero(int x, int y, int height, float * buffer)
{
	bool redZero = (buffer[3 * (x + y * height)] == 0); //Red is zero
	bool greenZero = (buffer[3 * (x + y * height) + 1] == 0); //Green is zero
	bool blueZero = (buffer[3 * (x + y * height) + 2] == 0); //Blue is zero

	//If all colors are zero, pixel is zero
	return (redZero && greenZero && blueZero);
}

//Shade pixel (x, y) with Gouraud shading
void Triangle::shadePixel(int x, int y, int height, float * buffer, Vertex v1, Vertex v2, Vertex v3)
{
	glm::vec3 color = GouraudShading(x, y, v1, v2, v3);
	setPixel(buffer, x, y, height, color);
}

//Get vertex data from polygon, and pass to lines
void Triangle::setVertices(vector<Vertex> vertices)
{
	v1 = vertices[v1index];
	v2 = vertices[v2index];
	v3 = vertices[v3index];

	verticesToLines(vertices);
}

//Pass vertex data to lines
void Triangle::verticesToLines(vector<Vertex> vertices)
{
	for (int i = 0; i < 3; i++)
	{
		lines[i].setVertices(vertices);
	}
}

//Set the depth to be either the x-, y- or z- depth, depending on the given direction
void Triangle::setProjDepth(int depthaxis)
{
	//Depth is in x-direction
	if (depthaxis == DEPTH_X)
	{
		projdepth = depth[0];
	}

	//Depth is in y-direction
	else if (depthaxis == DEPTH_Y)
	{
		projdepth = depth[1];
	}

	//Depth is in z-direction
	else
	{
		projdepth = depth[2];
	}
}

//Rasterize the triangle
void Triangle::RasterizeTriangle(int xresolution, int yresolution, float * buffer)
{
	//For each y
	for (int y = 0; y < yresolution; y++)
	{
		//If 0 or 1 pixels are turned on, continue to next y
		if (numPixelsInRow(buffer, y, xresolution, yresolution) < 2)
		{
			continue;
		}

		//Find the leftmost (minimum x) pixel that is on
		int leftmostx = leftMostX(buffer, y, xresolution, yresolution);

		//Find the rightmost (maximum x) pixel that is on
		int rightmostx = rightMostX(buffer, y, xresolution, yresolution);

		//Shade empty pixels from from leftmost to rightmost pixel
		for (int x = leftmostx + 1; x < rightmostx; x++)
		{
			if (isZero(x, y, yresolution, buffer))
			{
				shadePixel(x, y, yresolution, buffer, v1, v2, v3);
			}
		}
	}

}

//Count the number of nonzero pixels in row y
int Triangle::numPixelsInRow(float * buffer, int y, int xresolution, int yresolution)
{
	int numPixels = 0; //Start at 0

	//Go through row
	for (int x = 0; x < xresolution; x++)
	{
		//If a nonzero pixel is found, increment the pixel count
		if (!isZero(x, y, yresolution, buffer))
		{
			numPixels++;
		}
	}

	//Return the number of pixels
	return numPixels;
}

//Find x value of leftmost nonzero pixel in row y
int Triangle::leftMostX(float * buffer, int y, int xresolution, int yresolution)
{
	int lmx = 0; //The leftmost x

	//Go through the row, starting from the left
	for (int x = 0; x < xresolution; x++)
	{
		//When a nonzero pixel is found, its x value is the leftmost x
		if (!isZero(x, y, yresolution, buffer))
		{
			lmx = x;
			break;
		}
	}

	return lmx;
}

//Find x value of rightmost nonzero pixel in row y
int Triangle::rightMostX(float * buffer, int y, int xresolution, int yresolution)
{
	int rmx = 0; //The leftmost x

	//Go through the row, starting from the right
	for (int x = xresolution - 1; x >= 0; x--)
	{
		//When a nonzero pixel is found, its x value is the leftmost x
		if (!isZero(x, y, yresolution, buffer))
		{
			rmx = x;
			break;
		}
	}

	return rmx;
}

//t1 < t2 if t1 is closer to eye than t2
bool operator<(Triangle t1, Triangle t2)
{
	return (t1.projdepth) < (t2.projdepth);
}











//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~UNUSED FUNCTIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//Calculate the normal of the triangle at vertex with index given by vertexnum
void Triangle::calculateNormal(vector<Vertex> vertices, int vertexnum)
{
	//Vectorize vertex coordinates

	//v1 is vertex with index given by vertexnum
	glm::vec3 v1(vertices[vertexnum].getX(), vertices[vertexnum].getY(), vertices[vertexnum].getZ());

	//v2 and v3 are the other 2 vertices
	int v2in;
	int v3in;

	//If vertexnum is v1, the other two vertices are v2 and v3
	//Therefore normal will be (v1-v2) x (v1-v3)
	if (vertexnum == v1index)
	{
		v2in = v2index;
		v3in = v3index;
	}

	//If vertexnum is v2, other two vertices are v1 and v3
	else if (vertexnum == v2index)
	{
		v2in = v1index;
		v3in = v3index;
	}

	//If vertexnum is v3, other two vertices are v1 and v2
	else if (vertexnum == v3index)
	{
		v2in = v1index;
		v3in = v2index;
	}

	//Vectorize two vertices not given
	glm::vec3 v2(vertices[v2in].getX(), vertices[v2in].getY(), vertices[v2in].getZ());
	glm::vec3 v3(vertices[v3in].getX(), vertices[v3in].getY(), vertices[v3in].getZ());

	//n = (v1 - v2) x (v1 - v3)
	//Where v1 is given vertex and v2 and v3 are other two vertices
	glm::vec3 normal = glm::cross((v1 - v2), (v1 - v3));

	//Normalize
	normal = normal / glm::length(normal);
}

//Draw lines using the DDA algorithm
void Triangle::DrawLinesDDA(float *buffer, int yresolution, vector<Vertex> vertices)
{
	//For each line, draw using DDA algorithm
	for (int i = 0; i < 3; i++)
	{
		lines[i].DrawLineDDA(buffer, yresolution, vertices);
	}
}