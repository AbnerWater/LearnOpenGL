#include <GL/glew.h>
#include <gl\freeglut.h>
#define HDF5_DISABLE_VERSION_CHECK 1
#include <windows.h>
#include <iostream>
#include "H5Reader.h"
#define Demo 8
using namespace std;
//freeglut var
GLuint VBO;
void idle(void);
void display(void);
void reshape(int x, int y);
void keypress(unsigned char key, int x, int y);
void skeypress(int key, int x, int y);
void mouse(int bn, int st, int x, int y);
void motion(int x, int y);
int win_width, win_height;
float cam_theta, cam_phi = 25, cam_dist = 8;
float cam_pan[3];
vector<unsigned int> ele;
int mouse_x, mouse_y;
int bnstate[8];
int anim, help;
long anim_start;
long nframes;
void idle(void)
{
	glutPostRedisplay();
}
void display(void)
{
	long tm;
	float lpos[] = { -1, 2, 3, 0 };
	glClear(GL_COLOR_BUFFER_BIT);
	glEnableVertexAttribArray(0);//输入参数为属性位置，与shader中的location保持一致
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDrawElements(GL_STATIC_DRAW, ele.size(), GL_TRIANGLES, ele.data());
	glDisableVertexAttribArray(0);
	glutSwapBuffers();
	glFlush();
}
#define ZNEAR	0.5f
void reshape(int x, int y)
{
	float vsz, aspect = (float)x / (float)y;
	win_width = x;
	win_height = y;

	glViewport(0, 0, x, y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	vsz = 0.4663f * ZNEAR;
	glFrustum(-aspect * vsz, aspect * vsz, -vsz, vsz, 0.5, 500.0);
}
void keypress(unsigned char key, int x, int y)
{
	static int fullscr;
	static int prev_xsz, prev_ysz;

	switch (key) {
	case 27:
	case 'q':
		exit(0);
		break;

	case ' ':
		anim ^= 1;
		glutIdleFunc(anim ? idle : 0);
		glutPostRedisplay();

		if (anim) {
			anim_start = glutGet(GLUT_ELAPSED_TIME);
			nframes = 0;
		}
		else {
			long tm = glutGet(GLUT_ELAPSED_TIME) - anim_start;
			long fps = (nframes * 100000) / tm;
			printf("framerate: %ld.%ld fps\n", fps / 100, fps % 100);
		}
		break;

	case '\n':
	case '\r':
		if (!(glutGetModifiers() & GLUT_ACTIVE_ALT)) {
			break;
		}
	case 'f':
		fullscr ^= 1;
		if (fullscr) {
			prev_xsz = glutGet(GLUT_WINDOW_WIDTH);
			prev_ysz = glutGet(GLUT_WINDOW_HEIGHT);
			glutFullScreen();
		}
		else {
			glutReshapeWindow(prev_xsz, prev_ysz);
		}
		break;
	}
}
void skeypress(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_F1:
		help ^= 1;
		glutPostRedisplay();

	default:
		break;
	}
}
void mouse(int bn, int st, int x, int y)
{
	int bidx = bn - GLUT_LEFT_BUTTON;
	bnstate[bidx] = st == GLUT_DOWN;
	mouse_x = x;
	mouse_y = y;
}
void motion(int x, int y)
{
	int dx = x - mouse_x;
	int dy = y - mouse_y;
	mouse_x = x;
	mouse_y = y;

	if (!(dx | dy)) return;

	if (bnstate[0]) {
		cam_theta += dx * 0.5;
		cam_phi += dy * 0.5;
		if (cam_phi < -90) cam_phi = -90;
		if (cam_phi > 90) cam_phi = 90;
		glutPostRedisplay();
	}
	if (bnstate[1]) {
		float up[3], right[3];
		float theta = cam_theta * PI / 180.0f;
		float phi = cam_phi * PI / 180.0f;

		up[0] = -sin(theta) * sin(phi);
		up[1] = -cos(phi);
		up[2] = cos(theta) * sin(phi);
		right[0] = cos(theta);
		right[1] = 0;
		right[2] = sin(theta);
		cam_pan[0] += (right[0] * dx + up[0] * dy) * 0.01;
		cam_pan[1] += up[1] * dy * 0.01;
		cam_pan[2] += (right[2] * dx + up[2] * dy) * 0.01;
		glutPostRedisplay();
	}
	if (bnstate[2]) {
		cam_dist += dy * 0.1;
		if (cam_dist < 0) cam_dist = 0;
		glutPostRedisplay();
	}
}
static int CreateVertexBuffer()
{
	H5Reader mesh("D:/00_Github/LearnOpenGL/Learning/x64/Debug/xjtu.h5");
	vector<float> ver;
	vector<unsigned int> ind;
	if (!mesh.Avilable())
		return -1;
	ver = mesh.GetNodes();
	ele = mesh.GetElement();
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*ver.size(), ver.data(), GL_STATIC_DRAW);
	return 0;
}
//freeglut 加载网格
int freeglut_mesh(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("freeglut 3D view demo");
	glClearColor(0.2f, 0.6f, 0.3f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keypress);
	glutSpecialFunc(skeypress);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (CreateVertexBuffer() < 0)
		return -1;
	glutMainLoop();
	return 0;
}
int main(int argc, char **argv)
{
	return freeglut_mesh(argc, argv);
}