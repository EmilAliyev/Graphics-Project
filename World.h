#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "Polygon.h"

//Transformation ids
const int TRANSLATE = 1;
const int ROTATE = 2;
const int SCALE = 3;

//Phong parameter change ids
const int NO_CHANGE = 0;
const int CHANGE_PHONG_SOURCE = 1;
const int CHANGE_PHONG_FROM = 2;
const int CHANGE_PHONG_AMB_COEFF = 3;
const int CHANGE_PHONG_DIFF_COEFF = 4;
const int CHANGE_PHONG_SPEC_COEFF = 5;
const int CHANGE_PHONG_AMB_INTENSITY = 6;
const int CHANGE_PHONG_LIGHT_INTENSITY = 7;
const int CHANGE_PHONG_PHONG_CONS = 8;

class World
{
	Poly* polygons;
	int numPolygons;

	//Boundaries
	float xmin;
	float xmax;
	float ymin;
	float ymax;
	float zmin;
	float zmax;

	//Phong parameters:
	glm::vec3 source;
	glm::vec3 from;
	glm::vec3 ambientcoefficient;
	glm::vec3 diffusecoefficient;
	glm::vec3 specularcoefficient;
	float ambientIntensity;
	float lightIntensity;
	int phongconstant;
	float distance;


public:
	World(Poly * polygonarray, int numberpolygons);
	~World();

	void InitializeBoundaries();

	//Get boundaries
	float getxmin();
	float getxmax();
	float getymin();
	float getymax();
	float getzmin();
	float getzmax();

	//Get size of world plane in x y and z
	float getxSize();
	float getySize();
	float getzSize();

	//Write to the output
	void WriteToOutput();

	//Create the bounding box
	void BoundingBox();

	//Transformations
	void Transform(int transformation);
	void Translate(int polygonid);
	void Scale(int polygonid);
	void Rotate(int polygonid);

	//Transformation prompts
	int TransformationIDPrompt();
	glm::vec3 TranslationPrompt(int polygonid);
	void ScalePrompt(float &scalex, float &scaley, float &scalez);
	float RotationAnglePrompt();
	glm::vec3 RotationAxisPrompt(int pointNumber);

	//Bounding box helper functions

	//Set bounds
	void SetBoundsX();
	void SetBoundsY();
	void SetBoundsZ();

	//Get extrema
	float GetMaximumX();
	float GetMinimumX();
	float GetMaximumY();
	float GetMinimumY();
	float GetMaximumZ();
	float GetMinimumZ();

	//Set the vertex colors using the phong model
	void initializeVertexColors();
	void setVertexColors();
	void ReIlluminate(int ParameterToChange);
	void ParameterChangePrompt(int ParameterToChange);

	//Phong parameters
	void PhongParameterPrompt();
	float ScalarParameterPrompt(string parametername, bool normalized, bool nonnegative);
	glm::vec3 VectorParameterPrompt(string parametername, bool normalizedvalues);
	glm::vec3 ColorCoefficientPrompt(string coefficient, bool normalizedvalues);
	int IntegerParameterPrompt(string parametername, bool positiveOnly);

	float calculateLightDistance(glm::vec3 src);

	//Prompts for x, y, z values
	float VectorElementPrompt(string elementname, string vectorname, bool normalizedvalues);
};

