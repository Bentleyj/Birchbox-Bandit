#include "ofApp.h"
#define NUM_PANELS_PER_COLUMN 10

//--------------------------------------------------------------
void ofApp::setup(){

	panelWidth = ofGetWidth() / 3.0;
	panelHeight = ofGetHeight() / 2.0;

	sparticles.setup();

	frame.load("images/Frame_001.png");

	fade.load("shaders/fade");

	ofxNestedFileLoader loader;
	vector<string> imgPaths = loader.load("productImages");
	for (int i = 0; i < imgPaths.size(); i++) {
		ofImage* img = new ofImage();
		img->load(imgPaths[i]);
		productImages.push_back(img);
	}

	winSound.load("sounds/Big Win/JackPot SFX 4 (MP3).mp3");
	loseSound.load("sounds/No Win/342886__michael-kur95__time-s-up-03.wav");
	//spinningSound.load("sounds/Tension Builder/slot_machine_spin.mp3");
	spinningSound.load("sounds/Tension Builder/TensionBuildBongosSlot.wav");

	buffer.allocate(ofGetWidth(), ofGetHeight());
	PanelColumn* col1 = new PanelColumn();
	PanelColumn* col2 = new PanelColumn();
	PanelColumn* col3 = new PanelColumn();

	panelColumns.push_back(col1);
	panelColumns.push_back(col2);
	panelColumns.push_back(col3);

	for (int i = 0; i < panelColumns.size(); i++) {
		panelColumns[i]->emitter.initialVel = ofVec2f(0, 200);
		panelColumns[i]->emitter.pos = ofVec3f(i * panelWidth, -panelHeight * 1.5);
		panelColumns[i]->emitter.width = panelWidth;
		panelColumns[i]->emitter.height = panelHeight;
		panelColumns[i]->killDistance = ofGetHeight() + panelHeight * 1.5 + 10;
		panelColumns[i]->emitter.images = &productImages;
		panelColumns[i]->spinDuration = 2.0 + i;
		panelColumns[i]->stopSound.load("sounds/Clicks/chip_money.mp3");
		panelColumns[i]->frame = &frame;
	}

	for (int i = 0; i < panelColumns.size(); i++) {
		panelColumns[i]->createPanel();
	}

	//panelColumns[2]->emitter.spawnSound.load("sounds/Clicks/click 2.wav");
}

//--------------------------------------------------------------
void ofApp::update(){
	bool allStopped = true;
	for (int i = 0; i < panelColumns.size(); i++) {
		panelColumns[i]->update();
		if (!panelColumns[i]->stopped) {
			allStopped = false;
		}
	}
	if (winning && allStopped && spinning) {
		winSound.play();
		spinning = false;
		for (int i = 0; i < 1000; i++) {
			float maxRad = 50;
			float r = ofRandom(maxRad);
			float a = ofRandom(360);
			float offsetX = ofGetWidth()/2;
			float offsetY = ofGetHeight() / 2;
			if (ofRandom(1) > 0.5) {
				offsetX = ofGetWidth() / 2;
			}
			else if (ofRandom(1) > 0.5) {
				offsetX = ofGetWidth() / 6;
			}
			else {
				offsetX = ofGetWidth() * 5 / 6;
			}
			float x = offsetX + r * cos(a) - maxRad/2.0;
			float y = offsetY + r * sin(a) - maxRad/2.0;
			float dir = ofRandom(0, 360);
			float mag = ofRandom(0, 80);
			float dx = mag * cos(dir);
			float dy = mag * sin(dir);
			float dxx = 0.0;
			float dyy = ofRandom(0.5, 2.0);
			sparticles.spawn(x, y, dx, dy, dxx, dyy);
		}

	}
	else if (!winning && allStopped && spinning) {
		loseSound.play();
		spinning = false;
	}
	sparticles.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	buffer.begin();
	ofClear(0);
	ofPushMatrix();
	for (int i = 0; i < panelColumns.size(); i++) {
		panelColumns[i]->draw();
		frame.draw(panelColumns[i]->emitter.pos.x, 0);
	}
	sparticles.draw(2.0, 2.0);
	ofPopMatrix();
	buffer.end();

	buffer.draw(0, 0);

	//fade.begin();
	//fade.setUniformTexture("inputTexture", buffer.getTexture(), 0);
	//fade.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	//ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	//fade.end();

	//ofNoFill();
	//ofSetColor(0);
	//ofDrawRectangle(0, 0, buffer.getWidth(), buffer.getHeight());
	//ofSetLineWidth(5);
	//ofDrawRectangle(0, buffer.getHeight() / 4.0, buffer.getWidth(), buffer.getHeight() / 2.0);
	//ofPopStyle();
	//buffer.end();

	//buffer.draw(0, 0, buffer.getWidth() / 2, buffer.getHeight() / 2);
}

//--------------------------------------------------------------
void ofApp::startWinningSpin() {
	int colIndex = int(ofRandom(5));
	int imgIndex = int(ofRandom(productImages.size()));
	for (int i = 0; i < panelColumns.size(); i++) {
		panelColumns[i]->spin(colIndex, imgIndex);
	}
	spinning = true;
	winning = true;
	spinningSound.play();
}

//--------------------------------------------------------------
void ofApp::startAlmostWinningSpin() {
	int colIndex = int(ofRandom(5));
	int imgIndex = int(ofRandom(productImages.size()));
	for (int i = 0; i < panelColumns.size(); i++) {
		if (i == panelColumns.size() - 1) {
			int newColIndex = int(ofRandom(5));
			int newImgIndex = int(ofRandom(productImages.size()));
			if (colIndex == newColIndex && imgIndex == newImgIndex) {
				colIndex = (newColIndex + 1) % 5;
				imgIndex = (newImgIndex + 1) % productImages.size();
			}
			else {
				colIndex = newColIndex;
				imgIndex = newImgIndex;
			}
		}
		panelColumns[i]->spin(colIndex, imgIndex);
	}
	spinning = true;
	winning = false;
	spinningSound.play();
}

//--------------------------------------------------------------
void ofApp::startLosingSpin() {
	int colIndex = int(ofRandom(5));
	int imgIndex = int(ofRandom(productImages.size()));
	for (int i = 0; i < panelColumns.size(); i++) {
		int newColIndex = int(ofRandom(5));
		int newImgIndex = int(ofRandom(productImages.size()));
		if (colIndex == newColIndex && imgIndex == newImgIndex) {
			colIndex = (newColIndex + 1) % 5;
			imgIndex = (newImgIndex + 1) % productImages.size();
		}
		else {
			colIndex = newColIndex;
			imgIndex = newImgIndex;
		}
		panelColumns[i]->spin(colIndex, imgIndex);
	}
	spinning = true;
	winning = false;
	spinningSound.play();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		if (ofRandom(1) > 0.2) {
			if (ofRandom(1) > 0.4) {
				startLosingSpin();
			}
			else {
				startAlmostWinningSpin();
			}
		}
		else {
			startWinningSpin();
		}
	}
	else if (key == 'w') {
		startWinningSpin();
	}
	else if (key == 'l') {
		startLosingSpin();
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
