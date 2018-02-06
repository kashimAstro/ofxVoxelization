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
	int counter;
        int speedanim;
        float animationPosition;

	void setup()
	{
		ofSetVerticalSync(false);
		ofSetSmoothLighting(true);
		point     = false;
		counter   = 0;
        	speedanim = 50;

		assimp.loadModel("models/model.dae");
		cout << "mesh num: " << assimp.getMeshCount() << endl;
		for(int i = 0; i < assimp.getMeshCount(); i++) {
			m.push_back(assimp.getMesh(i));
		}

		shadow.setup();
		shadow.setColors(ofColor(0,0,0));
		shadow.setAlpha(1.f);

		light.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
        	light.setSpecularColor( ofColor(255.f, 255.f, 55.f));
		light.setPosition(ofVec3f(40,20,30));

		mat.setShininess( 64 );
		cam.setNearClip(.1);
	}

	void update()
	{
		shadow.setDirLight(light.getPosition());
		assimp.update();
                animationPosition = ofMap(counter++,0,speedanim,0.0,1.0);
                if(counter>speedanim) counter=0;
                assimp.setPositionForAllAnimations(animationPosition);
		m.clear();
		for(int i = 0; i < assimp.getMeshCount(); i++) {
			m.push_back(assimp.getCurrentAnimatedMesh(i));
		}
		mesh = vox.voxel(m,0.55,1.5);
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
        	ofBackgroundGradient(ofColor::white,ofColor::orange);
	        ofEnableDepthTest();

		cam.begin();
		ofEnableLighting();
    		light.enable();
    		mat.begin();

		render();
		
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
