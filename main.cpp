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
#include "mesh3D2.h"
#include "Mesh3D2.cpp"

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

float camPos[] = {200, 200, 50};

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

int circSize = 20;
int disp = 4;
void drawCir(){
    for (int indxRadm = 0; indxRadm < 100; indxRadm++) {
        int centerX = randPos[indxRadm][0];
        int centerZ = randPos[indxRadm][1];
        
        //determine the circle is out of the map boundary or not
        //have a clear circle boundary and within the circle, catch all the pixel and calculate the height of point
        //in half circle, there is a triangle with 90 degree for three vertces on the circle
        //height is a and x/a = a/(longSide-x)
        
        
        for (int x = centerX - circSize/2; x < centerX + circSize/2; x++) {
            int a = sqrtf(x * (circSize-x));
            for (int z = centerZ - a; z < centerZ + a; z++) {
                if (300 > x && x >= 0) {
                    if (300 > z && z >= 0) {
                        map[x][z] += disp/2 + cosf(sqrtf( (x-centerX)*(x-centerX)+(z-centerZ)*(z-centerZ)) * 3.14) * disp/2;
                        printf("Current point height: %d\n", map[x][z]);
                    }
                }
            }
        }
    }
        
        }


//func to generate random position
void createRandPos(){
    for (int i = 0; i < 100; i++) {
        randPos[i][0] = rand() % 300;//random number from 0 to 299
        printf("RandomX: %d", randPos[i][0]);
        randPos[i][1] = rand() % 300;
        printf(" RandomZ: %d\n", randPos[i][1]);

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
    glutPostRedisplay();
}

void drawMap(){
    glBegin(GL_POLYGON);
    for (int x = 0; x < 299; x++) {
        for (int z = 0; z < 299; z++) {
            
            glColor3f(x/300, 0.5, z/300);
            glVertex3i(x, map[x][z],z);
            glVertex3i(x+1, map[x+1][z], z);
            glVertex3i(x+1, map[x+1][z+1], z+1);
            glVertex3i(x, map[x][z+1], z+1);
            
        }
    }
    glEnd();
    
}

void drawMapLine(){
    for (int x = 0; x < 299; x++) {
        for (int z = 0; z < 299; z++) {
            glBegin(GL_LINE);
            glColor3f(1, 0, 0);
            glVertex3i(x, map[x][z], z);
            glVertex3i(x+1, map[x+1][z], z);
            glVertex3i(x+1, map[x+1][z+1], z+1);
            glVertex3i(x, map[x][z+1], z+1);
            glEnd();
        }
    }
    glFlush();
}



void init(){
    glClearColor(0, 0, 0, 0);
    glColor3f(1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(45, 1, 1, 1000);
    glOrtho(-500, 500, -500, 500, -500, 500);
    
}

void display(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(camPos[0], camPos[1], camPos[2], 0, 0, 0, 0, 1, 0);
    glColor3f(1, 1, 1);
    
    
    drawMap();
    
    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Terrain");
    
    glEnable(GL_DEPTH_TEST);
    
    init();
    
    initMap();
    createRandPos();
    drawCir();
    
    glutKeyboardFunc(kbd);
    
    
    glutDisplayFunc(display);
    
    glutMainLoop();
    
    
    
    return 0;
}
