#include "ofApp.h"
#define NUM_PANELS_PER_COLUMN 10

//--------------------------------------------------------------
void ofApp::setup(){
	// Hide the cursor when it is over our frame.
	ofHideCursor();

	// Set the panel width and panel height (THESE VALUES ARE IMPORTANT AND WILL CHANGE WHERE THE IMAGES STOP AFTER A SPIN!)
	// Todo: Remove the importance of these.
	panelWidth = ofGetWidth() / 3.0;
	panelHeight = ofGetHeight() / 2.0;

	// Setup our two different sets of particles by passing them different particles images.
	sparticles.setup("images/particles");
	grandPrizeSparticles.setup("images/GrandPrizeParticles");

	// Load our frame image for the frame around the outside.
	frame.load("images/Frames/Asset 3.png");

	// load our fade shader,
	fade.load("shaders/fade");

	// load all the product images and push them back on to the list of product image pointers.
	ofxNestedFileLoader loader;
	vector<string> imgPaths = loader.load("productImages");
	string grandPrize = "Win Coin B.png"; // Load our grand prize image separately.
	ofImage* img = new ofImage();
	img->load(grandPrize);  
	productImages.push_back(img); // make the grand prize image the first on our list of images so we can easily ignore it when selecting a random prize.
	for (int i = 0; i < imgPaths.size(); i++) {
		ofImage* img = new ofImage();
		img->load(imgPaths[i]);
		productImages.push_back(img);
	}

	// Load all our sounds.
	winSound.load("sounds/Big Win/JackPot SFX 4 (MP3).mp3");
	loseSound.load("sounds/No Win/342886__michael-kur95__time-s-up-03.wav");
	grandPrizeSound.load("sounds/Grand Prize/MusicFanfare SME01_13.1.wav");	//spinningSound.load("sounds/Tension Builder/slot_machine_spin.mp3");
	spinningSound.load("sounds/Tension Builder/TensionBuildBongosSlotButton2.wav");

	// Allocate our draw buffer for our shader to act on.
	buffer.allocate(ofGetWidth(), ofGetHeight());
	
	// Setup our three panel columns and push them back on to our vector.
	PanelColumn* col1 = new PanelColumn();
	PanelColumn* col2 = new PanelColumn();
	PanelColumn* col3 = new PanelColumn();

	panelColumns.push_back(col1);
	panelColumns.push_back(col2);
	panelColumns.push_back(col3);

	// setup each panel column, I've avoided a magic number here so everything is clear but you could wrap the above lines in to this loop.
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

	// Kick off panel creation. If each panel column creates an initial panel at the beginning they will then start cascading.
	for (int i = 0; i < panelColumns.size(); i++) {
		panelColumns[i]->createPanel();
	}

	// Setup our gui.
	string settingsPath = "settings/probabilities.xml";
	gui.setup("Probability", settingsPath);
	gui.add(winChance.set("Win Chance", 0.5, 0.0, 1.0));
	gui.add(gpChance.set("Grand Prize Chance", 0.01, 0.0, 0.1));
	gui.loadFromFile(settingsPath);
}

//--------------------------------------------------------------
void ofApp::update(){
	// Work out if all the panels have stopped.
	bool allStopped = true;
	for (int i = 0; i < panelColumns.size(); i++) {
		panelColumns[i]->update();
		if (!panelColumns[i]->stopped) {
			allStopped = false;
		}
	}
	// Check if we've finished a winning spin.
	if (winning && allStopped && spinning) {
		// Chekc if we won the grand prize!
		if (winningGrandPrize) {
			winSound.play();
			grandPrizeSound.play();
			spawnParticles(&grandPrizeSparticles, 40, 40);
		}
		// If not a grand prize we've won a normal prize.
		else {
			winSound.play();
			spawnParticles(&sparticles, 40, 40);
		}
		// Set spinning to false to note that we've finished a spin and are waiting for a new button press.
		spinning = false;
	}
	// If not we've finished a losing spin
	else if (!winning && allStopped && spinning) {
		loseSound.play();
		// Set spinning to false to note that we've finished a spin and are waiting for a new button press.
		spinning = false;
	}

	// Update our particles so they move across the screen.
	sparticles.update();
	grandPrizeSparticles.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	// Start our buffer in to which we will draw our panel columns.
	buffer.begin();
	ofClear(0);
	ofPushMatrix();
	// draw our panel columns in to our buffer
	for (int i = 0; i < panelColumns.size(); i++) {
		panelColumns[i]->draw();
	}
	ofPopMatrix();
	buffer.end();

	// apply our fade effect to our buffer.
	fade.begin();
	fade.setUniformTexture("inputTexture", buffer.getTexture(), 0);
	fade.setUniform2f("resolution", buffer.getWidth(), buffer.getHeight());
	ofDrawRectangle(0, 0, buffer.getWidth(), buffer.getHeight());
	fade.end();

	// draw our particles on top of the fade.
	sparticles.draw(2.0, 2.0);
	grandPrizeSparticles.draw(2.0, 2.0);

	// Draw our outer frame 3 times on top of each panelColumn and on top of the particles.
	for (int i = 0; i < panelColumns.size(); i++) {
		frame.draw(panelColumns[i]->emitter.pos.x, 0, buffer.getWidth()/3, buffer.getHeight());
	}

	// Draw our gui and statistics if necessary.
	if (drawGui) {
		int y = 10;
		ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate()), gui.getPosition().x + gui.getWidth(), y += 20);

		ofDrawBitmapStringHighlight("numSpins: " + ofToString(numSpins), gui.getPosition().x + gui.getWidth(), y += 20);
		ofDrawBitmapStringHighlight("numWins: " + ofToString(numNormalWins), gui.getPosition().x + gui.getWidth(), y += 20);
		ofDrawBitmapStringHighlight("numLoses: " + ofToString(numLoses), gui.getPosition().x + gui.getWidth(), y += 20);
		ofDrawBitmapStringHighlight("numGrandPrizes: " + ofToString(numGrandPrizes), gui.getPosition().x + gui.getWidth(), y += 20);
		ofDrawBitmapStringHighlight("winRate: " + ofToString(float(numNormalWins) / float(max(numSpins, 1))), gui.getPosition().x + gui.getWidth(), y += 20);
		ofDrawBitmapStringHighlight("gpRate: " + ofToString(float(numGrandPrizes) / float(max(numSpins, 1))), gui.getPosition().x + gui.getWidth(), y += 20);

		gui.draw();
	}
}

//--------------------------------------------------------------
// This method kicks off a winning spin.
void ofApp::startWinningSpin() {
	int colIndex = int(ofRandom(5)); // pick a random color
	int imgIndex = int(ofRandom(1, productImages.size())); // pick a random image. (not the grand prize image)
	for (int i = 0; i < panelColumns.size(); i++) {
		panelColumns[i]->spin(colIndex, imgIndex); // Tell the each panel Column to spin with the a matching color and image.
	}
	// set state booleans
	spinning = true;
	winning = true;
	winningGrandPrize = false;

	// play our spinning sound.
	spinningSound.play();
}

//--------------------------------------------------------------
// This method kicks off an "Almost" winning spin. This means that the first two panels will be identical but the third one will be different.
// We want to manually force this because otherwise the chance of this happening is quite rare, so you basically know once the second image has come up wether or not
// You will be a winner. This means you need to wait for the last panel to know if you've won.
void ofApp::startAlmostWinningSpin() {
	int colIndex = int(ofRandom(5)); // pick and random color.
	int imgIndex = int(ofRandom(0, productImages.size())); // pick a random image (could be the grand prize image)
	for (int i = 0; i < panelColumns.size(); i++) {
		// If its the third panel pick a different image and color.
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
		panelColumns[i]->spin(colIndex, imgIndex); // Start the spin on each panel with the same image and color for the first two and a different one for the third.
	}
	// set state booleans
	spinning = true;
	winning = false;
	winningGrandPrize = false;

	// Play spinning sound.
	spinningSound.play();
}

//--------------------------------------------------------------
// This method kicks off a grand prize winning spin.
void ofApp::startGrandPrizeSpin() {
	int colIndex = int(ofRandom(5));
	int imgIndex = 0;
	for (int i = 0; i < panelColumns.size(); i++) {
		panelColumns[i]->spin(colIndex, imgIndex);
	}
	spinning = true;
	winning = true;
	winningGrandPrize = true;
	spinningSound.play();
}


//--------------------------------------------------------------
void ofApp::startLosingSpin() {
	int colIndex = int(ofRandom(5));
	int imgIndex = int(ofRandom(0, productImages.size()));
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
		panelColumns[i]->spin(colIndex, imgIndex);
	}
	spinning = true;
	winning = false;
	winningGrandPrize = false;
	spinningSound.play();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	sparticles.killAllSparticles();
	grandPrizeSparticles.killAllSparticles();

	if (key == ' ') {
		if (!spinning) {
			numSpins++;
			if (ofRandom(1) > 1.0 - gpChance) {
				startGrandPrizeSpin();
				numGrandPrizes++;
			}
			else {
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
		}
	}
	else if (key == 'w') {
		startWinningSpin();
	}
	else if (key == 'g') {
		startGrandPrizeSpin();
	}
	else if (key == 'l') {
		startLosingSpin();
	} 
	else if (key == 's') {
		drawGui = !drawGui;
	}
}

//--------------------------------------------------------------
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
