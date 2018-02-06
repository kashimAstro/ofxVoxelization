#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxVoxelization.h"
#include "ofxFakeShadowMap.h"

class ofApp : public ofBaseApp
{
	public:
	ofEasyCam cam;
	ofxAssimpModelLoader assimp;
	ofMesh mesh;
	ofxVoxelization vox;
        ofxFakeShadowMap shadow;
	vector<ofMesh> m;

	ofLight light;
	ofMaterial mat;
	bool point;

	void setup()
	{
		ofSetVerticalSync(false);
		ofSetSmoothLighting(true);
		point     = false;

		assimp.loadModel("models/bunny.obj");
		cout << "mesh num: " << assimp.getMeshCount() << endl;
		for(int i = 0; i < assimp.getMeshCount(); i++) {
			m.push_back(assimp.getMesh(i));
		}
		mesh = vox.voxel(m,0.0025,0.01);

		shadow.setup();
		shadow.setColors(ofColor(0,0,0));
		shadow.setAlpha(1.f);

		light.setDiffuseColor( ofColor(255.f, 155.f, 255.f));
        	light.setSpecularColor( ofColor(55.f, 255.f, 55.f));
		light.setPosition(ofVec3f(40,20,30));

		mat.setShininess( 64 );
		cam.setNearClip(.1);
	}

	void render()
	{
		if(!point)
			mesh.draw(OF_MESH_FILL);
		else
			mesh.draw(OF_MESH_POINTS);
	}

	void draw()
	{
        	ofBackgroundGradient(ofColor::white,ofColor::violet);
	        ofEnableDepthTest();

		cam.begin();
		ofEnableLighting();
    		light.enable();
    		mat.begin();

		ofPushMatrix();
		ofTranslate(0,-0.015,0);
		render();
		ofPopMatrix();
		
		shadow.begin(cam);
			render();
		shadow.end();

		mat.end();
		light.disable();
		ofDisableLighting();
		cam.end();
	        ofDisableDepthTest();
	}

	void keyPressed(int key)
	{
		point=!point;
	}
};

int main( int argc, char ** argv )
{
	ofSetupOpenGL(1024,768, OF_WINDOW);
	ofRunApp( new ofApp());
}
