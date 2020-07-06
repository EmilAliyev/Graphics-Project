#include "Line.h"



Line::Line()
{
}

//Constructor
Line::Line(int v1in, int v2in)
{
	v1index = v1in;
	v2index = v2in;
}


Line::~Line()
{
}

//Initialize the line with data from input file
void Line::InitializeLine(ifstream & inputfile, vector<Vertex> vertices)
{
	//Next 2 values in file are vertices to be connected
	inputfile >> v1index;
	inputfile >> v2index;

	v1index--;
	v2index--;
}

//Write vertex numbers to output file
void Line::WriteToOutput(ofstream & outputfile)
{
	outputfile << v1index << ' ' << v2index << endl;
}

//Find where line going through (x1, y1) and (x2, y2) intersects vertical line x = xvertical
glm::vec2 Line::verticalintersection(float x1, float x2, float y1, float y2, float xvertical)
{

	//Find the slope
	float slope = (y2 - y1) / (x2 - x1);

	//Find y intercept b = y - mx
	float b = y2 - slope * x2;

	//Find y coordinate at which line y = mx + b intersects xvertical
	float y = slope * xvertical + b;

	return glm::vec2(xvertical, y);
}

//Find where line going through (x1, y1) and (x2, y2) intersects horizontal line y = yhorizontal
glm::vec2 Line::horizontalintersection(float x1, float x2, float y1, float y2, float yhorizontal)
{
	//If line is vertical, the intersection is (x, yhorizontal) where x is the coordinate of the vertical line
	if (x2 == x1)
		return glm::vec2(x2, yhorizontal);

	//Find the slope
	float slope = (y2 - y1) / (x2 - x1);

	//Find y intercept b = y - mx
	float b = y2 - slope * x2;

	//Find x coordinate at which line y=mx+b -> x = (y-b)/m intersects yhorizontal
	float x = (yhorizontal - b) / slope;

	return glm::vec2(x, yhorizontal);
}

//Set pixel (x, y) in buffer to white
void Line::setPixel(float * buffer, int x, int y, int height)
{
	buffer[3 * (x + y * height)] = 1; //R
	buffer[3 * (x + y * height) + 1] = 1; //G
	buffer[3 * (x + y * height) + 2] = 1; //B
}

//Set pixel (x, y) in buffer to given color
void Line::setPixelColor(float * buffer, int x, int y, int height, glm::vec3 color)
{
	buffer[3 * (x + y * height)] = color[0]; //R
	buffer[3 * (x + y * height) + 1] = color[1]; //G
	buffer[3 * (x + y * height) + 2] = color[2]; //B
}

//Draw line using DDA algorithm (triangle-by-triangle drawing)
void Line::DrawLineDDATri(float * buffer, int bufferheight)
{
	Vertex vtx1;
	Vertex vtx2;
	int startx;
	int endx;
	int starty;
	int endy;

	//Determine leftmost and rightmost vertex
	//Leftmost is vertex1, rightmost is vertex 2
	if (vertex1.getXPixel() > vertex2.getXPixel())
	{
		vtx1 = vertex2;
		vtx2 = vertex1;
	}
	else
	{
		vtx1 = vertex1;
		vtx2 = vertex2;
	}

	//Determine x and y values values to start and end line drawing 
	startx = vtx1.getXPixel(); //x1
	endx = vtx2.getXPixel();	//x2
	starty = vtx1.getYPixel(); //y1
	endy = vtx2.getYPixel();	//y2

	//Special case: If x values are the same, color in all y values in between (draw vertical line) and return to avoid dividing by 0
	if (startx == endx)
	{
		ExtremeSlope(startx, endx, starty, endy, buffer, bufferheight);
		return;
	}


	//Determine slope and y-intercept

	//m = (y2-y1)/(x2-x1)
	//b = y2-mx2
	float slope = (float)(endy - starty) / (float)(endx - startx);
	float yintercept = (float)endy - (slope * endx);



	//Case 1:  -1 < m < 1: increment x and find best y
	if ((slope <= 1) && (slope >= -1))
	{

		//Determine best y value for each x between the two vertices
		for (int x = startx + 1; x < endx; x++)
		{
			//Use y = mx + b to find y as a float
			float yf = slope * x + yintercept;

			//Round and convert to int
			int y = (int)round(yf);

			//Color in pixel (x, y)
			glm::vec3 color = GouraudShading(x, y); //Find the color
			setPixelColor(buffer, x, y, bufferheight, color); //Set the color
		}

	}

	//Case 2: m > 1: increment y and find best x
	if (slope > 1)
	{
		//Determine best x value for each y between the two vertices
		for (int y = starty+1; y < endy; y++)
		{
			//Use x = (y-b)/m to find x as a float
			float xf = (y - yintercept) / slope;

			//Round and convert to int
			int x = (int)round(xf);

			//Color in pixel (x, y)
			glm::vec3 color = GouraudShading(x, y); //Find the color
			setPixelColor(buffer, x, y, bufferheight, color); //Set the color
		}
	}

	//Case 3: m < -1: decrement y to find best x
	else
	{
		//Determine best x value for each y between the two vertices
		for (int y = starty-1; y > endy; y--)
		{
			//Use x = (y-b)/m to find x as a float
			float xf = (y - yintercept) / slope;

			//Round and convert to int
			int x = (int)round(xf);

			//Color in pixel (x, y)
			glm::vec3 color = GouraudShading(x, y); //Find the color
			setPixelColor(buffer, x, y, bufferheight, color); //Set the color
		}
	}
}

//Handle vertical lines
void Line::ExtremeSlope(int startx, int endx, int starty, int endy, float *buffer, int yresolution)
{
	//Go up if endy > starty, down if endy < starty
	if (endy > starty)
	{
		for (int j = (starty + 1); j < endy; j++)
		{
			//Color in pixel (x, y)
			glm::vec3 color = GouraudShading(startx, j); //Find the color
			setPixelColor(buffer, startx, j, yresolution, color); //Set the color
		}
	}
	else if (endy < starty)
	{

		for (int k = (starty - 1); k > endy; k--)
		{
			//Color in pixel (x, y)
			glm::vec3 color = GouraudShading(startx, k); //Find the color
			setPixelColor(buffer, startx, k, yresolution, color); //Set the color
		}
	}
}

//Return gouraud shading color value at point (x, y) on line
glm::vec3 Line::GouraudShading(int x, int y)
{
	//Get coordinates of vertices v1 and v2
	Vertex v1 = vertex1;
	Vertex v2 = vertex2;

	glm::vec2 pv1(v1.getXPixel(), v1.getYPixel());
	glm::vec2 pv2(v2.getXPixel(), v2.getYPixel());

	//Vector of current point (x, y)
	glm::vec2 p(x, y);

	float length = glm::length(pv2 - pv1);

	//I(p) = |p-v2|/length * I(v1) + |p-v1|/length * I(v2)
	glm::vec3 color = glm::length(p - pv2) / length * v1.getColor() + glm::length(p - pv1) / length * v2.getColor();

	return color;
}

//Get vertex data from polygon's vertex array
void Line::setVertices(vector<Vertex> vertices)
{
	vertex1 = vertices[v1index];
	vertex2 = vertices[v2index];
}











//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~UNUSED FUNCTIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~






//Draw line from v1 to v2 with DDA algorithm
void Line::DrawLineDDA(float * buffer, int yresolution, vector<Vertex> vertices)
{
	Vertex vertex1;
	Vertex vertex2;
	int startx;
	int endx;
	int starty;
	int endy;

	//Determine leftmost and rightmost vertex
	//Leftmost is vertex1, rightmost is vertex 2
	if (vertices[v1index].getXPixel() > vertices[v2index].getXPixel())
	{
		vertex1 = vertices[v2index];
		vertex2 = vertices[v1index];
	}
	else
	{
		vertex1 = vertices[v1index];
		vertex2 = vertices[v2index];
	}

	//Determine x and y values values to start and end line drawing 
	startx = vertex1.getXPixel(); //x1
	endx = vertex2.getXPixel();	//x2
	starty = vertex1.getYPixel(); //y1
	endy = vertex2.getYPixel();	//y2

								//Special case: If x values are the same, color in all y values in between (draw vertical line) and return to avoid dividing by 0
	if (startx == endx)
	{
		ExtremeSlope(startx, endx, starty, endy, buffer, yresolution, vertices);
		return;
	}


	//Determine slope and y-intercept

	//m = (y2-y1)/(x2-x1)
	//b = y2-mx2
	float slope = (float)(endy - starty) / (float)(endx - startx);
	float yintercept = (float)endy - (slope * endx);



	//Case 1:  -1 < m < 1: increment x and find best y
	if ((slope <= 1) && (slope >= -1))
	{

		//Determine best y value for each x between the two vertices
		for (int x = startx + 1; x < endx; x++)
		{
			//Use y = mx + b to find y as a float
			float yf = slope * x + yintercept;

			//Round and convert to int
			int y = (int)round(yf);

			//Color in pixel (x, y)
			glm::vec3 color = GouraudShading(x, y, vertices); //Find the color
			setPixelColor(buffer, x, y, yresolution, color); //Set the color
		}

	}

	//Case 2: m > 1: increment y and find best x
	if (slope > 1)
	{
		//Determine best x value for each y between the two vertices
		for (int y = starty + 1; y < endy; y++)
		{
			//Use x = (y-b)/m to find x as a float
			float xf = (y - yintercept) / slope;

			//Round and convert to int
			int x = (int)round(xf);

			//Color in pixel (x, y)
			glm::vec3 color = GouraudShading(x, y, vertices); //Find the color
			setPixelColor(buffer, x, y, yresolution, color); //Set the color
		}
	}

	//Case 3: m < -1: decrement y to find best x
	else
	{
		//Determine best x value for each y between the two vertices
		for (int y = starty - 1; y > endy; y--)
		{
			//Use x = (y-b)/m to find x as a float
			float xf = (y - yintercept) / slope;

			//Round and convert to int
			int x = (int)round(xf);

			//Color in pixel (x, y)
			glm::vec3 color = GouraudShading(x, y, vertices); //Find the color
			setPixelColor(buffer, x, y, yresolution, color); //Set the color
		}
	}
}

//Handle vertical lines
void Line::ExtremeSlope(int startx, int endx, int starty, int endy, float *buffer, int yresolution, vector<Vertex> vertices)
{
	//Go up if endy > starty, down if endy < starty
	if (endy > starty)
	{
		for (int j = (starty + 1); j < endy; j++)
		{
			//Color in pixel (x, y)
			glm::vec3 color = GouraudShading(startx, j, vertices); //Find the color
			setPixelColor(buffer, startx, j, yresolution, color); //Set the color
		}
	}
	else if (endy < starty)
	{

		for (int k = (starty - 1); k > endy; k--)
		{
			//Color in pixel (x, y)
			glm::vec3 color = GouraudShading(startx, k, vertices); //Find the color
			setPixelColor(buffer, startx, k, yresolution, color); //Set the color
		}
	}
}

//Return gouraud shading color value at point (x, y) on line
glm::vec3 Line::GouraudShading(int x, int y, vector<Vertex> vertices)
{
	//Get coordinates of vertices v1 and v2
	Vertex v1 = vertices[v1index];
	Vertex v2 = vertices[v2index];

	glm::vec2 pv1(v1.getXPixel(), v1.getYPixel());
	glm::vec2 pv2(v2.getXPixel(), v2.getYPixel());

	//Vector of current point (x, y)
	glm::vec2 p(x, y);

	float length = glm::length(pv2 - pv1);

	//I(p) = |p-v2|/length * I(v1) + |p-v1|/length * I(v2)
	glm::vec3 color = glm::length(p - pv2) / length * v1.getColor() + glm::length(p - pv1) / length * v2.getColor();

	return color;
}
