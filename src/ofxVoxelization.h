#include "ofMain.h"
#define VOXELIZER_DEBUG
#define VOXELIZER_IMPLEMENTATION
#include "voxelizer.h"

class ofxVoxelization 
{
	public:
	float res = 0.25;
	float precision = 1.0;
	vx_mesh_t* result;

	void calcNormals( ofMesh & mesh, bool bNormalize ) //thk zach lieberman
	{
		for( int i=0; i < mesh.getVertices().size(); i++ ) mesh.addNormal(ofPoint(0,0,0));
	        for( int i=0; i < mesh.getIndices().size(); i+=3 ){
			const int ia = mesh.getIndices()[i];
			const int ib = mesh.getIndices()[i+1];
			const int ic = mesh.getIndices()[i+2];
			ofVec3f e1 = mesh.getVertices()[ia] - mesh.getVertices()[ib];
			ofVec3f e2 = mesh.getVertices()[ic] - mesh.getVertices()[ib];
			ofVec3f no = e2.cross( e1 );
			mesh.getNormals()[ia] += no;
			mesh.getNormals()[ib] += no;
			mesh.getNormals()[ic] += no;
		}
	    	if (bNormalize) {
		    for(int i=0; i < mesh.getNormals().size(); i++ ) {
			ofVec3f(mesh.getNormals()[i]).normalize();
	    	    }
		}
	}

	ofMesh voxel(vector<ofMesh> _m, float resolution, float precision)
	{
		res = resolution;
		precision = precision;

		ofMesh meshed;
		meshed.setMode(OF_PRIMITIVE_TRIANGLES);

		for (size_t i = 0; i < _m.size(); i++) 
		{
			vx_mesh_t* mesh;
			mesh = vx_mesh_alloc(_m[i].getNumVertices(), _m[i].getNumIndices());
			for (size_t f = 0; f < _m[i].getNumIndices(); f++) 
			    mesh->indices[f] = _m[i].getIndices()[f];
			for (size_t v = 0; v < _m[i].getNumVertices(); v++) 
			{
			    mesh->vertices[v].x = _m[i].getVertex(v).x;
			    mesh->vertices[v].y = _m[i].getVertex(v).y;
			    mesh->vertices[v].z = _m[i].getVertex(v).z;
			}
			result = vx_voxelize(mesh, res, res, res, precision);

			for (int j = 0; j < result->nvertices; ++j) 
				meshed.addVertex(ofVec3f(result->vertices[j].x,result->vertices[j].y,result->vertices[j].z));
			for (int j = 0; j < result->nnormals; ++j) 
				meshed.addNormal(ofVec3f(result->normals[j].z, result->normals[j].y, result->normals[j].x));
			for (int j = 0; j < result->nindices; j++) 
				meshed.addIndex(result->indices[j]);
			vx_mesh_free(result);
			vx_mesh_free(mesh);
		}
		calcNormals(meshed, true);
		return meshed;
	}
};
