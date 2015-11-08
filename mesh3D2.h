#ifndef __MESH3D2_H__
#define __MESH3D2_H__
#include <vector>

using namespace std;
	
	struct vertex3D{
		float x, y, z;
	};
	
	struct face3D{
		//how many vertex per face
	};
	
	struct colour3D{
		//rgb value
	};
	
	class Mesh3D{
	public: 
		Mesh3D();
		void drawMesh();
		void load();
		void loadObj(char* filename);
		
	private:
		vector<vertex3D>*verts;
		vector<face3D>*faces;
        vector<colour3D>*colours;
	};

#endif


