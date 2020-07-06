#include "Vertex.h"



Vertex::Vertex()
{
}

//Create new vertex according to coordinates
Vertex::Vertex(glm::vec3 coordinates)
{
	world = coordinates;
}


Vertex::~Vertex()
{
}

//Get the x value of coordinate in the world plane
float Vertex::getX()
{
	return world[0];
}

//Get the Y value of coordinate in the world plane
float Vertex::getY()
{
	return world[1];
}

//Get the z value of coordinate on world plane
float Vertex::getZ()
{
	return world[2];
}

//Get the x pixel coordinate
int Vertex::getXPixel()
{
	return xPixel;
}

//Get the y pixel coordinate
int Vertex::getYPixel()
{
	return yPixel;
}

//Use coordinate values in input file to set initial coordinates in world plane
void Vertex::setInitialCoordinates(ifstream& inputfile)
{
	//Coordinates y, x and z respectively are next 3 floats in input file
	inputfile >> world[0];
	inputfile >> world[1];
	inputfile >> world[2];

	//Set the normal. The next 3 floats in the input file are the normal vector elements
	inputfile >> normal[0];
	inputfile >> normal[1];
	inputfile >> normal[2];

	//Turn the normal into a unit normal
	normal = normal / glm::length(normal);
}

//Set the color using the phong model
void Vertex::setColor(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, glm::vec3 from, glm::vec3 source, float Ia, float Il,
	int n, float distance)
{
	//I(p) = ka * Ia + (Il/(|f-p|+C)) * (ka*cos(alpha) + ks * (cos(beta))^n)
	//Where:

	//The vector I(p) is the color at point p

	//ka, kd and ks are the ambient, diffuse and specular color coefficients

	//Ia is the ambient light intensity and Il is the intensity of the source

	//f is the from point (where the eye is)
	glm::vec3 f = from;

	//p is the point where the vertex is
	glm::vec3 p = world;

	//C is the distance of the light source
	float c = distance;

	//alpha is the angle between n and l
	//n is the normal 
	//l = (x-p)/|x-p| is the light vector (x is the position of the light source)
	//Therefore cos(alpha) is n . l
	glm::vec3 light = (source - p) / glm::length(source - p);
	float ca = glm::dot(normal, light);


	//Beta is the angle between r and v
	//v is the view vector, (f-p)/|f-p|
	//r = -l + 2 (n . l) * n is the reflection vector
	//Therefore, cos(beta) = r.v
	glm::vec3 view = (f - p) / glm::length(f - p);
	glm::vec3 refl = -light + 2 * glm::dot(normal, light) * normal;
	float cb = glm::dot(refl, view);

	//n is the phong constant

	//Exceptional cases:

	//If light source and from point are on different sides, the intensity is the ambient intensity
	if (((glm::dot(normal, light) > 0) && (glm::dot(normal, view) > 0)) ||
		((glm::dot(normal, light) < 0) && (glm::dot(normal, view) < 0)))
	{
		color = ka * Ia;
	}

	//Highlights aren't visible: no specular light
	else if (glm::dot(refl, view) < 0)
	{
		color = ka * Ia + (Il / (glm::length(f - p) + c)*(kd * ca));
	}

	//Normal cases
	else
	{
		//I(p) = ka * Ia + (Il/(|f-p|+C)) * (ka*cos(alpha) + ks * (cos(beta))^n)
		color = ka * Ia + (Il / (glm::length(f - p) + c)*(kd * ca + ks * pow(cb, n)));
	}

	//Normalize the color if any intensity is greater than 1
	normalizeColor();

}

glm::vec3 Vertex::getColor()
{
	return color;
}

//Calculate the normal of the vertex given the normal of every triangle that contains it (not used)
void Vertex::calculateNormal(vector<glm::vec3> trianglenormals)
{
}

//Normalize the color if any of the component intensities are greater than 1
void Vertex::normalizeColor()
{
	if ((color[0] > 1) || (color[1] > 1) || (color[2] > 1))
	{
		float maxintensity = maxRGB();

		color = color / maxintensity;
	}
}

//Get the maximum intensity out of R, G and B
float Vertex::maxRGB()
{
	float maxintensity = 0;

	//Find the maximum intensity out of the 3 component colors: R, G, B
	for (int i = 0; i < 3; i++)
	{
		if (color[i] > maxintensity)
		{
			maxintensity = color[i];
		}
	}

	return maxintensity;
}

//Shift vertex by shift vector
void Vertex::Shift(glm::vec3 shift)
{
	world = world + shift;
}

//Convert coordinates to NDC plane
void Vertex::convertToNDC(float scaleFactor, float xmin, float ymin, float zmin)
{
	//xNDC = (xworld - xmin)/scale
	NDC[0] = (world[0] - xmin)/ scaleFactor;

	//yNDC = (yworld - ymin)/scale
	NDC[1] = (world[1] - ymin) / scaleFactor;

	//zNDC = (zworld - zmin)/scale
	NDC[2] = (world[2] - zmin) / scaleFactor;

}

//Convert NDC coordinates to pixel plane
void Vertex::convertToPixel(int xresolution, int yresolution)
{
	xPixel = (int) round((proj[0] * xresolution));
	yPixel = (int) round((proj[1] * yresolution));

}

//Orthographic projection of vertex into XY, YZ or XZ planes (axonometric projections)
void Vertex::Project(int plane)
{

	//if xy, just set x and y values of projection to x and y values of NDC
	if (plane == XY)
	{
		proj[0] = NDC[0];
		proj[1] = NDC[1];


	}

	//If xz, x -> x, z -> y

	else if (plane == XZ)
	{
		proj[0] = NDC[0];
		proj[1] = NDC[2];

	}
	
	//If yz, y -> x, z -> y
	else if (plane == YZ)
	{
		proj[0] = NDC[1];
		proj[1] = NDC[2];

	}

}

//Translate the vertex according to translation vector (translatex, translatey, translatez)
void Vertex::Translate(glm::vec3 translate)
{

	//p = p + t
	world = world + translate;

	//Translate the normal
	normal = normal + translate;
}

//Scale vertex with respect to centroid
void Vertex::Scale(float scalex, float scaley, float scalez, glm::vec3 center)
{

	//p = p - c (so origin becomes the center)
	world = world - center;

	//Do the same to normal
	normal = normal - center;

	//Create scaling matrix S = (+/-scalex, 0, 0; 0, +/-scaley, 0; 0, 0, scalez)
		//Sign of scalex and scaley is the same as sign of x and y elements of p
		//This is because you want to double the distance from the center
		//So if coordinate is negative with respect to center, it becomes more negative
	glm::mat3 SCL(scalex, 0, 0, 
					0, scaley, 0,
					0, 0, scalez);

	//p = Sp + c
	world = SCL * world + center;

	//Do the same to normal
	normal = SCL * normal + center;

}

//Rotate about axis going through p1 and p2 by rotation angle
void Vertex::Rotate(float rotationangle, glm::vec3 p1, glm::vec3 p2)
{

	//Get direction vector of axis
	glm::vec3 axis = p2 - p1;

	//Get unit direction vector of axis
	glm::vec3 axisu = axis / glm::distance(p1, p2);

	//Step 1: Move axis of rotation to origin (translate by -p1) 
	world = world - p1;

	//Do the same to normal
	normal = normal - p1;

	//Step 2: Rotate axis of rotation to z-axis
	

	//Step 2.1: Rotate axis of rotation to xz plane
	glm::mat3 M21 = RotateToXZ(axisu);
	world = M21 * world;
	axisu = M21 * axisu;

	//Do the same to normal
	normal = M21 * normal;
	
	//Step 2.2: Rotate axis of rotation about y-axis onto z-axis
	glm::mat3 M22 = RotateToZ(axisu);

	world = M22 * world;
	axisu = M22 * axisu;

	//Do the same to normal
	normal = M22 * normal;

	//Step 3: Rotate about Z-axis by rotationangle

	//Calculate rotation matrix
	glm::mat3 rotmatrix(cos(rotationangle), -sin(rotationangle), 0,
						sin(rotationangle), cos(rotationangle), 0,
						0, 0, 1);

	//Apply rotation matrix
	world = rotmatrix * world;

	//Do the same to normal
	normal = rotmatrix * normal;

	//Step 4: Inverse rotation of axis from step 2

	//Step 4.1: Inverse rotation to z-axis
	glm::mat3 M41 = glm::inverse(M22);
	world = M41 * world;
	axisu = M41 * axisu;

	//Do the same to normal
	normal = M41 * normal;

	//Step 4.2: Inverse rotation to XZ plane (rotate back to original orientation)
	glm::mat3 M42 = glm::inverse(M21);
	world = M42 * world;
	axisu = M42 * axisu;

	//Do the same to normal
	normal = M42 * normal;

	//Step 5: Move axis of rotation back to original position (translate by p1)
	world = world + p1;

	//Do the same to normal
	normal = normal + p1;
}

//Return matrix to rotate rotation axis to XZ-plane given direction vector for rotation axis
glm::mat3 Vertex::RotateToXZ(glm::vec3 rot)
{
	//Get projection in zy plane of rot
	glm::vec3 rotzy(0, rot[1], rot[2]);

	//Get y value, z value and length of the projection
	float projy = rotzy[1];
	float projz = rotzy[2];
	float length = sqrt(pow(projy, 2) + pow(projz, 2));

	//Rotate about x-axis by angle beta: angle between projection and z-axis

	//Cos(beta) = projz/length
	float cb = projz / length;

	//Sin(beta) = projy / length
	float sb = projy / length;

	//Rotation matrix by beta around x-axis
	glm::mat3 Rot(1, 0, 0,
					0, cb, -sb,
					0, sb, cb);

	return Rot;
}

//Return matrix to rotate from XZ-plane about y-axis to Z-axis
glm::mat3 Vertex::RotateToZ(glm::vec3 rot)
{
	//Rotate around y-axis by negative of angle delta between vector and z-axis
	//Negative because we are rotating clockwise

	//Get x and z values
	float x = rot[0];
	float z = rot[2];

	//cos(-delta) = cos(delta) = z/length = z
	//sin(-delta) = -sin(delta) = -x/length = -x

	//Matrix to rotate around y by -delta:
	glm::mat3 RotY(z, 0, -x,
					0, 1, 0,
					x, 0, z);

	return RotY;
}

//Write vertex coordinates (in world plane) to output file
void Vertex::WriteToOutput(ofstream & outputfile)
{
	outputfile << world[0] << ' ' << world[1] << ' ' << world[2] << endl;
}




//~~~~~~~~~~~~~~~~~~~~UNUSED FUNCTIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~





//Set the color using the phong model (distance is not constant here)
void Vertex::setColor(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, glm::vec3 from, glm::vec3 source, float Ia, float Il,
	int n)
{
	//I(p) = ka * Ia + (Il/(|f-p|+C)) * (ka*cos(alpha) + ks * (cos(beta))^n)
	//Where:

	//The vector I(p) is the color at point p

	//ka, kd and ks are the ambient, diffuse and specular color coefficients

	//Ia is the ambient light intensity and Il is the intensity of the source

	//f is the from point (where the eye is)
	glm::vec3 f = from;

	//p is the point where the vertex is
	glm::vec3 p = world;

	//C is the distance of the light source
	float c = length(source - p);

	//alpha is the angle between n and l
	//n is the normal 
	//l = (x-p)/|x-p| is the light vector (x is the position of the light source)
	//Therefore cos(alpha) is n . l
	glm::vec3 light = (source - p) / glm::length(source - p);
	float ca = glm::dot(normal, light);


	//Beta is the angle between r and v
	//v is the view vector, (f-p)/|f-p|
	//r = -l + 2 (n . l) * n is the reflection vector
	//Therefore, cos(beta) = r.v
	glm::vec3 view = (f - p) / glm::length(f - p);
	glm::vec3 refl = -light + 2 * glm::dot(normal, light) * normal;
	float cb = glm::dot(refl, view);

	//n is the phong constant

	//Exceptional cases:

	//If light source and from point are on different sides, the intensity is the ambient intensity
	if (((glm::dot(normal, light) > 0) && (glm::dot(normal, view) > 0)) ||
		((glm::dot(normal, light) < 0) && (glm::dot(normal, view) < 0)))
	{
		color = ka * Ia;
	}

	//Highlights aren't visible: no specular light
	else if (glm::dot(refl, view) < 0)
	{
		color = ka * Ia + (Il / (glm::length(f - p) + c)*(kd * ca));
	}

	//Normal cases
	else
	{
		//I(p) = ka * Ia + (Il/(|f-p|+C)) * (ka*cos(alpha) + ks * (cos(beta))^n)
		color = ka * Ia + (Il / (glm::length(f - p) + c)*(kd * ca + ks * pow(cb, n)));
	}

}