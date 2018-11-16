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
	vector<string> videos;
	int videoIndex = 0;
	string getNextVideo() {
		videoIndex++;
		videoIndex %= videos.size();
		return videos[videoIndex];
	}

	void printState() {
		cout << "Name: " << name << endl;
		cout << "Num Videos: " << videos.size() << endl;
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

		ofVideoPlayer winVideo;
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

		int imgIndex;

		float loadDelay = 0.5;
		bool loading;
		float spinStopTime;

		ofxPanel gui;

		ofParameter<float> winChance;
		ofParameter<float> videoAlpha;

		ofFbo buffer, fullScreen;
};
