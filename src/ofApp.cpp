#include "ofApp.h"
#include "ofAppGLFWWindow.h"

void ofApp::setup(){

	CustomApp::setup();

	// LISTENERS
	ofAddListener(screenSetup.setupChanged, this, &ofApp::setupChanged);
	ofAddListener(RUI_GET_OF_EVENT(), this, &ofApp::remoteUIClientDidSomething);


	ofLoadImage(background, "images/bg.png");

	layerNames.push_back(LayerContent("cloth","cloth.png"));
	layerNames.push_back(LayerContent("Sole","sole.png"));
	layerNames.push_back(LayerContent("frontLeather","frontLeather.png"));
	layerNames.push_back(LayerContent("bumper","bumper.png"));
	layerNames.push_back(LayerContent("backCross","backCross.png"));
	layerNames.push_back(LayerContent("SecondaryLine","bottomSecondaryLine.png", "topSecondaryline.png"));
	layerNames.push_back(LayerContent("MainLine","bottomMainLine.png", "topMainLine.png"));
	layerNames.push_back(LayerContent("top Edge","topEdge.png"));
	layerNames.push_back(LayerContent("Tongue","tongue.png"));
	layerNames.push_back(LayerContent("shoeLaces","shoelaces.png"));

	for(int i = 0; i < layerNames.size(); i++){
		Layer *l = new Layer();
		l->enabled = false;
		l->color = ofColor(255,255);
		for(int j = 0; j < layerNames[i].layers.size(); j++) {
			ofTexture * tex = new ofTexture();
			ofLoadImage(*tex, "images/" + layerNames[i].layers[j]);
			l->texs.push_back(tex);
		}
		layers.push_back(l);
		string n = layerNames[i].name;
		RUI_NEW_GROUP(n);
		RUI_SHARE_PARAM_WCN(n + "_enabled", layers[i]->enabled);
		RUI_SHARE_COLOR_PARAM_WCN(n + "_color", layers[i]->color);
	}
	ofSetWindowShape(background.getWidth(), background.getHeight());

	RUI_LOAD_FROM_XML();

	colors.push_back(ofColor::cyan);
	colors.push_back(ofColor::magenta);
	colors.push_back(ofColor::yellow);
	colors.push_back(ofColor::black);
	colorNames.push_back("c");
	colorNames.push_back("m");
	colorNames.push_back("y");
	colorNames.push_back("k");
}

void ofApp::randomize(){

	for(int i = 0; i < layers.size(); i++){
		layers[i]->color.setHsb(ofRandom(255), 255, 255);
	}
}

void ofApp::randomize2(){

	ofColor c;
	c.setHsb(ofRandom(255), 255, 255);
	for(int i = 0; i < layers.size(); i++){
		layers[i]->color = c;
	}
}


void ofApp::update(){
}


void ofApp::draw(){

	ofSetColor(255);
	background.draw(0,0);

	for(int i = 0; i < layers.size(); i++){
		if(layers[i]->enabled){
			ofSetColor(layers[i]->color);
			for(int j = 0; j < layers[i]->texs.size(); j++){
				layers[i]->texs[j]->draw(0,0);
			}
		}
	}
}


void ofApp::keyPressed(int key){

	if(key == 'w'){
		screenSetup.cycleToNextScreenMode();
	}
	if(key == 'r'){
		randomize();
	}
	if(key == 't'){
		randomize2();
	}

	if(key == 'e'){
		ofFbo fbo;
		vector<int> numbers;
		int nl = 0;
		for(int i = 0; i < layers.size(); i++){
			if(layers[i]->enabled){
				layerIndex[nl] = i;
				nl++;
			}
		}
		numbers.resize(nl);
		vector<int> maxes;
		for(int i = 0; i < nl; i++){
			maxes.push_back(colors.size());
		}

		nCombinations = powf(colors.size(), nl);
		cout << "exporting " <<  nCombinations << " combinations... " << endl;
		fbo.allocate(ofGetWidth(), ofGetHeight());
		int count = 0;
		recursive(numbers.size(), numbers, maxes, fbo, count);
	}

	RUI_PUSH_TO_CLIENT();
}


void ofApp::recursive(int depth, vector<int> & colorIndexes, vector<int> & colorIndexMax, ofFbo & fbo, int& count){

	if (depth > 0){
		for(int i = 0; i < colorIndexMax[depth-1]; i++){
			colorIndexes[depth - 1] = i;
			recursive(depth - 1, colorIndexes, colorIndexMax, fbo, count);
		}
	}else{
		string name;
		for(int i = 0; i < colorIndexes.size(); i++){ //walk layers
			int colorIndex = colorIndexes[i];
			int li = layerIndex[i]; //layer index
			layers[li]->color = colors[colorIndex];
			name += colorNames[colorIndex];
		}

		fbo.begin();
		draw();
		fbo.end();
		ofPixels p;
		fbo.getTextureReference().readToPixels(p);
		cout << name << " " << count << "/" << nCombinations << " (" << 100 * count / nCombinations << "%)" <<endl;
		ofSaveImage(p, "export/" + name + ".tga");
		count ++;

		RUI_GET_INSTANCE()->update(0.016);
	}
}


void ofApp::keyReleased(int key){

}


void ofApp::mouseMoved(int x, int y ){

}


void ofApp::mouseDragged(int x, int y, int button){

}


void ofApp::mousePressed(int x, int y, int button){

}


void ofApp::mouseReleased(int x, int y, int button){

}


void ofApp::windowResized(int w, int h){

}


void ofApp::gotMessage(ofMessage msg){

}


void ofApp::dragEvent(ofDragInfo dragInfo){
	
}


//////// CALLBACKS //////////////////////////////////////

void ofApp::setupChanged(ofxScreenSetup::ScreenSetupArg &arg){
	ofLogNotice()	<< "ofxScreenSetup setup changed from " << screenSetup.stringForMode(arg.oldMode)
	<< " (" << arg.oldWidth << "x" << arg.oldHeight << ") "
	<< " to " << screenSetup.stringForMode(arg.newMode)
	<< " (" << arg.newWidth << "x" << arg.newHeight << ")";
}


//define a callback method to get notifications of client actions
void ofApp::remoteUIClientDidSomething(RemoteUIServerCallBackArg &arg){
	switch (arg.action) {
		case CLIENT_CONNECTED: cout << "CLIENT_CONNECTED" << endl; break;
		case CLIENT_DISCONNECTED: cout << "CLIENT_DISCONNECTED" << endl; break;
		case CLIENT_UPDATED_PARAM: cout << "CLIENT_UPDATED_PARAM: "<< arg.paramName << " - ";
			arg.param.print();
			break;
		case CLIENT_DID_SET_PRESET: cout << "CLIENT_DID_SET_PRESET" << endl; break;
		case CLIENT_SAVED_PRESET: cout << "CLIENT_SAVED_PRESET" << endl; break;
		case CLIENT_DELETED_PRESET: cout << "CLIENT_DELETED_PRESET" << endl; break;
		case CLIENT_SAVED_STATE: cout << "CLIENT_SAVED_STATE" << endl; break;
		case CLIENT_DID_RESET_TO_XML: cout << "CLIENT_DID_RESET_TO_XML" << endl; break;
		case CLIENT_DID_RESET_TO_DEFAULTS: cout << "CLIENT_DID_RESET_TO_DEFAULTS" << endl; break;
		default:
			break;
	}
}
