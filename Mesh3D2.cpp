#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "mesh3D2.h"
#include <stdio.h>

using namespace std;

Mesh3D::Mesh3D(){
	verts = new vector<vertex3D>();
	faces = new vector<face3D>();
	colours = new vector<colour3D>();
}

void Mesh3D::drawMesh(){
	//use quads or triangles ot polygons to draw the 
}

void Mesh3D::load(){
	//create a mesh to test
}

void Mesh3D::loadObj(char* filename){
	//load your obj file
}