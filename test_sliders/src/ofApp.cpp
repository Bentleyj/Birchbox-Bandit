#include "ofApp.h"
#define NUM_PANELS_PER_COLUMN 10

//--------------------------------------------------------------
void ofApp::setup(){

	ofHideCursor();

	panelWidth = ofGetWidth() / 3.0;
	panelHeight = ofGetHeight() / 2.0;

	frame.load("images/Frames/DLX Frame.png");

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
			if (fe == "jpg" || fe == "png") {
				pi->img->load(contentPaths[i]);
				allImages.push_back(pi->img);
			}
			else if (fe == "mov" || fe == "mp4") {
				ofVideoPlayer* p = new ofVideoPlayer();
				p->load(contentPaths[i]);
				pi->videos.push_back(p);
			}
			panelImages.push_back(pi);
		}
		else {
			if (fe == "jpg" || fe == "png") {
				pi->img->load(contentPaths[i]);
				allImages.push_back(pi->img);
			}
			else if (fe == "mov" || fe == "mp4") {
				ofVideoPlayer* p = new ofVideoPlayer();
				p->load(contentPaths[i]);
				pi->videos.push_back(p);
			}
		}
	}

	for (int i = 0; i < panelImages.size(); i++) {
		panelImages[i]->printState();
	}

	loseSound.load("sounds/No Win/342886__michael-kur95__time-s-up-03.wav");
	loseSound.setVolume(0.2);
	spinningSound.load("sounds/bongo_roll.wav");
	spinningSound.setVolume(0.5);

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
		panelColumns[i]->emitter.images = &allImages;
		panelColumns[i]->spinDuration = 2.0 + i;
		panelColumns[i]->stopSound.load("sounds/Clicks/chip_money.mp3");
		panelColumns[i]->stopSound.setVolume(0.5);

		panelColumns[i]->frame = &frame;
	}

	for (int i = 0; i < panelColumns.size(); i++) {
		panelColumns[i]->createPanel();
	}

	string settingsPath = "settings/probabilities.xml";
	gui.setup("Probability", settingsPath);
	gui.add(winChance.set("Win Chance", 0.5, 0.0, 1.0));
	gui.add(videoAlpha.set("Video Alpha", 0.0, 0.0, 255.0));
	gui.loadFromFile(settingsPath);
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
		winVideo->play();
		videoPlaying = true;
		spinning = false;
	}
	else if (!winning && allStopped && spinning) {
		loseSound.play();
		spinning = false;
	}
	if (winVideo != nullptr) {
		if (videoPlaying) {
			videoAlpha = ofLerp(videoAlpha, 255.0, 0.05);
			winVideo->update();
			if (winVideo->getCurrentFrame() >= winVideo->getTotalNumFrames() - 5) {
				winVideo->setPosition(0);
				winVideo->stop();
				videoPlaying = false;
			}
		}
	}
	if (!videoPlaying) {
		videoAlpha = ofLerp(videoAlpha, 0.0, 0.05);
	}
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

	fade.begin();
	fade.setUniformTexture("inputTexture", buffer.getTexture(), 0);
	fade.setUniform2f("resolution", buffer.getWidth(), buffer.getHeight());
	ofDrawRectangle(0, 0, buffer.getWidth(), buffer.getHeight());
	fade.end();

	if (winVideo != nullptr) {
		if (videoPlaying) {
			ofSetColor(255, videoAlpha);
			float x = 0;
			float y = 0;
			while (x < ofGetWidth()) {
				float w = ofGetWidth() / 3.0;
				float scale = w / winVideo->getWidth();
				float h = winVideo->getHeight() * scale;
				winVideo->draw(x, y, w, h);
				y += h;
				if (y > ofGetHeight()) {
					x += w;
					float d = ofGetHeight() - (y - h);
					y = -d;
				}
			}
		}
	}

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
	winVideo = panelImages[imgIndex]->getNextVideo();
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
			int newImgIndex = int(ofRandom(panelImages.size()));
			if (colIndex == newColIndex) {
				newColIndex = (newColIndex + 1) % 5;
			}
			if (imgIndex == newImgIndex) {
				newImgIndex = (newImgIndex + 1) % panelImages.size();
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
		int newImgIndex = int(ofRandom(1, panelImages.size()));
		if (colIndex == newColIndex) {
			colIndex = (newColIndex + 1) % 5;
		}
		if (imgIndex == newImgIndex) {
			imgIndex = (newImgIndex + 1) % panelImages.size();
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
	if (key == ' ') {
		if (winVideo != nullptr) {
			winVideo->setPosition(0);
			winVideo->stop();
			videoPlaying = false;
		}
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
