// ==========================================================================
// Barebones OpenGL Core Profile Boilerplate
//    using the GLFW windowing system (http://www.glfw.org)
//
// Loosely based on
//  - Chris Wellons' example (https://github.com/skeeto/opengl-demo) and
//  - Camilla Berglund's example (http://www.glfw.org/docs/latest/quick.html)
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#pragma comment(lib, "opengl32.lib")	// TODO: Do this in the project properties?

#include <GL/glew.h>					// Only necessary on Windows
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "SphericalCamera.h"
#include "SphericalBody.h"

#define PI 3.14159265359

using namespace std;
using namespace glm;

//Forward definitions
bool CheckGLErrors(string location);
void QueryGLVersion();
string LoadSource(const string &filename);
GLuint CompileShader(GLenum shaderType, const string &source);
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);

vec2 mousePos;
GLFWwindow* window = 0;

SphericalCamera cam;
mat4 perspectiveMatrix;

bool mousePressed = false;



SphericalBody *currentSelected;
int sceneIndex = 0;



// --------------------------------------------------------------------------
// GLFW callback functions

// reports GLFW errors
void ErrorCallback(int error, const char* description)
{
	cout << "GLFW ERROR " << error << ":" << endl;
	cout << description << endl;
}

void setSelectedSphericalBody(SphericalBody *body) {
	currentSelected = body;
	SphericalBody *orbitee = body->orbitee;
	if(orbitee && orbitee->orbiters.size() > 0) {
		for(int i = 0; i < orbitee->orbiters.size(); i++) {
			if(orbitee->orbiters[i] == body) {
				sceneIndex = i;
				break;
			}
		}
	} else {
		sceneIndex = 0;
	}
}


bool globalNormalMapOverride = true;

// handles keyboard input events
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		} else if(key == GLFW_KEY_A) {
			SphericalBody *orbitee = currentSelected->orbitee;
			if(orbitee && orbitee->orbiters.size() > 0) {
				sceneIndex--;
				if(sceneIndex < 0) sceneIndex = orbitee->orbiters.size() - 1;
				currentSelected = orbitee->orbiters[sceneIndex];
			}
		} else if(key == GLFW_KEY_D) {
			SphericalBody *orbitee = currentSelected->orbitee;
			if(orbitee && orbitee->orbiters.size() > 0) {
				sceneIndex = (sceneIndex + 1) % orbitee->orbiters.size();
				currentSelected = orbitee->orbiters[sceneIndex];
			}
		} else if(key == GLFW_KEY_W) {
			if(currentSelected->orbitee) {
				setSelectedSphericalBody(currentSelected->orbitee);
			}
		} else if(key == GLFW_KEY_S) {
			if(currentSelected->orbiters.size() > 0) {
				setSelectedSphericalBody(currentSelected->orbiters[0]);
			}
		} else if(key == GLFW_KEY_SPACE) {
			// globalNormalMapOverride = !globalNormalMapOverride;
		}
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if( (action == GLFW_PRESS) || (action == GLFW_RELEASE) )
		mousePressed = !mousePressed;
}

void scrollWheelCallback(GLFWwindow* window, double xoffset, double yoffset) {
	cam.incrementSpherical(vec3(0.f, 0.f, -(float)yoffset*3));
}

void mousePosCallback(GLFWwindow* window, double xpos, double ypos) {
	int vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);

	vec2 newPos = vec2(xpos/(double)vp[2], -ypos/(double)vp[3])*2.f - vec2(1.f);
	vec2 diff = newPos - mousePos;

	if(mousePressed)
		cam.incrementSpherical(vec3(diff.y, diff.x, 0.f));

	mousePos = newPos;
}

void resizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}



//==========================================================================
// TUTORIAL STUFF


//vec2 and vec3 are part of the glm math library. 
//Include in your own project by putting the glm directory in your project, 
//and including glm/glm.hpp as I have at the top of the file.
//"using namespace glm;" will allow you to avoid writing everyting as glm::vec2
vector<vec3> points;
vector<vec2> uvs;

//Structs are simply acting as namespaces
//Access the values like so: VAO::LINES
struct VAO{
	enum {LINES=0, COUNT};		//Enumeration assigns each name a value going up
										//LINES=0, COUNT=1
};

struct VBO{
	enum {POINTS=0, UVS, COUNT};	//POINTS=0, UVS=1, COUNT=2
};

struct SHADER{
	enum {LINE=0, COUNT};		//LINE=0, COUNT=1
};

GLuint vbo [VBO::COUNT];		//Array which stores OpenGL's vertex buffer object handles
GLuint vao [VAO::COUNT];		//Array which stores Vertex Array Object handles
GLuint shader [SHADER::COUNT];		//Array which stores shader program handles


//Gets handles from OpenGL
void generateIDs()
{
	glGenVertexArrays(VAO::COUNT, vao);		//Tells OpenGL to create VAO::COUNT many
														// Vertex Array Objects, and store their
														// handles in vao array
	glGenBuffers(VBO::COUNT, vbo);		//Tells OpenGL to create VBO::COUNT many
													//Vertex Buffer Objects and store their
													//handles in vbo array
}

//Clean up IDs when you're done using them
void deleteIDs()
{
	for(int i=0; i<SHADER::COUNT; i++)
	{
		glDeleteProgram(shader[i]);
	}
	
	glDeleteVertexArrays(VAO::COUNT, vao);
	glDeleteBuffers(VBO::COUNT, vbo);	
}


//Describe the setup of the Vertex Array Object
bool initVAO()
{
	glBindVertexArray(vao[VAO::LINES]);		//Set the active Vertex Array

	glEnableVertexAttribArray(0);		//Tell opengl you're using layout attribute 0 (For shader input)
	glBindBuffer( GL_ARRAY_BUFFER, vbo[VBO::POINTS] );		//Set the active Vertex Buffer
	glVertexAttribPointer(
		0,				//Attribute
		3,				//Size # Components
		GL_FLOAT,	//Type
		GL_FALSE, 	//Normalized?
		sizeof(vec3),	//Stride
		(void*)0			//Offset
		);
	
	glEnableVertexAttribArray(1);		//Tell opengl you're using layout attribute 1
	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO::UVS]);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vec2),
		(void*)0
		);

	return !CheckGLErrors("initVAO");		//Check for errors in initialize
}


//Loads buffers with data
bool loadBuffer(const vector<vec3>& points, const vector<vec2>& uvs)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO::POINTS]);
	glBufferData(
		GL_ARRAY_BUFFER,				//Which buffer you're loading too
		sizeof(vec3)*points.size(),	//Size of data in array (in bytes)
		&points[0],							//Start of array (&points[0] will give you pointer to start of vector)
		GL_STATIC_DRAW						//GL_DYNAMIC_DRAW if you're changing the data often
												//GL_STATIC_DRAW if you're changing seldomly
		);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO::UVS]);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(vec2)*uvs.size(),
		&uvs[0],
		GL_STATIC_DRAW
		);

	return !CheckGLErrors("loadBuffer");	
}

//Compile and link shaders, storing the program ID in shader array
bool initShader()
{	
	string vertexSource = LoadSource("Engine/Shaders/vertex.glsl");		//Put vertex file text into string
	string fragmentSource = LoadSource("Engine/Shaders/fragment.glsl");		//Put fragment file text into string

	GLuint vertexID = CompileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragmentID = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
	
	shader[SHADER::LINE] = LinkProgram(vertexID, fragmentID);	//Link and store program ID in shader array

	return !CheckGLErrors("initShader");
}

//For reference:
//	https://open.gl/textures
GLuint createTexture(const char* filename)
{
	int components;
	GLuint texID;
	int tWidth, tHeight;

	//stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename, &tWidth, &tHeight, &components, 0);
	
	if(data != NULL)
	{
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		if(components==3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if(components==4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tWidth, tHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Clean up
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);

		return texID;
	} 
	
	return 0;	//Error
}

//Use program before loading texture
//	texUnit can be - GL_TEXTURE0, GL_TEXTURE1, etc...
bool loadTexture(GLuint texID, GLuint texUnit, GLuint program, const char* uniformName)
{
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_2D, texID);
	
	GLuint uniformLocation = glGetUniformLocation(program, uniformName);
	glUniform1i(uniformLocation, 0);
		
	return !CheckGLErrors("loadTexture");
}



//Initialization
void initGL()
{
	//Only call these once - don't call again every time you change geometry
	generateIDs();		//Create VertexArrayObjects and Vertex Buffer Objects and store their handles
	initShader();		//Create shader and store program ID

	initVAO();			//Describe setup of Vertex Array Objects and Vertex Buffer Object
}

//Draws buffers to screen
void render(mat4 modelviewMatrix, int startIndex, int numElements, bool diffuseEnabled, bool normalMapEnabled) {
	//Don't need to call these on every draw, so long as they don't change
	glUseProgram(shader[SHADER::LINE]);		//Use LINE program
	glBindVertexArray(vao[VAO::LINES]);		//Use the LINES vertex array

	mat4 camMatrix = cam.getMatrix();

	glUniformMatrix4fv(glGetUniformLocation(shader[SHADER::LINE], "cameraMatrix"),
						1,
						false,
						&camMatrix[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(shader[SHADER::LINE], "perspectiveMatrix"),
						1,
						false,
						&perspectiveMatrix[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(shader[SHADER::LINE], "modelviewMatrix"),
						1,
						false,
						&modelviewMatrix[0][0]);

	glUniform3f(glGetUniformLocation(shader[SHADER::LINE], "cameraPosition"),
						cam.pos.x, cam.pos.y, cam.pos.z);

	glUniform1i(glGetUniformLocation(shader[SHADER::LINE], "diffuseEnabled"),
						diffuseEnabled ? 1 : 0);

	glUniform1i(glGetUniformLocation(shader[SHADER::LINE], "normalMapEnabled"),
						normalMapEnabled ? 1 : 0);

	glDrawArrays(
			GL_TRIANGLES,		//What shape we're drawing	- GL_TRIANGLES, GL_LINES, GL_POINTS, GL_QUADS, GL_TRIANGLE_STRIP
			startIndex,						//Starting index
			numElements		//How many vertices
			);

	CheckGLErrors("render");
}



void renderAndUpdateSphericalBody(SphericalBody *body, mat4 translationMatrix, float deltaTime) {
	translationMatrix = body->getTranslationMatrix() * translationMatrix;

	bool normalMapEnabled = false;
	if(body->normalTexId > -1 && !globalNormalMapOverride) {
		loadTexture(body->normalTexId, GL_TEXTURE0, shader[SHADER::LINE], "normalMap");
		normalMapEnabled = true;
	}
	loadTexture(body->texId, GL_TEXTURE0, shader[SHADER::LINE], "tex");
	loadBuffer(body->points, body->uvs);
	render(translationMatrix*body->getRotationMatrix(), 0, body->points.size(), body->diffuseEnabled, normalMapEnabled);

	for(int i = 0; i < (body->orbiters).size(); i++) {
		SphericalBody *orbiter = (body->orbiters)[i];
		renderAndUpdateSphericalBody(orbiter, translationMatrix, deltaTime);
	}

	body->incrementSpherical(deltaTime);
	body->incrementRot(deltaTime);
}


void attachCameraToSphericalBody(SphericalBody *body) {
	mat4 translationMatrix(1.f);
	SphericalBody *orbitee = body->orbitee;
	while(orbitee != NULL) {
		translationMatrix *= orbitee->getTranslationMatrix();
		orbitee = orbitee->orbitee;
	}

	vec4 pos = translationMatrix * vec4(body->pos, 1.f);
	cam.setOffset(vec3(pos.x, pos.y, pos.z));
}



// ==========================================================================
// PROGRAM ENTRY POINT

int main(int argc, char *argv[])
{   
	// initialize the GLFW windowing system
	if (!glfwInit()) {
		cout << "ERROR: GLFW failed to initilize, TERMINATING" << endl;
		return -1;
	}
	glfwSetErrorCallback(ErrorCallback);

	// attempt to create a window with an OpenGL 4.1 core profile context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(764, 764, "CPSC 453 OpenGL Boilerplate", 0, 0);
	if (!window) {
		cout << "Program failed to create GLFW window, TERMINATING" << endl;
		glfwTerminate();
		return -1;
	}


	// set keyboard callback function and make our context current (active)
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, mousePosCallback);
	glfwSetScrollCallback(window, scrollWheelCallback);
	glfwSetWindowSizeCallback(window, resizeCallback);

	glfwMakeContextCurrent(window);

	#ifdef _WIN32
	// Intialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cout << "glew init failed" << endl;
		return -1;
	}
	#endif


	// query and print out information about our OpenGL environment
	QueryGLVersion();

	initGL();
	glEnable(GL_DEPTH_TEST);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	SphericalBody skybox(
		NULL,											// orbitee
		10000.f,										// radius (km)
		radians(45.f),									// axial tilt (radians)
		1200.f,											// rotation period (days)
		0.f,											// distance from orbitee (km)
		0.f,											// orbital inclination (radians)
		0.f,											// orbit period (days)
		createTexture("images/stars_milkyway.jpg"),		// texture ID
		-1,												// normal map ID
		false,											// diffuse enabled
		false);											// scaling enabled

	SphericalBody sun(
		&skybox,										// orbitee
		696000.f,										// radius (km)
		radians(7.25f),									// axial tilt (radians)
		25.38,											// rotation period (days)
		0.f,											// distance from orbitee (km)
		0.f,											// orbital inclination (radians)
		0.f,											// orbit period (days)
		createTexture("images/sun.jpg"),				// texture ID
		-1,												// normal map ID
		false,											// diffuse enabled
		true);											// scaling enabled

	SphericalBody mercury(
		&sun,											// orbitee
		2439.64f,										// radius (km)
		radians(0.0f),									// axial tilt (radians)
		-58.646225f,									// rotation period (days)
		57909175.f,										// distance from orbitee (km)
		radians(7.f),									// orbital inclination (radians)
		0.2408467f*365.25,								// orbit period (days)
		createTexture("images/mercury.jpg"),			// texture ID
		-1,												// normal map ID
		true,											// diffuse enabled
		true);											// scaling enabled

	SphericalBody venus(
		&sun,											// orbitee
		6051.59f,										// radius (km)
		radians(177.3),									// axial tilt (radians)
		243.0187,										// rotation period (days)
		108208930.f,									// distance from orbitee (km)
		radians(3.39f),									// orbital inclination (radians)
		0.61519726f*365.25,								// orbit period (days)
		createTexture("images/venus.jpg"),				// texture ID
		-1,												// normal map ID
		true,											// diffuse enabled
		true);											// scaling enabled

	SphericalBody earth(
		&sun,											// orbitee
		6378.1f,										// radius (km)
		radians(23.44f),								// axial tilt (radians)
		-1.f,											// rotation period (days)
		149597890.f,									// distance from orbitee (km)
		0.f,											// orbital inclination (radians)
		365.25f,										// orbit period (days)
		createTexture("images/earth.jpg"),				// texture ID
		createTexture("images/earth_normal.png"),		// normal map ID
		true,											// diffuse enabled
		true);											// scaling enabled
	SphericalBody moon(
		&earth,											// orbitee
		1737.1f,										// radius (km)
		radians(6.68f),									// axial tilt (radians)
		-27.321582f,									// rotation period (days)
		384399.f,										// distance from orbitee (km)
		radians(23.435f),								// orbital inclination (radians)
		27.32158f,										// orbit period (days)
		createTexture("images/moon.jpg"),				// texture ID
		-1,												// normal map ID
		true,											// diffuse enabled
		true);											// scaling enabled

	SphericalBody mars(
		&sun,											// orbitee
		3397.f,											// radius (km)
		radians(25.19f),								// axial tilt (radians)
		-1.02595675,									// rotation period (days)
		227936640.f,									// distance from orbitee (km)
		radians(1.85f),									// orbital inclination (radians)
		1.8808476f*365.25f,								// orbit period (days)
		createTexture("images/mars.jpg"),				// texture ID
		-1,												// normal map ID
		true,											// diffuse enabled
		true);											// scaling enabled

	SphericalBody jupiter(
		&sun,											// orbitee
		71492.68f,										// radius (km)
		radians(3.12f),									// axial tilt (radians)
		-0.41354f,										// rotation period (days)
		778412010.f,									// distance from orbitee (km)
		radians(1.31f),									// orbital inclination (radians)
		11.862615f*365.25,								// orbit period (days)
		createTexture("images/jupiter.jpg"),			// texture ID
		-1,												// normal map ID
		true,											// diffuse enabled
		true);											// scaling enabled
	SphericalBody io(
		&jupiter,										// orbitee
		1815.f,											// radius (km)
		radians(0.f),									// axial tilt (radians)
		-1.7691378f,									// rotation period (days)
		421600.f,										// distance from orbitee (km)
		radians(0.f),									// orbital inclination (radians)
		1.769138f,										// orbit period (days)
		createTexture("images/io.jpg"),					// texture ID
		-1,												// normal map ID
		true,											// diffuse enabled
		true);											// scaling enabled
	SphericalBody europa(
		&jupiter,										// orbitee
		1569.f,											// radius (km)
		radians(0.f),									// axial tilt (radians)
		-3.551181f,										// rotation period (days)
		670900.f,										// distance from orbitee (km)
		radians(0.47f),									// orbital inclination (radians)
		3.551181f,										// orbit period (days)
		createTexture("images/europa.jpg"),				// texture ID
		-1,												// normal map ID
		true,											// diffuse enabled
		true);											// scaling enabled
	SphericalBody ganymede(
		&jupiter,										// orbitee
		2634.1f,										// radius (km)
		radians(0.165f),								// axial tilt (radians)
		-7.154553f,										// rotation period (days)
		1070400.f,										// distance from orbitee (km)
		radians(1.85f),									// orbital inclination (radians)
		7.154553f,										// orbit period (days)
		createTexture("images/ganymede.png"),			// texture ID
		-1,												// normal map ID
		true,											// diffuse enabled
		true);											// scaling enabled
	SphericalBody callisto(
		&jupiter,										// orbitee
		2410.3f,										// radius (km)
		radians(0.f),									// axial tilt (radians)
		-16.68902f,										// rotation period (days)
		1882700.f,										// distance from orbitee (km)
		radians(0.2f),									// orbital inclination (radians)
		16.68902f,										// orbit period (days)
		createTexture("images/callisto.jpg"),			// texture ID
		-1,												// normal map ID
		true,											// diffuse enabled
		true);											// scaling enabled

	SphericalBody saturn(
		&sun,											// orbitee
		60267.14f,										// radius (km)
		radians(26.73f),								// axial tilt (radians)
		-0.44401,										// rotation period (days)
		1426725400.f,									// distance from orbitee (km)
		radians(2.48f),									// orbital inclination (radians)
		29.447498*365.25,								// orbit period (days)
		createTexture("images/saturn.jpg"),				// texture ID
		-1,												// normal map ID
		true,											// diffuse enabled
		true);											// scaling enabled
	SphericalBody rhea(
		&saturn,										// orbitee
		764.3f,											// radius (km)
		radians(0.f),									// axial tilt (radians)
		-4.518212f,										// rotation period (days)
		527108.f,										// distance from orbitee (km)
		radians(0.345f),								// orbital inclination (radians)
		4.518212f,										// orbit period (days)
		createTexture("images/rhea.jpg"),				// texture ID
		-1,												// normal map ID
		true,											// diffuse enabled
		true);											// scaling enabled
	SphericalBody titan(
		&saturn,										// orbitee
		2576.f,											// radius (km)
		radians(0.f),									// axial tilt (radians)
		-15.945f,										// rotation period (days)
		1221870.f,										// distance from orbitee (km)
		radians(0.33f),									// orbital inclination (radians)
		15.945f,										// orbit period (days)
		createTexture("images/titan.png"),				// texture ID
		-1,												// normal map ID
		true,											// diffuse enabled
		true);											// scaling enabled

	SphericalBody uranus(
		&sun,											// orbitee
		25557.25f,										// radius (km)
		radians(97.86f),								// axial tilt (radians)
		0.71833f,										// rotation period (days)
		2870972200.f,									// distance from orbitee (km)
		radians(0.76f),									// orbital inclination (radians)
		84.016846*365.25,								// orbit period (days)
		createTexture("images/uranus.jpg"),				// texture ID
		-1,												// normal map ID
		true,											// diffuse enabled
		true);											// scaling enabled

	SphericalBody neptune(
		&sun,											// orbitee
		24766.36f,										// radius (km)
		radians(29.58f),								// axial tilt (radians)
		-0.67125f,										// rotation period (days)
		4498252900.f,									// distance from orbitee (km)
		radians(1.77f),									// orbital inclination (radians)
		164.79132*365.25,								// orbit period (days)
		createTexture("images/neptune.jpg"),			// texture ID
		-1,												// normal map ID
		true,											// diffuse enabled
		true);											// scaling enabled
	
	setSelectedSphericalBody(&earth);

	cam = SphericalCamera(vec3(0.f), vec3(PI/2.f, 0.f, 20.f));

	//float fovy, float aspect, float zNear, float zFar
	perspectiveMatrix = perspective(radians(90.f), 1.f, 0.1f, 20000.f);
	

	float speed = 1.f;

	double globalTime = glfwGetTime();

	// run an event-triggered main loop
	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - globalTime;
		globalTime = currentTime;

		glClearColor(0.1f, 0.1f, 0.1f, 0.f);		//Color to clear the screen with (R, G, B, Alpha)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//Clear color and depth buffers (Haven't covered yet)

		renderAndUpdateSphericalBody(&skybox, mat4(1.f), deltaTime * speed);
		attachCameraToSphericalBody(currentSelected);

		if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			speed += 0.01f;
		} if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			speed -= 0.01f;
		}

		// scene is rendered to the back buffer, so swap to front for display
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// clean up allocated resources before exit
   deleteIDs();
	glfwDestroyWindow(window);
   glfwTerminate();

   return 0;
}

// ==========================================================================
// SUPPORT FUNCTION DEFINITIONS

// --------------------------------------------------------------------------
// OpenGL utility functions

void QueryGLVersion()
{
	// query opengl version and renderer information
	string version  = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	string glslver  = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

	cout << "OpenGL [ " << version << " ] "
		 << "with GLSL [ " << glslver << " ] "
		 << "on renderer [ " << renderer << " ]" << endl;
}

bool CheckGLErrors(string location)
{
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
	{
		cout << "OpenGL ERROR:  ";
		switch (flag) {
		case GL_INVALID_ENUM:
			cout << location << ": " << "GL_INVALID_ENUM" << endl; break;
		case GL_INVALID_VALUE:
			cout << location << ": " << "GL_INVALID_VALUE" << endl; break;
		case GL_INVALID_OPERATION:
			cout << location << ": " << "GL_INVALID_OPERATION" << endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			cout << location << ": " << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl; break;
		case GL_OUT_OF_MEMORY:
			cout << location << ": " << "GL_OUT_OF_MEMORY" << endl; break;
		default:
			cout << "[unknown error code]" << endl;
		}
		error = true;
	}
	return error;
}

// --------------------------------------------------------------------------
// OpenGL shader support functions

// reads a text file with the given name into a string
string LoadSource(const string &filename)
{
	string source;

	ifstream input(filename.c_str());
	if (input) {
		copy(istreambuf_iterator<char>(input),
			 istreambuf_iterator<char>(),
			 back_inserter(source));
		input.close();
	}
	else {
		cout << "ERROR: Could not load shader source from file "
			 << filename << endl;
	}

	return source;
}

// creates and returns a shader object compiled from the given source
GLuint CompileShader(GLenum shaderType, const string &source)
{
	// allocate shader object name
	GLuint shaderObject = glCreateShader(shaderType);

	// try compiling the source as a shader of the given type
	const GLchar *source_ptr = source.c_str();
	glShaderSource(shaderObject, 1, &source_ptr, 0);
	glCompileShader(shaderObject);

	// retrieve compile status
	GLint status;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetShaderInfoLog(shaderObject, info.length(), &length, &info[0]);
		cout << "ERROR compiling shader:" << endl << endl;
		cout << source << endl;
		cout << info << endl;
	}

	return shaderObject;
}

// creates and returns a program object linked from vertex and fragment shaders
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
	// allocate program object name
	GLuint programObject = glCreateProgram();

	// attach provided shader objects to this program
	if (vertexShader)   glAttachShader(programObject, vertexShader);
	if (fragmentShader) glAttachShader(programObject, fragmentShader);

	// try linking the program with given attachments
	glLinkProgram(programObject);

	// retrieve link status
	GLint status;
	glGetProgramiv(programObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetProgramInfoLog(programObject, info.length(), &length, &info[0]);
		cout << "ERROR linking shader program:" << endl;
		cout << info << endl;
	}

	return programObject;
}


// ==========================================================================
