#include "World.h"


//Set the polygon array, set initial boundaries
World::World(Poly* polygonarray, int numberpolygons)
{
	polygons = polygonarray;

	numPolygons = numberpolygons;

	//Initial boundaries
	InitializeBoundaries();

	//Write to the output
	WriteToOutput();
}


World::~World()
{
}

//Initialize the boundaries (0 to 1 in each axis)
void World::InitializeBoundaries()
{
	xmin = 0;
	xmax = 1;
	ymin = 0;
	ymax = 1;
	zmin = 0;
	zmax = 1;
}

//Get the minimum x boundary
float World::getxmin()
{
	return xmin;
}

//Get the maximum x boundary
float World::getxmax()
{
	return xmax;
}

//Get the minimum y boundary
float World::getymin()
{
	return ymin;
}

//Get the maximum y boundary
float World::getymax()
{
	return ymax;
}

//Get the minimum z boundary
float World::getzmin()
{
	return zmin;
}

//Get the maximum z boundary
float World::getzmax()
{
	return zmax;
}

//Get the size of the world in x
float World::getxSize()
{
	return xmax - xmin;
}

//Get the size of the world in y
float World::getySize()
{
	return ymax - ymin;
}

//Get the size of the world in z
float World::getzSize()
{
	return zmax - zmin;
}

//Write data (taken from polygon array) to the output file
void World::WriteToOutput()
{
	//If output.txt exists, delete it
	remove("output.txt");

	ofstream output;
	output.open("output.txt");

	//Write number of polygons on first line
	output << numPolygons << endl;

	//For each polygon, write info to output file
	for (int i = 0; i < numPolygons; i++)
	{
		polygons[i].writeToOutput(output);
	}
	output.close();
}

//Create the bounding box
void World::BoundingBox()
{
	//Set the bounds in x, y and z
	SetBoundsX();
	SetBoundsY();
	SetBoundsZ();
}

//Transform a polygon
void World::Transform(int transformation)
{
	//Prompt for the polygon id
	int polygonid = TransformationIDPrompt();

	//Use transformation number to determine which transformation to use
	if (transformation == TRANSLATE)
	{
		Translate(polygonid);
	}
	else if (transformation == ROTATE)
	{
		Rotate(polygonid);
	}
	else if (transformation == SCALE)
	{
		Scale(polygonid);
	}

	//Create bounding box
	BoundingBox();

	//Write to output
	WriteToOutput();
}

//Translate
void World::Translate(int polygonid)
{
	//Prompt for translation vector
	glm::vec3 tvec = TranslationPrompt(polygonid);

	//Translate the chosen polygon
	polygons[polygonid].Translate(tvec);
}

//Scale
void World::Scale(int polygonid)
{
	//Scale factors
	float scalex;
	float scaley;
	float scalez;

	//Prompt for scale factors
	ScalePrompt(scalex, scaley, scalez);

	//Scale the chosen polygon
	polygons[polygonid].Scale(scalex, scaley, scalez);
}

//Rotate
void World::Rotate(int polygonid)
{
	//Prompt for rotationangle
	float rotangle = RotationAnglePrompt();

	//Prompt for rotation axes (point 1 and point 2)
	glm::vec3 p1 = RotationAxisPrompt(1);
	glm::vec3 p2 = RotationAxisPrompt(2);

	//Rotate the chosen polygon
	polygons[polygonid].Rotate(rotangle, p1, p2);
}

//Prompt for the id of the polygon to transform
int World::TransformationIDPrompt()
{
	int polygonid;
	bool invalidvalue = false;
	do
	{
		cout << "Please enter the id of the polygon you wish to transform (the nth polygon in the input file has an id of n-1)" << endl;

		//Invalid data type
		if (!(cin >> polygonid))
		{
			cout << "Invalid value entered. Please try again. The value entered must be an integer." << endl;
			invalidvalue = true;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		//Invalid range
		else if (polygonid < 0 || polygonid > numPolygons - 1)
		{
			cout << "Invalid value entered. Please try again. The value entered must be between 0 and n-1, where n "
				<< "is the number of polygons." << endl;
			invalidvalue = true;
		}
		else
		{
			invalidvalue = false;
		}
	} while (invalidvalue);

	return polygonid;
}

//Prompt for translation vector
glm::vec3 World::TranslationPrompt(int polygonid)
{
	bool invalidvalue = false;
	float tx;
	float ty;
	float tz;

	//Repeat each prompt if user enters invalid value

	//for tx
	do
	{
		cout << "Please enter the x value by which you wish to translate polygon " << polygonid << ' ' << endl;

		//Invalid data type
		if (!(cin >> tx))
		{
			cout << "Invalid value entered. Please try again." << endl;
			invalidvalue = true;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		else
		{
			invalidvalue = false;
		}
	} while (invalidvalue);

	//for ty
	do
	{
		cout << "Please enter the y value by which you wish to translate polygon " << polygonid << ' ' << endl;

		//Invalid data type
		if (!(cin >> ty))
		{
			cout << "Invalid value entered. Please try again." << endl;
			invalidvalue = true;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else
		{
			invalidvalue = false;
		}
	} while (invalidvalue);

	//for tz
	do
	{
		cout << "Please enter the z value by which you wish to translate polygon " << polygonid << ' ' << endl;

		//Invalid data type
		if (!(cin >> tz))
		{
			cout << "Invalid value entered. Please try again." << endl;
			invalidvalue = true;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else
		{
			invalidvalue = false;
		}
	} while (invalidvalue);

	return glm::vec3(tx, ty, tz);
}

//Prompt for scale parameters
void World::ScalePrompt(float &scalex, float &scaley, float &scalez)
{
	bool invalidvalue = false;

	//Prompt for x scale factor
	do
	{
		cout << "Please enter the factor by which you wish to scale the polygon on the x-axis." << endl;

		//Invalid data type
		if (!(cin >> scalex))
		{
			cout << "Invalid value entered. Please try again." << endl;
			invalidvalue = true;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else
		{
			invalidvalue = false;
		}
	} while (invalidvalue);

	//Prompt for y scale factor
	do
	{
		cout << "Please enter the factor by which you wish to scale the polygon on the y-axis." << endl;

		//Invalid data type
		if (!(cin >> scaley))
		{
			cout << "Invalid value entered. Please try again." << endl;
			invalidvalue = true;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else
		{
			invalidvalue = false;
		}
	} while (invalidvalue);

	//Prompt for z scale factor
	do
	{
		cout << "Please enter the factor by which you wish to scale the polygon on the z-axis." << endl;

		//Invalid data type
		if (!(cin >> scalez))
		{
			cout << "Invalid value entered. Please try again." << endl;
			invalidvalue = true;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else
		{
			invalidvalue = false;
		}
	} while (invalidvalue);
}

//Prompt user for rotation angle
float World::RotationAnglePrompt()
{
	bool invalidvalue = false;
	float rotangle;

	//If invalid value entered, repeat prompt
	do
	{
		cout << "Please enter the angle (in degrees) by which you wish to rotate the polygon." << endl;

		//Invalid data type
		if (!(cin >> rotangle))
		{
			cout << "Invalid value entered. Please try again." << endl;
			invalidvalue = true;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else
		{
			invalidvalue = false;
		}
	} while (invalidvalue);

	return rotangle;
}

//Prompt for rotation axis
glm::vec3 World::RotationAxisPrompt(int pointNumber)
{
	bool invalidvalue = false;
	float x;
	float y;
	float z;

	//Prompt the user for x, y and z values of point on the rotation axis
	cout << "Please enter the coordinates of point " << pointNumber << " on the rotation axis" << endl;

	//for x
	do
	{
		cout << "Please enter the x value of point " << pointNumber << endl;

		//Invalid data type
		if (!(cin >> x))
		{
			cout << "Invalid value entered. Please try again." << endl;
			invalidvalue = true;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		else
		{
			invalidvalue = false;
		}
	} while (invalidvalue);

	//for ty
	do
	{
		cout << "Please enter the y value of point " << pointNumber << endl;

		//Invalid data type
		if (!(cin >> y))
		{
			cout << "Invalid value entered. Please try again." << endl;
			invalidvalue = true;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else
		{
			invalidvalue = false;
		}
	} while (invalidvalue);

	//for tz
	do
	{
		cout << "Please enter the z value of point " << pointNumber << endl;

		//Invalid data type
		if (!(cin >> z))
		{
			cout << "Invalid value entered. Please try again." << endl;
			invalidvalue = true;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else
		{
			invalidvalue = false;
		}
	} while (invalidvalue);

	return glm::vec3(x, y, z);
}

//Set the x bounds of the bounding box
void World::SetBoundsX()
{
	xmin = GetMinimumX();
	xmax = GetMaximumX();
}

//Set the y bounds of the bounding box
void World::SetBoundsY()
{
	ymin = GetMinimumY();
	ymax = GetMaximumY();
}

//Set the z bounds of the bounding box
void World::SetBoundsZ()
{
	zmin = GetMinimumZ();
	zmax = GetMaximumZ();
}

//Get maximum x among polygon vertices
float World::GetMaximumX()
{
	float maxx = xmax; //max is the current max by default

	//For each polygon, find maximum x value. If greater, it becomes new max
	for (int i = 0; i < numPolygons; i++)
	{
		float polymax = polygons[i].getXmax();

		if (maxx < polymax)
		{
			maxx = polymax;
		}
	}
	
	return maxx;
}

//Get minimum x among polygon vertices
float World::GetMinimumX()
{
	float minx = xmin; //min is the current min by default

	//For each polygon, find minimum x value. If smaller, it becomes new max
	for (int i = 0; i < numPolygons; i++)
	{
		float polymax = polygons[i].getXmin();

		if (minx > polymax)
		{
			minx = polymax;
		}
	}

	return minx;
}

//Get maximum y among polygon vertices
float World::GetMaximumY()
{
	float maxy = ymax; //max is the current max by default

	//For each polygon, find maximum y value. If greater, it becomes new max
	for (int i = 0; i < numPolygons; i++)
	{
		float polymax = polygons[i].getYmax();

		if (maxy < polymax)
		{
			maxy = polymax;
		}
	}

	return maxy;
}

//Get minimum y among polygon vertices
float World::GetMinimumY()
{
	float miny = ymin; //min is the current min by default

	//For each polygon, find minimum y value. If smaller, it becomes new min
	for (int i = 0; i < numPolygons; i++)
	{
		float polymax = polygons[i].getYmin();

		if (miny > polymax)
		{
			miny = polymax;
		}
	}

	return miny;
}

//Get maximum z among polygon vertices
float World::GetMaximumZ()
{
	float maxz = zmax; //max is the current max by default

	//For each polygon, find maximum z value. If greater, it becomes new max
	for (int i = 0; i < numPolygons; i++)
	{
		float polymax = polygons[i].getZmax();

		if (maxz < polymax)
		{
			maxz = polymax;
		}
	}

	return maxz;
}

//Get minimum z among polygon vertices
float World::GetMinimumZ()
{
	float minz = zmin; //min is the current min by default

	//For each polygon, find minimum y value. If smaller, it becomes new min
	for (int i = 0; i < numPolygons; i++)
	{
		float polymax = polygons[i].getZmin();

		if (minz > polymax)
		{
			minz = polymax;
		}
	}

	return minz;
}

//Initialize the vertex colors
void World::initializeVertexColors()
{
	//Prompt for phong parameters
	PhongParameterPrompt();

	//Set the vertex colors
	setVertexColors();
}

//Set the vertex colors
void World::setVertexColors()
{
	//Set the vertex colors for each polygon
	for (int i = 0; i < numPolygons; i++)
	{
		polygons[i].setVertexColors(source, from, ambientcoefficient, diffusecoefficient, specularcoefficient,
			ambientIntensity, lightIntensity, phongconstant, distance);
	} 
	
}

//Reilluminate using the phong model, changing one parameter
void World::ReIlluminate(int ParameterToChange)
{
	//Change the given parameter
	ParameterChangePrompt(ParameterToChange);

	//Reset the vertex colors for each polygon
	setVertexColors();
}

//Prompt for user input to change the given parameter
void World::ParameterChangePrompt(int ParameterToChange)
{
	switch (ParameterToChange)
	{
		case NO_CHANGE: //No change - do nothing
			break;
		case CHANGE_PHONG_SOURCE:
			source = VectorParameterPrompt("the light source position", false); //x
			break;
		case CHANGE_PHONG_FROM:
			from = VectorParameterPrompt("the from point", false); //f
			break;
		case CHANGE_PHONG_AMB_COEFF:
			ambientcoefficient = ColorCoefficientPrompt("the ambient color coefficient", true); //ka
			break;
		case CHANGE_PHONG_DIFF_COEFF:
			diffusecoefficient = ColorCoefficientPrompt("the diffuse color coefficient", true); //kd
			break;
		case CHANGE_PHONG_SPEC_COEFF:
			specularcoefficient = ColorCoefficientPrompt("the specular color coefficient", true); //ks
			break;
		case CHANGE_PHONG_AMB_INTENSITY:
			ambientIntensity = ScalarParameterPrompt("the ambient light intensity", false, true); //Ia
			break;
		case CHANGE_PHONG_LIGHT_INTENSITY:
			lightIntensity = ScalarParameterPrompt("the intensity of the light source", false, true); //Il
			break;
		case CHANGE_PHONG_PHONG_CONS:
			phongconstant = IntegerParameterPrompt("the phong constant", true); //n
			break;
		
	}

	//Calculate the distance
	distance = calculateLightDistance(source);
}

//Prompt the user for the phong parameters
void World::PhongParameterPrompt()
{
	//Prompt user for each parameter
	source = VectorParameterPrompt("the light source position", false); //x
	from = VectorParameterPrompt("the from point", false); //f
	ambientcoefficient = ColorCoefficientPrompt("the ambient color coefficient", true); //ka
	diffusecoefficient = ColorCoefficientPrompt("the diffuse color coefficient", true); //kd
	specularcoefficient = ColorCoefficientPrompt("the specular color coefficient", true); //ks
	ambientIntensity = ScalarParameterPrompt("the ambient light intensity", false, true); //Ia
	lightIntensity = ScalarParameterPrompt("the intensity of the light source", false, true); //Il
	phongconstant = IntegerParameterPrompt("the phong constant", true); //n
	
	//Calculate the distance
	distance = calculateLightDistance(source);

}

//Prompt for scalar parameters
float World::ScalarParameterPrompt(string parametername, bool normalized, bool nonnegative)
{
	bool invalidvalue = false;
	float input;

	do
	{
		//Prompt the user for the parameter
		cout << "Please enter the value of " << parametername << "." << endl;
		cout << "This value MUST be a real number!" << endl;

		//Normalized prompt
		if (normalized)
		{
			cout << "This value must be between 0 and 1" << endl;
		}

		//Nonnegative prompt
		if (nonnegative)
		{
			cout << "This value cannot be negative" << endl;
		}

		//Invalid data type
		if (!(cin >> input))
		{
			cout << "Invalid value entered. Please try again." << endl;
			invalidvalue = true;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		//Non normalized input when normalized only is on
		else if ((normalized) && ((input < 0) || (input > 1)))
		{
			cout << "The value must be between 0 and 1. Please try again" << endl;
			invalidvalue = true;
		}

		//Negative input while nonnegative is on
		else if (nonnegative && (input < 0))
		{
			cout << "The value must be positive. Please try again" << endl;
			invalidvalue = true;
		}

		else
		{
			invalidvalue = false;
		}
	} while (invalidvalue);

	return input;
}

//Prompt for vector (x, y, z) parameters
glm::vec3 World::VectorParameterPrompt(string parametername, bool normalizedvalues)
{
	//Prompt for x, y and z values
	float x = VectorElementPrompt("x", parametername, normalizedvalues);
	float y = VectorElementPrompt("y", parametername, normalizedvalues);
	float z = VectorElementPrompt("z", parametername, normalizedvalues);

	return glm::vec3(x, y, z);
}

//Prompt for vector (R, G, B) parameters
glm::vec3 World::ColorCoefficientPrompt(string coefficient, bool normalizedvalues)
{
	//Prompt for R, G, B values
	float R = VectorElementPrompt("R", coefficient, normalizedvalues);
	float G = VectorElementPrompt("G", coefficient, normalizedvalues);
	float B = VectorElementPrompt("B", coefficient, normalizedvalues);

	return glm::vec3(R, G, B);
}

//Prompt for integer parameters
int World::IntegerParameterPrompt(string parametername, bool PositiveOnly)
{
	bool invalidvalue = false;
	int input;

	do
	{
		//Prompt the user for the parameter
		cout << "Please enter the value of " << parametername << "." << endl;
		cout << "This value MUST be an integer!" << endl;

		//Positive only prompt
		if (PositiveOnly)
		{
			cout << "This value cannot be negative" << endl;
		}

		//Invalid data type
		if (!(cin >> input))
		{
			cout << "Invalid value entered. Please try again." << endl;
			invalidvalue = true;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		//Negative number when positive only
		else if ((PositiveOnly) && input < 0)
		{
			cout << "The value entered cannot be negative. Please try again" << endl;
			invalidvalue = true;
		}

		else
		{
			invalidvalue = false;
		}
	} while (invalidvalue);

	return input;
}

//Calculate the distance from the light source to the center of the scene. 
float World::calculateLightDistance(glm::vec3 src)
{
	glm::vec3 center((xmax - xmin)/2, (ymax-ymin)/2, (zmax-zmin)/2); //The center of the scene
	
	glm::vec3 d = src - center; //The distance vector

	float dist = glm::length(d); //The magnitude of the distance vector

	return dist;
}

//Prompt for vector element <elementname> of vector <vectorname>
float World::VectorElementPrompt(string elementname, string vectorname, bool normalized)
{
	bool invalidvalue = false;
	float input;

	do
	{
		//Prompt the user for the value of the element of the vector
		cout << "Please enter the " << elementname << " value of " << vectorname << "." << endl;

		//Normalized prompt
		if (normalized)
		{
			cout << "This value must be between 0 and 1" << endl;
		}

		//Invalid data type
		if (!(cin >> input))
		{
			cout << "Invalid value entered. Please try again." << endl;
			invalidvalue = true;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		//Non normalized input when normalized only is on
		else if ((normalized) && ((input < 0) || (input > 1)))
		{
			cout << "The value must be between 0 and 1. Please try again" << endl;
			invalidvalue = true;
		}

		else
		{
			invalidvalue = false;
		}
	} while (invalidvalue);

	return input;
}
