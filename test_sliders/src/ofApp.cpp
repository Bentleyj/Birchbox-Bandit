#include "ofApp.h"
#define NUM_PANELS_PER_COLUMN 10

//--------------------------------------------------------------
void ofApp::setup(){

	panelWidth = ofGetWidth() / 3.0;
	panelHeight = ofGetHeight() / 2.0;

	sparticles.setup("images/particles");
	grandPrizeSparticles.setup("images/GrandPrizeParticles");

	frame.load("images/Frames/Frame_with Colours.png");

	fade.load("shaders/fade");

	ofxNestedFileLoader loader;
	vector<string> contentPaths = loader.load("characters");

	for (int i = 0; i < contentPaths.size(); i++) {
		vector<string> fs = loader.getFolderStructure(contentPaths[i]);
		string fe = loader.getFileExtension(contentPaths[i]);
		string name = fs[1];
		PanelImage* pi = nullptr;
		for (int i = 0; i < panelImages.size(); i++) {
			if (name == panelImages[i]->name) {
				pi = panelImages[i];
				break;
			}
		}
		if (pi == nullptr) {
			pi = new PanelImage();
			pi->name = name;
			if (fe == "jpg") {
				pi->img->load(contentPaths[i]);
			}
			else if (fe == "wav") {
				ofSoundPlayer* p = new ofSoundPlayer();
				pi->sounds.push_back(p);
			}
			panelImages.push_back(pi);
		}
		else {
			if (fe == "jpg") {
				pi->img->load(contentPaths[i]);
			}
			else if (fe == "wav") {
				ofSoundPlayer* p = new ofSoundPlayer();
				pi->sounds.push_back(p);
			}
		}
	}

	loseSound.load("sounds/No Win/342886__michael-kur95__time-s-up-03.wav");
	spinningSound.load("sounds/Tension Builder/TensionBuildBongosSlotButton2.wav");

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
		panelColumns[i]->spinDuration = 2.0 + i;
		panelColumns[i]->stopSound.load("sounds/Clicks/chip_money.mp3");
		panelColumns[i]->frame = &frame;
	}

	for (int i = 0; i < panelColumns.size(); i++) {
		panelColumns[i]->createPanel();
	}

	string settingsPath = "settings/probabilities.xml";
	gui.setup("Probability", settingsPath);
	gui.add(winChance.set("Win Chance", 0.5, 0.0, 1.0));
	gui.loadFromFile(settingsPath);
}

void ofApp::spawnParticles(Sparticles* particles, float delayMin, float delayMax)
{
	for (int i = 0; i < 1000; i++) {
		float maxRad = 50;
		float r = ofRandom(maxRad);
		float a = ofRandom(360);
		float offsetX = ofGetWidth() / 2;
		float offsetY = ofGetHeight() / 2;
		float dir = ofRandom(0, 360);
		float mag = 0;

		if (ofRandom(1) > 0.5) {
			offsetX = ofGetWidth() / 2;
			mag = ofRandom(50, 80);
		}
		else if (ofRandom(1) > 0.5) {
			offsetX = ofGetWidth() / 6;
			mag = ofRandom(30, 60);
		}
		else {
			offsetX = ofGetWidth() * 5 / 6;
			mag = ofRandom(30, 60);
		}
		float x = offsetX + r * cos(a) - maxRad / 2.0;
		float y = offsetY + r * sin(a) - maxRad / 2.0;

		float dx = mag * cos(dir);
		float dy = mag * sin(dir);
		float dxx = 0.0;
		float dyy = ofRandom(0.5, 2.0);
		float delay = ofRandom(delayMin, delayMax);
		particles->spawn(x, y, dx, dy, dxx, dyy, delay);
	}
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
		winSound->play();
		spawnParticles(&sparticles, 40, 40);
		spinning = false;
	}
	else if (!winning && allStopped && spinning) {
		loseSound.play();
		spinning = false;
	}

	sparticles.update();
	grandPrizeSparticles.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	buffer.begin();
	ofClear(0);
	ofPushMatrix();
	for (int i = 0; i < panelColumns.size(); i++) {
		panelColumns[i]->draw();
	}
	ofPopMatrix();
	buffer.end();

	//buffer.draw(0, 0);

	//ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate()), 10, 10);

	fade.begin();
	fade.setUniformTexture("inputTexture", buffer.getTexture(), 0);
	fade.setUniform2f("resolution", buffer.getWidth(), buffer.getHeight());
	ofDrawRectangle(0, 0, buffer.getWidth(), buffer.getHeight());
	fade.end();

	sparticles.draw(2.0, 2.0);
	grandPrizeSparticles.draw(2.0, 2.0);


	for (int i = 0; i < panelColumns.size(); i++) {
		frame.draw(panelColumns[i]->emitter.pos.x, 0, buffer.getWidth()/3, buffer.getHeight());
	}

	if (drawGui) {
		int y = 10;
		ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate()), gui.getPosition().x + gui.getWidth(), y += 20);

		ofDrawBitmapStringHighlight("numSpins: " + ofToString(numSpins), gui.getPosition().x + gui.getWidth(), y += 20);
		ofDrawBitmapStringHighlight("numWins: " + ofToString(numNormalWins), gui.getPosition().x + gui.getWidth(), y += 20);
		ofDrawBitmapStringHighlight("numLoses: " + ofToString(numLoses), gui.getPosition().x + gui.getWidth(), y += 20);
		ofDrawBitmapStringHighlight("winRate: " + ofToString(float(numNormalWins) / float(max(numSpins, 1))), gui.getPosition().x + gui.getWidth(), y += 20);

		gui.draw();
	}
}

//--------------------------------------------------------------
void ofApp::startWinningSpin() {
	int colIndex = int(ofRandom(5));
	int imgIndex = int(ofRandom(0, panelImages.size()));
	winSound = panelImages[imgIndex]->getRandomSound();
	for (int i = 0; i < panelColumns.size(); i++) {
		panelColumns[i]->spin(colIndex, panelImages[imgIndex]->img);
	}
	spinning = true;
	winning = true;
	spinningSound.play();
}

//--------------------------------------------------------------
void ofApp::startAlmostWinningSpin() {
	int colIndex = int(ofRandom(5));
	int imgIndex = int(ofRandom(0, panelImages.size()));
	for (int i = 0; i < panelColumns.size(); i++) {
		if (i == panelColumns.size() - 1) {
			int newColIndex = int(ofRandom(5));
			int newImgIndex = int(ofRandom(productImages.size()));
			if (colIndex == newColIndex) {
				newColIndex = (newColIndex + 1) % 5;
			}
			if (imgIndex == newImgIndex) {
				newImgIndex = (newImgIndex + 1) % productImages.size();
			}
			colIndex = newColIndex;
			imgIndex = newImgIndex;
		}
		panelColumns[i]->spin(colIndex, panelImages[imgIndex]->img);
	}
	spinning = true;
	winning = false;
	spinningSound.play();
}

//--------------------------------------------------------------
void ofApp::startLosingSpin() {
	int colIndex = int(ofRandom(5));
	int imgIndex = int(ofRandom(0, panelImages.size()));
	for (int i = 0; i < panelColumns.size(); i++) {
		int newColIndex = int(ofRandom(5));
		int newImgIndex = int(ofRandom(1, productImages.size()));
		if (colIndex == newColIndex) {
			colIndex = (newColIndex + 1) % 5;
		}
		if (imgIndex == newImgIndex) {
			imgIndex = (newImgIndex + 1) % productImages.size();
		}
		else {
			colIndex = newColIndex;
			imgIndex = newImgIndex;
		}
		panelColumns[i]->spin(colIndex, panelImages[imgIndex]->img);
	}
	spinning = true;
	winning = false;
	spinningSound.play();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	sparticles.killAllSparticles();
	grandPrizeSparticles.killAllSparticles();
	if (key == ' ') {
		numSpins++;
		if (ofRandom(1) > winChance) {
			if (ofRandom(1) > 0.4) {
				startLosingSpin();
			}
			else {
				startAlmostWinningSpin();
			}
			numLoses++;
		}
		else {
			startWinningSpin();
			numNormalWins++;
		}
	}
	else if (key == 'w') {
		startWinningSpin();
	}
	else if (key == 'l') {
		startLosingSpin();
	} 
	else if (key == 'g') {
		drawGui = !drawGui;
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
