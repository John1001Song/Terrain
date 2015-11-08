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
int xz[300][300] = {};

void initMap(){
    for (int i = 0; i < 300; i++) {
        for (int j = 0; j < 300; j++) {
            xz[i][j] = 0;
        }
    }
}

//generate circle with random position but certain ratio
void randCir(){}

//keyboard function
void kbd(){}



void display(){}

int main(int argc, char * argv[]) {
    
    return 0;
}
