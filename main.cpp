//
//  main.cpp
//  Terrain
//
//  Created by Jinyue Song on 2015-11-06.
//  Copyright © 2015 Jinyue Song. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#ifdef __APPLE__
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <GLUT/glut.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/freeglut.h>
#endif

using namespace std;

float eye[] = {50, 50, 50};

//camera postion
float camPos[] = {200, 200, 50};

//light position
float light_pos[] = {.8, .7, .6, 0};
float light_pos2[] = {-200, 30, 200, 1};

//material
float amb0[4] = {.2, .32, .2, 1};
float diff0[4] = {.31, .31, .31, 1};
float spec0[4] = {.2, 0.2, 0.1, 1};

float m_amb[] = {.33, .22, .22, 1.0};
float m_diff[] = {.78, .57, .11, 1.0};
float m_spec[] = {.29, .51, .51, 1.0};
float shiny = 20;

// map size is 300*300
int map[300][300] = {};

//array to store random position and later, update the position array to map array to draw circles
int randPos[100][2] = {};


void initMap(){
    for (int x = 0; x < 300; x++) {
        for (int z = 0; z < 300; z++) {
            map[x][z] = 0;
        }
    }
}

//generate circle with random position but certain ratio

/*
 for each terrain point (tx,tz) do
 
	pd = distance from circle center * 2 / terrainCircleSize
	
	if fabs(pd) <= 1.0
 
 height(tx,tz) +=  disp/2 + cos(pd*3.14)*disp/2;

 this pseudocode is from http://www.lighthouse3d.com/opengl/terrain/index.php?circles to help with coding
*/

int circSize = 100;
int disp = 10;
void drawCir(){
    for (int indxRadm = 0; indxRadm < 100; indxRadm++) {
        int centerX = randPos[indxRadm][0];
        int centerZ = randPos[indxRadm][1];
        
        //determine the circle is out of the map boundary or not
        //have a clear circle boundary and within the circle, catch all the pixel and calculate the height of point
        //in half circle, there is a triangle with 90 degree for three vertces on the circle
        //height is a and x/a = a/(longSide-x)
        
        int xstart = centerX - circSize/2;
        for (int dx = 0; dx < circSize; dx++) {
            int a = (int)sqrtf(dx * (circSize - dx));
            int x = xstart + dx;
            for (int z = centerZ - a; z < centerZ + a; z++) {
                if (300 > x && x >= 0) {
                    if (300 > z && z >= 0) {
                        int pd = sqrtf((x-centerX)*(x-centerX)+(z-centerZ)*(z-centerZ)) * 2 / circSize;
                        map[x][z] += disp/2 + cosf(pd * 3.14) * disp/2;
                    }
                }
            }
        }
    }
        
        }


//func to generate random position
void createRandPos(){
    srand(time(NULL));
    for (int i = 0; i < 100; i++) {
        randPos[i][0] = rand() % 300;//random number from 0 to 299
        randPos[i][1] = rand() % 300;
    }
}



//keyboard function
void kbd(unsigned char key, int x, int y){
    switch (key) {
        case 'q':
        case 'Q':
            exit(0);
            break;
            
        default:
            break;
    }
}

void special(int key, int x, int y){
    switch (key) {
        case GLUT_KEY_LEFT:
            eye[0] -= 10;
            break;
            
        case GLUT_KEY_RIGHT:
            eye[0] += 10;
            break;
        
        case GLUT_KEY_UP:
            eye[2] += 10;
            break;
            
        case GLUT_KEY_DOWN:
            eye[2] -= 10;
            break;
            
        default:
            break;
    }
}

void drawMap(){
    for (int x = 0; x < 299; x++) {
        for (int z = 0; z < 299; z++) {
            glBegin(GL_POLYGON);
            float mod = 1.0 * map[x][z] / 100;
            glColor3f(mod/4, mod/2, mod/4);
            glNormal3i(x, map[x][z], z);
            glVertex3i(x, map[x][z],z);
            glNormal3i(x+1, map[x+1][z], z);
            glVertex3i(x+1, map[x+1][z], z);
            glNormal3i(x+1, map[x+1][z+1], z+1);
            glVertex3i(x+1, map[x+1][z+1], z+1);
            glNormal3i(x, map[x][z+1], z+1);
            glVertex3i(x, map[x][z+1], z+1);
            glEnd();
        }
    }
    
}

void drawMapLine(){
    for (int x = 0; x < 299; x++) {
        for (int z = 0; z < 299; z++) {
            glBegin(GL_LINES);
            
            glColor3f(1, 1, 1);
            
            glVertex3i(x, map[x][z], z);
            glVertex3i(x+1, map[x+1][z], z);
            
            glVertex3i(x+1, map[x+1][z], z);
            glVertex3i(x+1, map[x+1][z+1], z+1);

            glVertex3i(x+1, map[x+1][z+1], z+1);
            glVertex3i(x, map[x][z+1], z+1);
            
            glVertex3i(x, map[x][z+1], z+1);
            glVertex3i(x, map[x][z], z);
            
            glEnd();
            
        }
    }
}

/* drawAxis() -- draws an axis at the origin of the coordinate system
 *   red = +X axis, green = +Y axis, blue = +Z axis
 */
void drawAxis()
{
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(0,0,0);
    glVertex3f(500,0,0);
    
    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,500,0);
    
    glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,500);
    glEnd();
}

void init(){
    glClearColor(0, 0, 0, 1);
    glColor3f(1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(45, 1, 1, 100);
    glOrtho(-250, 250, -250, 250, -250, 250);
    
    glShadeModel(GL_SMOOTH);
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos2);

    glLightfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb0);
    glLightfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff0);
    glLightfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec0);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);
    
    glEnable(GL_DEPTH_TEST);

}

void display(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(eye[0], eye[1], eye[2], 0, 0, 0, 0, 1, 0);
    
    drawAxis();
    
    glColor3f(1, 1, 1);
    
    
    glPushMatrix();
    
    
    
    //drawMap();
    
    drawMapLine();
    
    glPopMatrix();
    
    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Terrain");
    
    
    glEnable(GL_NORMALIZE);
    
    init();
    
    initMap();
    createRandPos();
    drawCir();
    
    
    glutKeyboardFunc(kbd);
    glutSpecialFunc(special);
    
    glutDisplayFunc(display);
    
    glutMainLoop();
    
    
    
    return 0;
}
