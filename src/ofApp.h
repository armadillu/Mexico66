#pragma once

#include "ofMain.h"
#include "CustomApp.h"


class ofApp : public CustomApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);


	// APP CALLBACKS ////////////////////////////////////////

	void setupChanged(ofxScreenSetup::ScreenSetupArg &arg);
	void remoteUIClientDidSomething(RemoteUIServerCallBackArg & arg);


	// APP SETUP ////////////////////////////////////////////

	struct Layer{
		ofColor color;
		bool enabled;
		vector<ofTexture*> texs;
	};

	struct LayerContent{
		string name;
		vector<string> layers;
		LayerContent(string lname, string l1){
			name = lname;
			layers.push_back(l1);
		};
		LayerContent(string lname, string l1, string l2){
			name = lname;
			layers.push_back(l1);
			layers.push_back(l2);
		}
	};


	ofTexture background;

	vector<Layer*> layers;
	vector<LayerContent> layerNames;

	void randomize();
	void randomize2();

	void recursive(int depth, vector<int> & colorIndexes,vector<int> & colorIndexMax, ofFbo & fbo, int & count);

	vector<ofColor> colors;
	vector<string> colorNames;
	map<int, int> layerIndex; //map IDs to skip disabled layers
	int nCombinations;

};
