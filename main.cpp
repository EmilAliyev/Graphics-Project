#include <windef.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Polygon.h"
#include "Unit.h"
#include "World.h"
#include "Projection.h"
#include "PixelPlane.h"

//Function definitions
void InitializeImage();
void display();
void InitializePolygons();

//Menus
void menuSetup();
void menu(int val);
void TransformMenu(int val);
void PhongMenu(int val);
void CoeffMenu(int val);
void IntensityMenu(int val);

//Default resolution and window position
const int xresolution = 480;
const int yresolution = 480;
const int windowposx = 500;
const int windowposy = 250;

//Transformation IDs
const int TRANSFORM_TRANSLATE = 1;
const int TRANSFORM_ROTATE = 2;
const int TRANSFORM_SCALE = 3;

//Phong parameter change ids
const int PHONG_NO_CHANGE = 0;
const int PHONG_CHANGE_SOURCE = 1;
const int PHONG_CHANGE_FROM = 2;
const int PHONG_CHANGE_AMB_COEFF = 3;
const int PHONG_CHANGE_DIFF_COEFF = 4;
const int PHONG_CHANGE_SPEC_COEFF = 5;
const int PHONG_CHANGE_AMB_INTENSITY = 6;
const int PHONG_CHANGE_LIGHT_INTENSITY = 7;
const int PHONG_CHANGE_PHONG_CONS = 8;

using namespace std;

float *PixelBuffer; //The pixel buffer
float *HalfTonedBuffer; //The half-toned buffer
Poly* polygons; //Array containing polygons
int numPolygons; //Number of polygons
World *world; //The scene
bool halftoning = false; //Half toning on/off
int projectionplane; //The plane to project to

int main(int argc, char** argv)
{
	
	//Get the initial image into the pixel buffer
	InitializeImage();

	//Draw the image

	//Initialize glut
	glutInit(&argc, argv);

	//Set window size and position
	glutInitWindowSize(xresolution, yresolution);
	glutInitWindowPosition(windowposx, windowposy);

	//Set window title
	glutCreateWindow("Polygon drawer");
	glClearColor(0, 0, 0, 0);
	glutDisplayFunc(display);

	//Set up menu
	menuSetup();

	//Main glut loop
	glutMainLoop();

	delete[] polygons;
	delete[] PixelBuffer;
	delete[] HalfTonedBuffer;
	delete world;
}

//The main display loop
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();


	//draw the pixels

	//Draw the normal pixel buffer is half toning is off
	if (!halftoning)
	{
		glDrawPixels(xresolution, yresolution, GL_RGB, GL_FLOAT, PixelBuffer);
	}

	//Otherwise, draw the half toned buffer
	else
	{
		glDrawPixels(xresolution, yresolution, GL_RGB, GL_FLOAT, HalfTonedBuffer);
	}

	glFlush();
}

//Create the pixel buffer for the initial image in the file
void InitializeImage()
{
	//Initialize PixelBuffer and the half toned pixel bfufer
	PixelBuffer = new float[xresolution * yresolution * 3];

	for (int i = 0; i < xresolution * yresolution * 3; i++)
	{
		PixelBuffer[i] = 0;
	}

	

	//Get initial polygons from file
	InitializePolygons();

	//World Plane
	world = new World (polygons, numPolygons); //Initialize the world
	world->initializeVertexColors(); //Set the initial vertex colors

	//NDC
	Unit NDC(polygons, world->getxSize(), world->getySize(), world->getzSize(), world->getxmin(), world->getymin(), world->getzmin(), 
		numPolygons);
	NDC.Create();

	//Projection

	//Prompt user for plane to project to
	cout << "Please select the plane you would like to project to (press the corresponding button)" << endl;
	cout << "1. xy-plane" << endl;
	cout << "2. xz-plane" << endl;
	cout << "3. yz-plane" << endl;

	cin >> projectionplane;

	//Create projection

	Projection projection(polygons, projectionplane, numPolygons);

	//Project
	projection.Project();

	//Pixel Plane
	
	PixelPlane pxplane(polygons, xresolution, yresolution, numPolygons, 1, projectionplane);
	pxplane.Create();

	//Copy pixel buffer from pixel plane
	float *TempPixelBuffer = pxplane.GetPixelBuffer(); 
	
	for (int i = 0; i < xresolution * yresolution * 3; i++)
	{
		PixelBuffer[i] = TempPixelBuffer[i];
	}

	HalfTonedBuffer = pxplane.halfTone(PixelBuffer, xresolution, yresolution);

}

//Transform (transformation id tells which transformation to perform) and redraw
void Transform(int transformationid)
{

	//Clear PixelBuffer
	for (int i = 0; i < xresolution * yresolution * 3; i++)
		PixelBuffer[i] = 0;

	//World Plane
	world->ReIlluminate(PHONG_NO_CHANGE); //Reilluminate with no changes

	//Transform according to transformation id
	world->Transform(transformationid);

	//NDC
	Unit NDC(polygons, world->getxSize(), world->getySize(), world->getzSize(), 
		world->getxmin(), world->getymin(), world->getzmin(), numPolygons);
	NDC.Create();

	//Projection

	//Prompt user for plane to project to
	cout << "Please select the plane you would like to project to (enter the corresponding key on the keyboard)" << endl;
	cout << "1. xy-plane" << endl;
	cout << "2. xz-plane" << endl;
	cout << "3. yz-plane" << endl;

	cin >> projectionplane;

	//Create projection

	Projection projection(polygons, projectionplane, numPolygons);

	//Project
	projection.Project();

	//Pixel Plane

	PixelPlane pxplane(polygons, xresolution, yresolution, numPolygons, 1, projectionplane);
	pxplane.Create();

	//Copy pixel buffer from pixel plane
	float *TempPixelBuffer = pxplane.GetPixelBuffer();

	for (int i = 0; i < xresolution * yresolution * 3; i++)
	{
		PixelBuffer[i] = TempPixelBuffer[i];
	}
}

//Change given phong parameter, reilluminate and redraw
void ReIlluminate(int parameterToChange)
{

	//Clear PixelBuffer
	for (int i = 0; i < xresolution * yresolution * 3; i++)
		PixelBuffer[i] = 0;

	//World Plane
	world->ReIlluminate(parameterToChange);

	//NDC
	Unit NDC(polygons, world->getxSize(), world->getySize(), world->getzSize(), world->getxmin(), 
		world->getymin(), world->getzmin(), numPolygons);
	NDC.Create();

	//Projection

	//Prompt user for plane to project to
	cout << "Please select the plane you would like to project to (enter the corresponding key on the keyboard)" << endl;
	cout << "1. xy-plane" << endl;
	cout << "2. xz-plane" << endl;
	cout << "3. yz-plane" << endl;

	cin >> projectionplane;

	//Create projection

	Projection projection(polygons, projectionplane, numPolygons);

	//Project
	projection.Project();

	//Pixel Plane

	PixelPlane pxplane(polygons, xresolution, yresolution, numPolygons, 1, projectionplane);
	pxplane.Create();

	//Copy pixel buffer from pixel plane
	float *TempPixelBuffer = pxplane.GetPixelBuffer();

	for (int i = 0; i < xresolution * yresolution * 3; i++)
	{
		PixelBuffer[i] = TempPixelBuffer[i];
	}

}

//Change Light Source Position
void PhongChangeSource()
{
	//Reilluminate, changing the light source position
	ReIlluminate(PHONG_CHANGE_SOURCE);

	//Go back to display
	glutIdleFunc(NULL);

	glutPostRedisplay();


}

//Change From Point
void PhongChangeFrom()
{
	//Reilluminate, changing the from point
	ReIlluminate(PHONG_CHANGE_FROM);

	//Go back to display
	glutIdleFunc(NULL);

	glutPostRedisplay();
}

//Change ambient light coefficient
void PhongChangeAmbientCoeff()
{
	//Reilluminate, changing the ambient coefficient
	ReIlluminate(PHONG_CHANGE_AMB_COEFF);

	//Go back to display
	glutIdleFunc(NULL);

	glutPostRedisplay();
}

//Change diffuse light coefficient
void PhongChangeDiffuseCoeff()
{
	//Reilluminate, changing the diffuse coefficient
	ReIlluminate(PHONG_CHANGE_DIFF_COEFF);

	//Go back to display
	glutIdleFunc(NULL);

	glutPostRedisplay();
}

//Change specular light coefficient
void PhongChangeSpecularCoeff()
{
	//Reilluminate, changing the specular coefficient
	ReIlluminate(PHONG_CHANGE_SPEC_COEFF);

	//Go back to display
	glutIdleFunc(NULL);

	glutPostRedisplay();
}

//Change ambient intensity
void PhongChangeAmbIntensity()
{
	//Reilluminate, changing the ambient intensity
	ReIlluminate(PHONG_CHANGE_AMB_INTENSITY);

	//Go back to display
	glutIdleFunc(NULL);

	glutPostRedisplay();
}

//Change source intensity
void PhongChangeLightIntensity()
{
	//Reilluminate, changing the source intensity
	ReIlluminate(PHONG_CHANGE_LIGHT_INTENSITY);

	//Go back to display
	glutIdleFunc(NULL);

	glutPostRedisplay();
}

//Change phong constant
void PhongChangePhongConstant()
{
	//Reilluminate, changing the phong constant
	ReIlluminate(PHONG_CHANGE_PHONG_CONS);

	//Go back to display
	glutIdleFunc(NULL);

	glutPostRedisplay();
}

//Translate
void Translate()
{
	//Call transform with translate id
	Transform(TRANSFORM_TRANSLATE);

	//Go back to display
	glutIdleFunc(NULL);

	glutPostRedisplay();
}

//Scale
void Scale()
{
	//Call transform with scale id
	Transform(TRANSFORM_SCALE);

	//Go back to display
	glutIdleFunc(NULL);

	glutPostRedisplay();
}

//Rotate
void Rotate()
{
	//Call transform with scale id
	Transform(TRANSFORM_ROTATE);

	//Go back to display
	glutIdleFunc(NULL);

	glutPostRedisplay();
}

//Toggle half toning on/off
void ToggleHalfToning()
{
	//Toggle half toning
	if (halftoning)
	{
		halftoning = false;
	}

	else
	{
		halftoning = true;
	}

	//Go back to display
	glutIdleFunc(NULL);

	glutPostRedisplay();
}

//Get initial polygons from file
void InitializePolygons()
{
	string filename;

	//Prompt the user for the file name
	cout << "Please enter the input file name" << endl;
	cin >> filename;

	//Open the file with this name
	ifstream file;
	const char *fname = filename.c_str();
	file.open(fname);

	//Get the number of polygons from the first line
	file >> numPolygons;

	//Initialize polygon array
	polygons = new Poly[numPolygons];

	//For each polygon, get vertices and lines from file
	for (int i = 0; i < numPolygons; i++)
	{
		polygons[i].InitializePolygon(file);
	}
	
	file.close();

}

//Transform Menu loop
void TransformMenu(int val)
{
	//Translate
	if (val == 1)
	{
		//Set idle function to translate
		glutIdleFunc(Translate);
	}

	//Rotate
	if (val == 2)
	{
		//Set idle function to rotate
		glutIdleFunc(Rotate);
	}

	//Scale
	if (val == 3)
	{
		//Set idle function to scale
		glutIdleFunc(Scale);
	}
}

//The menu to change phong parameters
void PhongMenu(int val)
{
	//Change light source position
	if (val == 1)
	{
		glutIdleFunc(PhongChangeSource);
	}

	//Change the from point
	if (val == 2)
	{
		glutIdleFunc(PhongChangeFrom);
	}

	//Change the phong constant
	if (val == 3)
	{
		glutIdleFunc(PhongChangePhongConstant);
	}
}

//The menu to change color coefficients (ambient, diffuse or specular)
void CoeffMenu(int val)
{
	//Change ambient coefficient
	if (val == 1)
	{
		glutIdleFunc(PhongChangeAmbientCoeff);
	}

	//Change diffuse coefficient
	if (val == 2)
	{
		glutIdleFunc(PhongChangeDiffuseCoeff);
	}

	//Change specular coefficient
	if (val == 3)
	{
		glutIdleFunc(PhongChangeSpecularCoeff);
	}

}

//The menu to change light intensities (ambient or source)
void IntensityMenu(int val)
{
	//Change ambient intensity
	if (val == 1)
	{
		glutIdleFunc(PhongChangeAmbIntensity);
	}

	//Change light source intensity
	if (val == 2)
	{
		glutIdleFunc(PhongChangeLightIntensity);
	}
}

//Set up the menus
void menuSetup()
{
	//Set up transformation sub menu
	int transformmenu = glutCreateMenu(TransformMenu);
	glutAddMenuEntry("Translate", 1);
	glutAddMenuEntry("Rotate", 2);
	glutAddMenuEntry("Scale", 3);

	//Set up color coefficient sub menu (sub menu of illumination sub menu)
	int coefficientmenu = glutCreateMenu(CoeffMenu);
	glutAddMenuEntry("Ambient", 1);
	glutAddMenuEntry("Diffuse", 2);
	glutAddMenuEntry("Specular", 3);

	//Set up intensity sub menu (sub menu of illumination sub menu)
	int intensitysubmenu = glutCreateMenu(IntensityMenu);
	glutAddMenuEntry("Ambient Intensity", 1);
	glutAddMenuEntry("Light Intensity", 2);

	//Set up illumination sub menu
	int phongmenu = glutCreateMenu(PhongMenu);
	glutAddSubMenu("Color Coefficients", coefficientmenu);
	glutAddSubMenu("Intensity", intensitysubmenu);
	glutAddMenuEntry("Light Source Position", 1);
	glutAddMenuEntry("From Point", 2);
	glutAddMenuEntry("Phong Constant", 3);

	//Set up main menu
	int mainmenu = glutCreateMenu(menu);
	glutSetMenu(mainmenu);
	glutAddSubMenu("Phong Parameters", phongmenu);
	glutAddSubMenu("Transform", transformmenu);
	glutAddMenuEntry("Toggle half-toning", 1);
	glutAddMenuEntry("Exit", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//main menu loop
void menu(int val)
{
	//Toggle half-toning
	if (val == 1)
	{
		glutIdleFunc(ToggleHalfToning);
	}

	//Exit
	if (val == 2)
	{
		exit(0);
	}
}
