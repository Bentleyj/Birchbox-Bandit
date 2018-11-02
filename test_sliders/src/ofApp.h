#pragma once

#include "ofMain.h"
#include "PanelColumn.h"
#include "ofxNestedFileLoader.h"
#include "ofxGui.h"

struct PanelImage {
	PanelImage() {
		img = new ofImage();
		videos.resize(0);
	}
	~PanelImage() {}
	string name;
	ofImage* img;
	vector<ofVideoPlayer* > videos;
	ofVideoPlayer* getRandomVideo() {
		return videos[ofRandom(0, videos.size())];
	}
	void playRandomVideo() {
		videos[ofRandom(0, videos.size())]->play();
	}

	void printState() {
		cout << "Name: " << name << endl;
		cout << "Num Sounds: " << videos.size() << endl;
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

		vector<ofImage* > allImages;

		vector<PanelImage* > panelImages;

		ofVideoPlayer* winVideo;
		ofSoundPlayer loseSound;
		ofSoundPlayer spinningSound, spinningSound2;

		ofImage frame;

		ofShader fade;

		bool winning;
		bool spinning;
		bool videoPlaying;

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
