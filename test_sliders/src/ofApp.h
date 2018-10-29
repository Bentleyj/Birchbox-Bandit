#pragma once

#include "ofMain.h"
#include "PanelColumn.h"
#include "ofxNestedFileLoader.h"
#include "Sparticles.h"
#include "ofxGui.h"

struct PanelImage {
	PanelImage() {
		img = new ofImage();
		sounds.resize(0);
	}
	~PanelImage() {}
	string name;
	ofImage* img;
	vector<ofSoundPlayer* > sounds;
	ofSoundPlayer* getRandomSound() {
		return sounds[ofRandom(0, sounds.size())];
	}
	void playRandomSound() {
		sounds[ofRandom(0, sounds.size())]->play();
	}
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void startWinningSpin();
		void startLosingSpin();
		void startAlmostWinningSpin();
		void spawnParticles(Sparticles* particles, float delayMin, float delayMax);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		vector<PanelColumn* > panelColumns;

		vector<ofImage* > productImages;

		vector<PanelImage* > panelImages;

		Sparticles sparticles, grandPrizeSparticles;

		ofSoundPlayer* winSound;
		ofSoundPlayer loseSound;
		ofSoundPlayer spinningSound, spinningSound2;

		ofImage frame;

		ofShader fade;

		bool winning;
		bool spinning;

		int numSpins;
		int numNormalWins;
		int numLoses;

		float panelHeight;
		float panelWidth;

		bool drawGui = false;

		ofxPanel gui;

		ofParameter<float> winChance;

		ofFbo buffer, fullScreen;
};
