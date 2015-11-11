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
#include <iostream>

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
//map size
int mapSize;

//angle for rotate with Y or (X and Z)
int angY = 0;
int angX = 0;

//flag used to switch different map
int flag_map = 0;

//flag used to turn on/off the light
int flag_Light = 1;

//flag used to draw axis
int flag_axis = 0;

//inital viewing position
float eye[] = {250, 250, 250};

//light position
float light_pos[] = {0, 0, 0, 1};
float light_pos1[] = {0, 50, 0, 1};

//light
float amb0[4] = {1, 1, 1, 1};
float diff0[4] = {1, .31, .31, 1};
float spec0[4] = {.2, 0.2, 0.1, 1};

//material
float m_amb[] = {.33, .22, .22, 1.0};
float m_diff[] = {.78, .57, .11, 1.0};
float m_spec[] = {.29, .51, .51, 1.0};
float shiny = 20;

// map array
int** map;

//array to store random position and used to update the position array to map array and draw circles
int randPos[100][2] = {};


void initMap(){
    int n = mapSize;
    map = new int*[n];
    for (int x = 0; x < n; x++) {
        map[x] = new int[n];
        for (int z = 0; z < n; z++) {
            map[x][z] = 0;
        }
    }
}

void deleteMap() {
    int n = mapSize;
    for(int i = 0; i < n; i++) {
        delete[] map[i];
    }
    delete[] map;
}

//generate circle with random position but certain ratio

/*
 for each terrain point (tx,tz) do
 
	pd = distance from circle center * 2 / terrainCircleSize
	
	if fabs(pd) <= 1.0
 
 height(tx,tz) +=  disp/2 + cos(pd*3.14)*disp/2;

 this pseudocode is from http://www.lighthouse3d.com/opengl/terrain/index.php?circles to help with coding
*/

void drawCir(){
    int circSize = mapSize/3;
    int disp = 2 + (mapSize - 50) / 30;
//    printf("%d\n", circSize);
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
                if (mapSize > x && x >= 0) {
                    if (mapSize > z && z >= 0) {
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
        randPos[i][0] = rand() % mapSize;//random number from 0 to 299
        randPos[i][1] = rand() % mapSize;
    }
}



//keyboard function
void kbd(unsigned char key, int x, int y){
    switch (key) {
            //press q or Q to exit
        case 'q':
        case 'Q':
            exit(0);
            break;
            
            //press l or L to turn on/off the light
        case 'l':
        case 'L':
            flag_Light = 0;
            break;
            
            //turn off light
        case 'k':
        case 'K':
            flag_Light = 1;
            break;
            
            //map rotate with Y axis
        case 'd':
            angY -= 10;
            break;
            
            //map rotate with Y
        case 'f':
            angY += 10;
            break;
            
            //map rotate with X
        case 'e':
            angX += 10;
            break;
            
            //map rotate with X
        case 'c':
            angX -= 10;
            break;
         
            //use flag to switch three maps
        case 'w':
        case 'W':
            flag_map += 1;
            flag_map = flag_map % 3;
            break;
            
            //re-draw the map
        case 'r':
            deleteMap();
            initMap();
            createRandPos();
            drawCir();
            break;
            
        case 'h':
            flag_axis++;
            break;
            
        default:
            break;
    }
}

//special func to change viewing position
void special(int key, int x, int y){
    double ang = 0;
    switch (key) {
            //view point rotate around Y axis
        case GLUT_KEY_LEFT:
            ang += .1;
            eye[0] = eye[0] * cos(ang) + eye[2] * sin(ang);
            eye[2] = eye[0] * (-sin(ang)) + eye[2] * cos(ang);
            break;
            
            //view point rotate around Y axis
        case GLUT_KEY_RIGHT:
            ang -= .1;
            eye[0] = eye[0] * cos(ang) + eye[2] * sin(ang);
            eye[2] = eye[0] * (-sin(ang)) + eye[2] * cos(ang);
            break;
        
            //view point rotate around X axis
        case GLUT_KEY_UP:
            ang -= .1;
            eye[1] = eye[1] * cos(ang) - eye[2] * sin(ang);
            eye[2] = eye[1] * (sin(ang)) + eye[2] * cos(ang);
            break;
            
            //view point rotate around X axis
        case GLUT_KEY_DOWN:
            ang += .1;
            eye[1] = eye[1] * cos(ang) - eye[2] * sin(ang);
            eye[2] = eye[1] * (sin(ang)) + eye[2] * cos(ang);
            break;
        
        default:
            break;
    }
}

//draw the map and same time normalize the polygon
void drawMap(){
    int n = mapSize - 1;
    for (int x = 0; x < n; x++) {
        for (int z = 0; z < n; z++) {
            glBegin(GL_POLYGON);
            int div = 10 + (mapSize - 50) / 3;
            float mod = 1.0 * map[x][z] / div;

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

//draw map with line
void drawMapLine(){
    int n = mapSize - 1;
    for (int x = 0; x < n; x++) {
        for (int z = 0; z < n; z++) {
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
//draw axis to find XYZ direction
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

//inital background etc
void init(){
    glClearColor(0, 0, 0, 1);
    glColor3f(1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //perpective viewing mode helps viewer to catch and distinguish lines; if in ortho viewing mode, lines are too close and combined to a face
    gluPerspective(63, 1, 700, 1);
    //glOrtho(-250, 250, -250, 250, -250, 250);
    
    //glShadeModel(GL_SMOOTH);
    
    
    
    
    
    glEnable(GL_DEPTH_TEST);

}


void display(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
    if (flag_Light == 0) {
        glEnable(GL_LIGHTING);
    }
    
    if (flag_Light == 1) {
        glDisable(GL_LIGHTING);
    }
    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
    //drawAxis();
    
    glColor3f(1, 1, 1);
    
    glPushMatrix();
    
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);
    
    glLightfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb0);
    glLightfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff0);
    glLightfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec0);
    
    //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
    //glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    
    
    
    
    
    gluLookAt(eye[0], eye[1], eye[2], 0, 0, 0, 0, 1, 0);
    
    glRotated(angY, 0, 1, 0);
    glRotated(angX, 1, 0, 0);
    
    //switch maps
    if (flag_map == 0) {
        drawMap();
    }
    
    if (flag_map == 1) {
        drawMapLine();
    }
    
    if (flag_map == 2) {
        drawMap();
        drawMapLine();
    }
    
    //draw axis or not
    if (flag_axis%2 == 1) {
        drawAxis();
    }
    
    glPopMatrix();
    
    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char * argv[]) {
    
    cout << "Suggest map size between 100 and 250. Too small # is hard to see and too big # will involve camera but with better details. \n";
    cout << "Please enter the map size: ";
    cin >> mapSize;
    
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Terrain");
    
    glEnable(GL_NORMALIZE);
    
    init();
    
    initMap();
    createRandPos();
    drawCir();
    
    for (int x = 0; x < mapSize; x++) {
        for (int z = 0; z < mapSize; z++) {
            printf("At (%d, %d), height is: %d\n", x, z, map[x][z]);
        }
    }
    
    cout << "key menue: \n";
    cout << "'w' switch map, 'l' turn on light, 'k' turn off light, 'r' re-draw, 'h' draw XYZ axis, 'q' exit \n";
    cout << "map movement: 'e' forward, 'c' backward, 'd' left turn, 'f' right turn \n";
    cout << "eye movement: 'up' rotate with xz+, 'down' rotate with xz-, 'left' rotate around Y+ axis, 'right' rotate around Y- axis \n";

    
    glutKeyboardFunc(kbd);
   
    glutSpecialFunc(special);
 
    glutDisplayFunc(display);
    
    glutMainLoop();
    
    return 0;
}
