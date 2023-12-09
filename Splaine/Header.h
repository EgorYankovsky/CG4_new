#pragma once
#include <windows.h>
#include <algorithm>
#include <gl/glut.h>
#include <vector>
#include <iostream>
#include <sstream>
#pragma comment( linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

using namespace std;

GLint width, height;
GLuint mesh;
double zoom = 1.0;
int ind = 60, localShift = 3;
bool moving = false, canMake = false;
vector<int> Shift = { 0, 0 };
vector<int> Shift2 = { 0, 0 };
string message = "Add points to make spline.";

struct point
{
	double wx = 0, wy = 0;
	point(double _x = 0.0, double _y = 0.0, bool isScreen = true)
	{
		if (isScreen)
		{
			wx = (_x - double(Shift[0]) - width / 2.0) / double(ind) *
				zoom;
			wy = (_y - double(Shift[1]) - height / 2.0) / double(ind) *
				zoom;
		}
		else
		{
			wx = _x;
			wy = _y;
		}
	}
};

bool operator==(point a, point b)
{
	return a.wx == b.wx && a.wy == b.wy;
}

bool operator!=(point a, point b)
{
	return a.wx != b.wx || a.wy != b.wy;
}

point* p;
point mouse;
vector <point> checkPoints;
vector <point> splinePoints;

void loadTexture()
{
	GLuint type, texWidth, texHeight;
	unsigned char* pixels;
	LPCWSTR FileName = L"background1.bmp";
	HBITMAP hbmp = (HBITMAP)LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0,
					LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	BITMAP bmp;
	GetObject(hbmp, sizeof(bmp), &bmp);
	texWidth = bmp.bmWidth;
	texHeight = bmp.bmHeight;
	pixels = (unsigned char*)bmp.bmBits;
	type = GL_BGR_EXT;
	glGenTextures(1, &mesh);
	glBindTexture(GL_TEXTURE_2D, mesh);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texWidth, texHeight, GL_RGB,
	GL_UNSIGNED_BYTE, pixels);
}

void Mesh()
{
	glBindTexture(GL_TEXTURE_2D, mesh);
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2d(-5000, -5000);
	glTexCoord2d(0, 50); glVertex2d(-5000, 5000);
	glTexCoord2d(50, 50); glVertex2d(5000, 5000);
	glTexCoord2d(50, 0); glVertex2d(5000, -5000);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void Axis()
{
	int len = 5;
	int cenOx = width / 2.0;
	int cenOy = height / 2.0;
	int lineShiftX = 14;
	int sepShiftX = 2;
	int lineShiftY = 7;
	int sepShiftY = 3;
	
	for (int i = 0; i < 100; i++)
	{
		glLineWidth(1);
		glColor3ub(0, 0, 0);
		glBegin(GL_LINES);
		glVertex2i(cenOx + i * ind, cenOy - len);
		glVertex2i(cenOx + i * ind, cenOy + len);
		glEnd();
		glBegin(GL_LINES);
		glVertex2i(cenOx - i * ind, cenOy - len);
		glVertex2i(cenOx - i * ind, cenOy + len);
		glEnd();
		glBegin(GL_LINES);
		glVertex2i(cenOx - len, cenOy + i * ind);
		glVertex2i(cenOx + len, cenOy + i * ind);
		glEnd();
		glBegin(GL_LINES);
		glVertex2i(cenOx - len, cenOy - i * ind);
		glVertex2i(cenOx + len, cenOy - i * ind);
		glEnd();
		glColor3ub(0, 0, 0);
		char dest[10];
		double num = (double)i * zoom;
		sprintf_s(dest, "%0.2lf", num);
		string out1 = string(dest);
		string out2 = "-" + string(dest);
			glRasterPos2i(cenOx + i * ind + sepShiftX, cenOy - lineShiftX);
		for (auto a : out1)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, a);
		if (i != 0)
		{
			glRasterPos2i(cenOx - i * ind + sepShiftX, cenOy -
				lineShiftX);
			for (auto a : out2)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, a);
			glRasterPos2i(cenOx + lineShiftY, cenOy + i * ind -
				sepShiftY);
			for (auto a : out1)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, a);
			glRasterPos2i(cenOx + lineShiftY, cenOy - i * ind -
				sepShiftY);
			for (auto a : out2)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, a);
		}
	}
	
	glLineWidth(2);
	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2i(-5000, cenOy);
	glVertex2i(5000, cenOy);
	glEnd();
	glBegin(GL_LINES);
	glVertex2i(cenOx, -5000);
	glVertex2i(cenOx, 5000);
	glEnd();
}

void printCoord()
{
	char destX[10];
	char destY[10];
	double sx = mouse.wx / zoom * double(ind) + double(Shift[0]) + 0.5 * width,
		   sy = mouse.wy / zoom * double(ind) + double(Shift[1]) + 0.5 * height;
	sprintf_s(destX, "%0.2lf", mouse.wx);
	sprintf_s(destY, "%0.2lf", mouse.wy);
	string outX = "x = " + string(destX);
	string outY = "y = " + string(destY);
	glColor3ub(0, 0, 0);
	glRasterPos2i(sx + 12, sy + 2);
	for (auto a : outX)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, a);
	glRasterPos2i(sx + 12, sy - 10);
	for (auto a : outY)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, a);
	glutPostRedisplay();
}

void Message()
{
	glColor3ub(0, 0, 0);
	glRasterPos2i(10, height - 24);
	for (auto mes : message)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, mes);
	glutPostRedisplay();
}

bool comparison(point a, point b)
{
	return a.wx < b.wx;
}

void MineRemakeSpline()
{
	int point_index = 0;
	splinePoints.clear();
	splinePoints.push_back(checkPoints[0]);
	for (int i = 0; i < checkPoints.size(); i += i == 0 ? 4 : 3)
	{
		int step = i == 0 ? 4 : 3;

		if (i + step > checkPoints.size()) break;

		double h = (checkPoints[i + step - 1].wx - checkPoints[i - (i == 0 ? 0 : 1)].wx);
		int n = (200 + int(20 * h) + int(10 / zoom));
		double hi = (checkPoints[i + step - 1].wx - checkPoints[i - (i == 0 ? 0 : 1)].wx) / (2.0 * n);

		double x0 = checkPoints[i - (i == 0 ? 0 : 1)].wx;
		double x1 = checkPoints[i + 1 - (i == 0 ? 0 : 1)].wx;
		double x2 = checkPoints[i + 2 - (i == 0 ? 0 : 1)].wx;
		double x3 = checkPoints[i + 3 - (i == 0 ? 0 : 1)].wx;

		double y0 = checkPoints[i - (i == 0 ? 0 : 1)].wy;
		double y1 = checkPoints[i + 1 - (i == 0 ? 0 : 1)].wy;
		double y2 = checkPoints[i + 2 - (i == 0 ? 0 : 1)].wy;
		double y3 = checkPoints[i + 3 - (i == 0 ? 0 : 1)].wy;

		for (int j = 0; j < 2 * n - 1; j++)
		{
			double x = splinePoints[j + point_index].wx + hi;

			double y = (x - x1) / (x0 - x1) * (x - x2) / (x0 - x2) * (x - x3) / (x0 - x3) * y0 +
				(x - x0) / (x1 - x0) * (x - x2) / (x1 - x2) * (x - x3) / (x1 - x3) * y1 +
				(x - x0) / (x2 - x0) * (x - x1) / (x2 - x1) * (x - x3) / (x2 - x3) * y2 +
				(x - x0) / (x3 - x0) * (x - x1) / (x3 - x1) * (x - x2) / (x3 - x2) * y3;

			splinePoints.push_back(point(x, y, false));
		}
		splinePoints.push_back(checkPoints[i + (i == 0 ? 3 : 2)]); 
		point_index = splinePoints.size() - 1;
	}
}

void MineAddPoint(double x, double y)
{
	point pnt = point(x, y);
	checkPoints.push_back(pnt);
	sort(checkPoints.begin(), checkPoints.end(), comparison);
	MineRemakeSpline();
}