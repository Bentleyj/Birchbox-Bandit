#include "ofApp.h"
#define NUM_PANELS_PER_COLUMN 10

//--------------------------------------------------------------
void ofApp::setup(){

	panelWidth = ofGetWidth() / 3.0;
	panelHeight = ofGetHeight() / 3.0;

	ofxNestedFileLoader loader;
	vector<string> imgPaths = loader.load("productImages");
	for (int i = 0; i < imgPaths.size(); i++) {
		ofImage* img = new ofImage();
		img->load(imgPaths[i]);
		productImages.push_back(img);
	}

	buffer.allocate(ofGetWidth(), ofGetHeight() * 2);
	PanelColumn* col1 = new PanelColumn();
	PanelColumn* col2 = new PanelColumn();
	PanelColumn* col3 = new PanelColumn();

	panelColumns.push_back(col1);
	panelColumns.push_back(col2);
	panelColumns.push_back(col3);

	for (int i = 0; i < panelColumns.size(); i++) {
		panelColumns[i]->emitter.initialVel = ofVec2f(0, 100);
		panelColumns[i]->emitter.pos = ofVec3f(i * panelWidth, -panelHeight * 2);
		panelColumns[i]->emitter.width = panelWidth;
		panelColumns[i]->emitter.height = panelHeight;
		panelColumns[i]->killDistance = ofGetHeight() + panelHeight + 10;
		panelColumns[i]->emitter.images = &productImages;
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	for (int i = 0; i < panelColumns.size(); i++) {
		panelColumns[i]->update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	buffer.begin();
	ofClear(0);
	ofPushMatrix();
	ofTranslate(0, buffer.getHeight() / 4.0);
	for (int i = 0; i < panelColumns.size(); i++) {
		panelColumns[i]->draw();
	}
	ofPopMatrix();

	ofPushStyle();
	ofNoFill();
	ofSetColor(0);
	ofDrawRectangle(0, 0, buffer.getWidth(), buffer.getHeight());
	ofSetLineWidth(5);
	ofDrawRectangle(0, buffer.getHeight()/4.0, buffer.getWidth(), buffer.getHeight()/2.0);
	ofPopStyle();
	buffer.end();

	buffer.draw(0, 0, buffer.getWidth()/2, buffer.getHeight() /2);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		for (int i = 0; i < panelColumns.size(); i++) {
			panelColumns[i]->createPanel();
		}
		for (int i = 0; i < panelColumns.size(); i++) {
			for (int j = 0; j < panelColumns[i]->panels.size(); j++) {
				panelColumns[i]->start();
			}
		}
	}
	if (key == '0') {
		panelColumns[0]->stop();
	}
	if (key == '1') {
		panelColumns[1]->stop();
	}	
	if (key == '2') {
		panelColumns[2]->stop();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
