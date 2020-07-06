#include "PixelPlane.h"

//Default constructor
PixelPlane::PixelPlane()
{
}

//Constructor
PixelPlane::PixelPlane(Poly *polygonarrayinput, int xresolutioninput, int yresolutioninput, int numPolygonsinput, int algo, int projplane)
{
	polygonarray = polygonarrayinput;
	numPolygons = numPolygonsinput;
	xresolution = xresolutioninput;
	yresolution = yresolutioninput;
	pixelBuffer = new float[xresolution * yresolution * 3];
	polygonBuffer = new float[xresolution * yresolution * 3];
	algorithm = 1;
	projectionPlane = projplane;

	//Initialize Buffers
	for (int i = 0; i < xresolution * yresolution * 3; i++)
	{
		pixelBuffer[i] = 0;
		polygonBuffer[i] = 0;
	}
}

//Destructor
PixelPlane::~PixelPlane()
{
	delete[] pixelBuffer;

	

}

//Create - The "Master Function" of the PixelPlane class
void PixelPlane::Create()
{
	//Plane is discrete. Number of pixels depends on resolution.

	//To transfer from unit plane to pixel, multiply coordinate by resolution - 1


	//Convert vertices to pixel plane
	ConvertToPixel();

	//Draw the triangles
	DrawTriangles();
}

//Convert vertices to pixel plane
void PixelPlane::ConvertToPixel()
{
	//Loop through each polygon and convert each of its vertices to pixel plane
	for (int i = 0; i < numPolygons; i++)
	{
		polygonarray[i].convertToPixel(xresolution, yresolution);
	}
}

//Return the pixelBuffer
float * PixelPlane::GetPixelBuffer()
{
	return pixelBuffer;
}

//Copy polygon from polygon buffer to pixel buffer
void PixelPlane::PolyToPixelBuffer()
{
	//Copy every 1 to pixel buffer
	for (int i = 0; i < xresolution * yresolution * 3; i++)
	{

		if (polygonBuffer[i] != 0)
		{
			pixelBuffer[i] = polygonBuffer[i];
		}

	}
}

//Clear the polygon buffer
void PixelPlane::ClearPolyBuffer()
{
	//Set every element to 0
	for (int i = 0; i < xresolution * yresolution * 3; i++)
	{
		polygonBuffer[i] = 0;
	}
}

//Draw the scene, triangle by triangle
void PixelPlane::DrawTriangles()
{
	//For each polygon
	for (int i = 0; i < numPolygons; i++)
	{
		//Pass vertex data to triangles
		polygonarray[i].verticesToTriangles();

		vector<Triangle> temptrianglevector = polygonarray[i].getTriangles(); //Get all triangles of polygon i

		//Push each into triangle vector
		int size = temptrianglevector.size();
		for (int j = 0; j < size; j++)
		{
			trianglearray.push_back(temptrianglevector[j]);
		}
	}

	//Sort the triangles by depth (depth in z if xy plane, etc)
	sortTrianglesByDepth();

	//Draw each triangle (FROM BACK TO FRONT. AS THEY ARE SORTED IN ASCENDING ORDER (LOWEST DEPTH TO HIGHEST), 
	//START FROM THE END OF THE VECTOR)
	int numTriangles = trianglearray.size();
	
	for (int i = numTriangles - 1; i >= 0; i--)
	{
		DrawTriangle(trianglearray[i]);

		//Copy to the pixel buffer
		PolyToPixelBuffer();

		//Clear the buffer
		ClearPolyBuffer();
	}
}

//Draw the given triangle
void PixelPlane::DrawTriangle(Triangle triangle)
{
	//Draw vertices
	triangle.drawVertices(polygonBuffer, yresolution);
	
	//Draw lines
	triangle.drawLinesDDA(polygonBuffer, yresolution);

	//Rasterize
	triangle.RasterizeTriangle(xresolution, yresolution, polygonBuffer);
}

//Sort the triangles by depth
void PixelPlane::sortTrianglesByDepth()
{
	int numTriangles = trianglearray.size();

	//If projection plane is XY, sort by z-depth
	if (projectionPlane == PLANE_XY)
	{
		//Set the depths of the triangles to be their z-depths
		for (int i = 0; i < numTriangles; i++)
		{
			trianglearray[i].setProjDepth(Z_AXIS);
		}
	}

	//If projection plane is XZ, sort by y-depth
	else if (projectionPlane == PLANE_XZ)
	{
		//Set the depths of the triangles to be their y-depths
		for (int i = 0; i < numTriangles; i++)
		{
			trianglearray[i].setProjDepth(Y_AXIS);
		}
	}

	//If projection plane is YZ, sort by z-depth
	else
	{
		//Set the depths of the triangles to be their x-depths
		for (int i = 0; i < numTriangles; i++)
		{
			trianglearray[i].setProjDepth(X_AXIS);
		}
	}

	//Sort the triangles
	sort(trianglearray.begin(), trianglearray.end());
}

//Implement half toning on the pixel buffer
float * PixelPlane::halfTone(float * buffer, int xresolution, int yresolution)
{
	float *halftonedbuffer = new float [xresolution * yresolution * 3];

	//Initialize half toned buffer
	for (int i = 0; i < xresolution * yresolution * 3; i++)
	{
		halftonedbuffer[i] = 0;
	}

	//Divide pixel buffer into 3x3 "mega-pixels"

	//Calculate the number of megapixel rows and columns
	int numMegaPixelRows = xresolution / MEGAPIXEL_WIDTH;
	int numMegaPixelColumns = yresolution / MEGAPIXEL_HEIGHT;
	

	//For each megapixel (i, j), get average intensity and use it to set half-toned pixels
	for (int i = 0; i < numMegaPixelRows; i++)
	{
		for (int j = 0; j < numMegaPixelColumns; j++)
		{
			//get average intensity
			glm::vec3 avgcolor = getMegaPixelColor(buffer, i, j, yresolution);

			//Set the average intensity in corresponding megapixel of new pixelbuffer
			setMegaPixelColor(avgcolor, halftonedbuffer, i, j, yresolution);
		}

	}
	
	return halftonedbuffer;
}

//Get the average intensity of megapixel(x, y)
glm::vec3 PixelPlane::getMegaPixelColor(float * buffer, int x, int y, int height)
{
	glm::vec3 totalcolor(0, 0, 0);

	//Sum the intensities of all pixels in the megapixel
	for (int i = 0; i < MEGAPIXEL_WIDTH; i++)
	{
		for (int j = 0; j < MEGAPIXEL_HEIGHT; j++)
		{

			//Get the physical pixel coordinates
			glm::vec2 pixelpos = megaPixelToPixel(x, y, i, j); 
			int pixelx = (int) pixelpos[0];
			int pixely = (int) pixelpos[1];

			glm::vec3 pixelcolor = getPixelColor(buffer, pixelx, pixely, height); //Get the color of the physical pixel

			totalcolor = totalcolor + pixelcolor; //Add this color to the total
		}
	}

	//Get the average

	float numPixels = MEGAPIXEL_HEIGHT * MEGAPIXEL_WIDTH; //The number of pixels is the height * the width of the megapixel
	
	glm::vec3 averagecolor = totalcolor / numPixels;

	return averagecolor;
}

//Set the color of megapixel(x, y)
void PixelPlane::setMegaPixelColor(glm::vec3 color, float * buffer, int x, int y, int height)
{
	float Rintensity = color[0]; //R intensity
	float Gintensity = color[1]; //G intensity
	float Bintensity = color[2]; //B intensity

	int numpixels = MEGAPIXEL_HEIGHT * MEGAPIXEL_WIDTH; //Get the number of pixels

	float maxintensity = Rintensity; //Max intensity initialized to R intensity

	//Set the max intensity to the correct one
	if (Gintensity > maxintensity)
	{
		maxintensity = Gintensity;
	}

	if (Bintensity > maxintensity)
	{
		maxintensity = Bintensity;
	}

	//The number of pixels turned on is the maximum intensity * the mega pixel size rounded to the nearest whole number
	int numpixelsOn = (int)round(maxintensity * numpixels);

	//Get intensities as fraction of total
	float totalIntensity = Rintensity + Gintensity + Bintensity;

	//If the total intensity is 0, do not turn any pixels on
	if (totalIntensity != 0)
	{
		float Rfrac = Rintensity / totalIntensity;
		float Gfrac = Gintensity / totalIntensity;
		float Bfrac = Bintensity / totalIntensity;
		glm::vec3 intensityratios(Rfrac, Gfrac, Bfrac);

		//Turn pixels on
		MegaPixelOn(numpixelsOn, buffer, x, y, height, intensityratios);
	}
}

//Randomly turn on pixels in megapixel (x, y), according to numpixelson and intensity ratios
void PixelPlane::MegaPixelOn(int numpixelson, float * buffer, int x, int y, int height, glm::vec3 intensityratios)
{
	//Red, green and blue pixels
	glm::vec3 R(1, 0, 0);
	glm::vec3 G(0, 1, 0);
	glm::vec3 B(0, 0, 1);

	//RGB ratios
	float rRatio = intensityratios[0];
	float gRatio = intensityratios[1];
	float bRatio = intensityratios[2];

	//Number of R, G and B pixels to turn on
	int numRPixels = (int)round(rRatio * numpixelson);
	int numGPixels = (int)round(gRatio * numpixelson);
	int numBPixels = (int)round(bRatio * numpixelson);

	//Randomly set pixels
	int pixelsleft = numpixelson; //Number of pixels left to turn on
	
	//Set the red pixels
	while ((numRPixels > 0) && (pixelsleft > 0))
	{
		//Set a random pixel within the megapixel to be red
		MegaPixelSetRandPixel(x, y, R, buffer, height);
		numRPixels--;
		pixelsleft--;
	}
	
	//Set the green pixels
	while ((numGPixels > 0) && (pixelsleft > 0))
	{
		//Set a random pixel within the megapixel to be green
		MegaPixelSetRandPixel(x, y, G, buffer, height);
		numGPixels--;
		pixelsleft--;
	}

	//Set the blue pixels
	while ((numBPixels > 0) && (pixelsleft > 0))
	{
		//Set a random pixel within the megapixel to be blue
		MegaPixelSetRandPixel(x, y, B, buffer, height);
		numBPixels--;
		pixelsleft--;
	}
}

//Set a random pixel in megapixel (x, y)
void PixelPlane::MegaPixelSetRandPixel(int x, int y, glm::vec3 color, float *buffer, int height)
{
	bool pixelfound = false; //Whether a pixel to turn on has been found

	//Physical pixels to turn on
	int ip; 
	int jp;

	//Until a blank pixel is found
	while (!pixelfound)
	{

		//Generate a random i and j
		int i = rand() % MEGAPIXEL_WIDTH;
		int j = rand() % MEGAPIXEL_HEIGHT;

		//Get the physical pixel corresponding to (i, j) of the megapixel
		glm::vec2 p = megaPixelToPixel(x, y, i, j);

		ip = (int) p[0];
		jp = (int) p[1];

		//If the pixel generated is blank, exit the loop
		if (isZero(ip, jp, height, buffer))
		{
			pixelfound = true;
		}
	}

	//Set the pixel
	setPixel(buffer, ip, jp, height, color);
}

//Get physical pixel from (i, j)th pixel of megapixel (x, y)
glm::vec2 PixelPlane::megaPixelToPixel(int x, int y, int i, int j)
{
	//Megapixel (x, y) starts at (MP_WIDTH * x, MP_HEIGHT * y)
	glm::vec2 startpixel(MEGAPIXEL_WIDTH * x, MEGAPIXEL_HEIGHT * y);

	//Add (i, j) to get the physical pixel
	glm::vec2 pixelpos = startpixel + glm::vec2(i, j);

	return pixelpos;
}

//Get the color of pixel (x, y)
glm::vec3 PixelPlane::getPixelColor(float * buffer, int x, int y, int height)
{
	float red = buffer[3 * (x + y * height)]; //R
	float blue = buffer[3 * (x + y * height) + 1]; //G
	float green = buffer[3 * (x + y * height) + 2]; //B

	return glm::vec3(red, blue, green); //return the color
}

//Check if pixel (x, y) is zero
bool PixelPlane::isZero(int x, int y, int height, float * buffer)
{
	bool redZero = (buffer[3 * (x + y * height)] == 0); //Red is zero
	bool greenZero = (buffer[3 * (x + y * height) + 1] == 0); //Green is zero
	bool blueZero = (buffer[3 * (x + y * height) + 2] == 0); //Blue is zero

	//If all colors are zero, pixel is zero
	return (redZero && greenZero && blueZero);
}

//Set pixel (x, y) according to color
void PixelPlane::setPixel(float * buffer, int x, int y, int height, glm::vec3 color)
{
	buffer[3 * (x + y * height)] = color[0]; //R
	buffer[3 * (x + y * height) + 1] = color[1]; //G
	buffer[3 * (x + y * height) + 2] = color[2]; //B

}






//Unused functions

//Draw the lines
void PixelPlane::DrawLines(int polygonNumber)
{

	//Draw lines using DDA algorithm
	DrawLinesDDA(polygonNumber);


}

//Draw the polygons
void PixelPlane::DrawPolygons()
{

	//Draw each polygon one by one to a polygon buffer. 
	//Once it is fully drawn in polygon buffer, copy it to the pixel buffer. 
	for (int i = 0; i < numPolygons; i++)
	{
		//Place vertices in polygon buffer (vertex coordinates become 1s)
		PlaceVertiesInPolygonBuffer(i);

		//Draw lines
		DrawLines(i);

		//Copy polygon from polygon buffer to pixel buffer
		PolyToPixelBuffer();

		//Clear polygon buffer
		ClearPolyBuffer();
	}

	//Delete the polygon buffer

	delete[] polygonBuffer;
}

//Draw lines with DDA algorithm
void PixelPlane::DrawLinesDDA(int polygonNumber)
{
	//Draw lines for corresponding polygon
	polygonarray[polygonNumber].DrawLinesDDA(polygonBuffer, yresolution);
}

//Place the vertices in the polygon buffer
void PixelPlane::PlaceVertiesInPolygonBuffer(int polygonNumber)
{
	//Loop through each polygon and find x and y values for each vertex. Set these x and y values of pixel buffer to 1.
	polygonarray[polygonNumber].verticesToBuffer(polygonBuffer, yresolution);

}