#include "header.h"

void Reshape(GLint w, GLint h)
{
	width = w;
	height = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluOrtho2D(0.0, width, 0.0, height);
}

void Mouse(int button, int state, int x, int y)
{
	if (state != GLUT_DOWN) return;
	if (button == GLUT_RIGHT_BUTTON)
	{
		Shift2[0] = x;
		Shift2[1] = height - y;
		moving = !moving;
	}
	if (button == GLUT_LEFT_BUTTON)
		MineAddPoint(x, height - y);
}

void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);

	//нарисовать сетку
	glPushMatrix();
	glTranslatef((GLfloat)Shift[0], (GLfloat)Shift[1], 0);
	Mesh();
	glPopMatrix();
	//нарисовать координатные оси
	glPushMatrix();
	glTranslatef((GLfloat)Shift[0], (GLfloat)Shift[1], 0);
	Axis();
	glPopMatrix();
	glPointSize(5);
	glBegin(GL_POINTS);
	
	for (int i = 0; i < checkPoints.size(); i++)
	{
		glColor3ub(0, 0, 0);
		glPointSize(7);
		p = &checkPoints[i];
		glVertex2f(p->wx / zoom * double(ind) + Shift[0] + 0.5 * width,
				   p->wy / zoom * double(ind) + double(Shift[1]) + 0.5 * height);
	}
	glEnd();
	glLineWidth(1);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < splinePoints.size(); i++)
	{
		glColor3ub(53, 197, 22);
		p = &splinePoints[i];
		glVertex2f(p->wx / zoom * double(ind) + Shift[0] + 0.5 * width,
				   p->wy / zoom * double(ind) + double(Shift[1]) + 0.5 * height);
	}
	glEnd();
	printCoord();

	if (!canMake) Message();
	
	glutSwapBuffers();
}

void Keyboard(unsigned char key, int x, int y)
{
	if (key == 43 && zoom > 0.25) zoom -= 0.25;
	if (key == 45) zoom += 0.25;
	glutPostRedisplay();
}

void Move(int x, int y)
{
	mouse.wx = (x - double(Shift[0]) - width / 2.0) / double(ind) * zoom;
	mouse.wy = (height - y - double(Shift[1]) - height / 2.0) / double(ind) * zoom;

	if (moving)
	{
		Shift[0] += Shift2[0] - x;
		Shift[1] += Shift2[1] - height + y;
		Shift2[0] = x;
		Shift2[1] = height - y;
	}
	glutPostRedisplay();
}


void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP && zoom > 0.25)
	{
		Shift[1] += Shift2[1] - 5;
		Shift2[1] = Shift2[1];
	}
	if (key == GLUT_KEY_DOWN) 
	{
		Shift[1] += Shift2[1] + 5;
		Shift2[1] = Shift2[1];
	}
	if (key == GLUT_KEY_RIGHT)
	{
		Shift[0] += Shift2[0] - 5;
		Shift2[0] = Shift2[0];
	}
	if (key == GLUT_KEY_LEFT)
	{
		Shift[0] += Shift2[0] + 5;
		Shift2[0] = Shift2[0];
	}
	glutPostRedisplay();
}

void Initialize()
{
	glClearColor(1.0, 1.0, 1.0, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluOrtho2D(0.0, width, 0.0, height);
	loadTexture();
}

void main(int argc, char* argv[])
{
	width = 800;
	height = 500;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("»нтерпол€ционный сплайн на основе полиномов Ћагранжа степени 3.");
	Initialize();
	glutSpecialFunc(SpecialKeys);
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutPassiveMotionFunc(Move);
	glutMainLoop();
}