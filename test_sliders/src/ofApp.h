#pragma once

#include "ofMain.h"
#include "PanelColumn.h"
#include "ofxNestedFileLoader.h"
#include "Sparticles.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void startWinningSpin();
		void startGrandPrizeSpin();
		void startLosingSpin();
		void startAlmostWinningSpin();
		void spawnParticles();

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

		vector<ofImage*> productImages;

		Sparticles sparticles;

		ofSoundPlayer winSound;
		ofSoundPlayer loseSound;
		ofSoundPlayer spinningSound, spinningSound2;

		ofImage frame;

		ofShader fade;

		bool winning;
		bool spinning;

		int numSpins;
		int numNormalWins;
		int numLoses;
		int numGrandPrizes;

		float panelHeight;
		float panelWidth;

		ofFbo buffer, fullScreen;
};
