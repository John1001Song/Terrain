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

void drawCir(vertex3D::vertex3D){
    
}


//func to generate random position
void createRanPos(){
    for (int i = 0; i < 100; i++) {
        randPos[i][0] = rand() % 300;//random number from 0 to 299
        randPos[i][1] = rand() % 300;
    }
}



//keyboard function
void kbd(){}



void display(){}

int main(int argc, char * argv[]) {
    
    return 0;
}
