// Triangle01.cpp : Defines the entry point for the console application.
//

// Specifique a Windows
#if _WIN32
#include <Windows.h>
#define FREEGLUT_LIB_PRAGMAS 0
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")
#endif

// Entete OpenGL 
#define GLEW_STATIC 1
#include <GL/glew.h>

// FreeGLUT
#include "GL/freeglut.h"

// STB
#define STB_IMAGE_IMPLEMENTATION
//#include "stb-master/stb_image.h"

#include <cstdio>
#include <cmath>

#include "EsgiShader.h"
#include "cube.h"
#include "Army.hpp"
#include <cstdlib>
#include <time.h>
#include <iostream>
#include "training.hpp"
#include "fstream"


#include <vector>

EsgiShader cubeShader;
EsgiShader gridShader;

int previousTime = 0;

GLuint cubeVBO, cubeIBO, textureObj;

GLuint gridVBO, gridVAO, gridEBO;


const GLuint GRID_W = 50, GRID_H = 50;
std::vector<GLfloat> verticesGrid = std::vector<GLfloat>();
std::vector<GLuint> indicesGrid = std::vector<GLuint>();

std::vector<float> cubePosition = std::vector<float>();

void beginBattle(){

	std::srand(static_cast<unsigned int>(time(nullptr)));
	std::vector<std::unique_ptr<Army> > champions;
	try
	{
		std::ifstream in("Army_10_100.save");
		Army army = Army::load(in);
		std::cout << army << std::endl;
		champions.push_back(std::unique_ptr<Army>(new Army(army)));
	}
	catch (...)
	{
	}

	try
	{
		std::unique_ptr<Army> army = train(10, 100, 20, 10, 100, champions);
		std::cout << *army << std::endl;
		std::ofstream out("Army_10_100.save");
		army->save(out);
		out.flush();
		out.close();
	}
	catch (std::invalid_argument& e)
	{
		std::cout << "toto" << std::endl;
	}
}


void calculateGrid()
{
	verticesGrid.clear();
	indicesGrid.clear();

	float incrementW = 0.1f;
	float incrementH = 0.1f;

	//float increment = GRID_H >= GRID_W?

	//float incrementW = 3.f / GRID_W;
	//float incrementH = 3.f / GRID_W;

	for (float i = 0.f; i <= GRID_W; i += 1.f)
	{
		float yIncr = 0;
		for (float j = 0.f; j <= GRID_H; j += 1.f)
		{
			verticesGrid.push_back(j * incrementW);
			verticesGrid.push_back(i * incrementH);
			verticesGrid.push_back(0);

			if (j < GRID_H / 2)
				yIncr += 0.1f;
			else
				yIncr -= 0.1f;
		}
	}

	for (float i = 0.f; i < GRID_W; i += 1.f)
	{
		for (float j = 0.f; j < GRID_H; j += 1.f)
		{
			indicesGrid.push_back(i * (GRID_H + 1) + j);
			indicesGrid.push_back((i + 1) * (GRID_H + 1) + (j + 1));
			indicesGrid.push_back((i + 1) * (GRID_H + 1) + j);

			indicesGrid.push_back(i * (GRID_H + 1) + j);
			indicesGrid.push_back(i * (GRID_H + 1) + (j + 1));
			indicesGrid.push_back((i + 1) * (GRID_H + 1) + j + 1);
		}
	}

}

void Initialize()
{


	GLenum error = glewInit();
	if (error != GL_NO_ERROR) {
		// TODO
	}

	std::cout << "beginBattle" << std::endl;
	//beginBattle();
	std::cout << "endBattle" << std::endl;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);

	cubeShader.LoadVertexShader("cube.vs");
	cubeShader.LoadFragmentShader("cube.fs");
	cubeShader.Create();

	gridShader.LoadVertexShader("grid.vs");
	gridShader.LoadFragmentShader("grid.fs");
	gridShader.Create();

	glGenBuffers(1, &cubeVBO);
	glGenBuffers(1, &cubeIBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, g_cubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6 * 2 * 3, g_cubeIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	previousTime = glutGet(GLUT_ELAPSED_TIME);

	//Team1
	cubePosition.push_back(2.f);
	cubePosition.push_back(1.0f);
	cubePosition.push_back(-0.5f);


	cubePosition.push_back(5.f);
	cubePosition.push_back(1.0f);
	cubePosition.push_back(-0.5f);


	cubePosition.push_back(8.f);
	cubePosition.push_back(1.0f);
	cubePosition.push_back(-0.5f);


	cubePosition.push_back(3.5f);
	cubePosition.push_back(3.0f);
	cubePosition.push_back(-0.5f);


	cubePosition.push_back(6.5f);
	cubePosition.push_back(3.0f);
	cubePosition.push_back(-0.5f);

	////Team2
	cubePosition.push_back(2.f);
	cubePosition.push_back(8.5f);
	cubePosition.push_back(-0.5f);


	cubePosition.push_back(5.f);
	cubePosition.push_back(8.5f);
	cubePosition.push_back(-0.5f);


	cubePosition.push_back(8.f);
	cubePosition.push_back(8.5f);
	cubePosition.push_back(-0.5f);


	cubePosition.push_back(3.5f);
	cubePosition.push_back(6.f);
	cubePosition.push_back(-0.5f);


	cubePosition.push_back(6.5f);
	cubePosition.push_back(6.f);
	cubePosition.push_back(-0.5f);

}

void Terminate()
{
	glDeleteTextures(1, &textureObj);
	glDeleteBuffers(1, &cubeIBO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &gridEBO);
	glDeleteBuffers(1, &gridVBO);
	glDeleteBuffers(1, &gridVAO);

	cubeShader.Destroy();
	gridShader.Destroy();
}


#pragma region matrix transformation

void MultiplyMatrix(float* matrixA, float* matrixB, float* result) {

	memset(result, 0, sizeof(float) * 16);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float sum = 0.0;
			for (int k = 0; k < 4; k++)
				sum = sum + matrixA[i * 4 + k] * matrixB[k * 4 + j];
			result[i * 4 + j] = sum;
		}
	}
}

void Identity(float *matrix)
{
	memset(matrix, 0, sizeof(float) * 16);
	matrix[0] = 1.0f; matrix[5] = 1.0f; matrix[10] = 1.0f; matrix[15] = 1.0f;
}

void Orthographic(float *matrix, float L, float R, float T, float B, float N, float F)
{
	memset(matrix, 0, sizeof(float) * 16);
	matrix[0] = 2.f / (R-L); 
	matrix[5] = 2.f / (T-B); 
	matrix[10] = -2.f / (F-N);
	matrix[12] = -(R+L) / (R-L);
	matrix[13] = -(T+B) / (T-B);
	matrix[14] = -(F+N) / (F-N);
	matrix[15] = 1.f;
}

void Perspective(float *matrix, float FOV, float width, float height, float N, float F)
{
	memset(matrix, 0, sizeof(float) * 16);
	float aspect = width / height;

	float xymax = N * tan(FOV * (3.141592f / 180.f) * 0.5f);
	float ymin = -xymax;
	float xmin = -xymax;
	width = xymax - xmin;
	height = xymax - ymin;
	float depth = F - N;
	float q = -(F+N) / depth;
	float qn = -2.0f * (F*N) / depth;
	
	float w = 2.0f * N / width;
	w = w / aspect;
	float h = 2.0f * N / height;

	matrix[0] = w;
	matrix[5] = h;
	matrix[10] = q;
	matrix[11] = -1.f;
	matrix[14] = qn;
}

void Translate(float *matrix, float tx, float ty, float tz = 0.f)
{
	memset(matrix, 0, sizeof(float) * 16);
	matrix[0] = 1.f; 
	matrix[5] = 1.f; 
	matrix[10] = 1.f;
	matrix[12] = tx;
	matrix[13] = ty;
	matrix[14] = tz;
	matrix[15] = 1.f;
}

void Rotate(float *matrix, float deg, float x, float y, float z)
{

	float matrixA[16];
	memcpy(matrixA, matrix, sizeof(float) * 16);

	float matrixB[16];
	memset(matrixB, 0, sizeof(float) * 16);

	//memset(matrix, 0, sizeof(float) * 16);
	matrixB[0] = 1.f;
	matrixB[5] = cos(deg);
	matrixB[6] = -sin(deg);
	matrixB[9] = sin(deg);
	matrixB[10] = cos(deg);
	matrixB[15] = 1.f;

	MultiplyMatrix(matrixA, matrixB, matrix);
}

void scale(float *matrix, float x, float y, float z)
{
	float matrixA[16];
	memcpy(matrixA, matrix, sizeof(float) * 16);

	float matrixB[16];
	memset(matrixB, 0, sizeof(float) * 16);

	matrixB[0] = x;
	matrixB[5] = y;
	matrixB[10] = z;
	matrixB[15] = 1.f;

	MultiplyMatrix(matrixA, matrixB, matrix);
}


#pragma endregion

void RenderGrid(float w, float h)
{

	calculateGrid();
	gridShader.Bind();

	GLuint program = gridShader.GetProgram();
	glUseProgram(program);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glGenVertexArrays(1, &gridVAO);
	glGenBuffers(1, &gridVBO);
	glGenBuffers(1, &gridEBO);

	glBindVertexArray(gridVAO);

	glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
	glBufferData(GL_ARRAY_BUFFER, verticesGrid.size() * sizeof(float), &verticesGrid.front(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesGrid.size() * sizeof(float), &indicesGrid.front(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); // Unbind VAO

	cubeShader.Bind();

	float projection[16];
	Perspective(projection, 45.f, w, h, 0.1f, 1000.f);
	GLint projLocation = glGetUniformLocation(program, "u_projectionMatrix");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, projection);

	float viewTransform[16];
	Identity(viewTransform);
	viewTransform[12] = -2.5f;
	viewTransform[13] =  -1.f;
	viewTransform[14] = -5.0f;
	GLint viewLocation = glGetUniformLocation(program, "u_viewMatrix");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewTransform);

	float worldTransform[16];
	Identity(worldTransform);
	Translate(worldTransform, 0.f, 0.0f, 0.f);
	Rotate(worldTransform, 1, 1, 0, 0);
	GLint worldLocation = glGetUniformLocation(program, "u_worldMatrix");
	glUniformMatrix4fv(worldLocation, 1, GL_FALSE, worldTransform);

	glBindVertexArray(gridVAO);
	glDrawElements(GL_TRIANGLES, indicesGrid.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	gridShader.Unbind();
}

void RenderCube(float w, float h)
{
	GLuint program = cubeShader.GetProgram();
	glUseProgram(program);

	// passage des attributs de sommet au shader

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	GLint positionLocation = glGetAttribLocation(program, "a_position");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	// variables uniformes (constantes) durant le rendu de la primitive

	float projection[16];
	Perspective(projection, 45.f, w, h, 0.1f, 1000.f);
	GLint projLocation = glGetUniformLocation(program, "u_projectionMatrix");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, projection);

	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int delta = currentTime - previousTime;
	previousTime = currentTime;
	static float time = 1.f;
	time += delta/1000.f;
	GLint timeLocation = glGetUniformLocation(program, "u_time");
	glUniform1f(timeLocation, time);

	float viewTransform[16];
	Identity(viewTransform);
	//viewTransform[12] = 0.5f;
	viewTransform[12] = -2.5f;
	viewTransform[13] = -1.f;
	viewTransform[14] = -5.0f;
	GLint viewLocation = glGetUniformLocation(program, "u_viewMatrix");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewTransform);


	for (int i = 0; i < cubePosition.size(); i+=3){

		float worldTransform[16];
		Identity(worldTransform);
		Translate(worldTransform, cubePosition[i], cubePosition[i + 1] /* sin(time)*/, (cubePosition[i + 2] + 1) /* (sin(time) * 2*/);
		Rotate(worldTransform, 1, 1, 0, 0);
		scale(worldTransform, 0.5, 0.5, 0.5);
		GLint worldLocation = glGetUniformLocation(program, "u_worldMatrix");
		glUniformMatrix4fv(worldLocation, 1, GL_FALSE, worldTransform);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
		glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_SHORT, 0);
	}

	//float worldTransform2[16];
	//Identity(worldTransform2);
	//Translate(worldTransform2, 2.f, 0.0f, 0.0f);
	//Rotate(worldTransform2, 1, 1, 0, 0);
	//scale(worldTransform2, 0.1, 0.1, 0.1);
	//GLint worldLocation2 = glGetUniformLocation(program, "u_worldMatrix");
	//glUniformMatrix4fv(worldLocation2, 1, GL_FALSE, worldTransform2);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
	//glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_SHORT, 0);

	glUseProgram(0);

	cubeShader.Unbind();
}


void Render()
{
	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glClearDepth(1.F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float w = (float)glutGet(GLUT_WINDOW_WIDTH);
	float h = (float)glutGet(GLUT_WINDOW_HEIGHT);

	RenderGrid(w, h);

	RenderCube(w, h);
	

	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Cube Basique");

#ifdef FREEGLUT
	// Note: glutSetOption n'est disponible qu'avec freeGLUT
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
				  GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif

	Initialize();

	glutDisplayFunc(Render);

	glutMainLoop();

	Terminate();

	return 0;
}

