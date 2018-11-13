#pragma once

#include "ofMain.h"
#include "PanelColumn.h"
#include "ofxNestedFileLoader.h"
#include "Sparticles.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		// Custom functions to control spinning and particles.
		void startWinningSpin();
		void startGrandPrizeSpin();
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


		// Vector of pointers to panel columns.
		vector<PanelColumn* > panelColumns;

		// Vector of product images which will be passed individually to each panel emitter. 
		// Don't need to load these one time for each emitter, just load once and pass.
		vector<ofImage*> productImages;

		// Sparticles for the normal win and the grand prize.
		Sparticles sparticles, grandPrizeSparticles;

		// Sounds for win, grand prize, loss, spinning etc.
		ofSoundPlayer winSound;
		ofSoundPlayer grandPrizeSound;
		ofSoundPlayer loseSound;
		ofSoundPlayer spinningSound;

		// Frame that goes around the outside of everything. This is drawn on-top of each panel Column.
		ofImage frame;

		// a fade shader for the top and bottom of the panels.
		ofShader fade;

		// booleans to control win checking.
		bool winning;
		bool winningGrandPrize;
		bool spinning;

		// Statistics to keep track of the system and check our win %s
		int numSpins;
		int numNormalWins;
		int numLoses;
		int numGrandPrizes;

		// Width and height of each panel (THIS IS ACTUALLY REALLY IMPORTANT!)
		// Our system isn't perfect and if we modify these a lot then we will change our system.
		float panelHeight;
		float panelWidth;

		// boolean to control drawing our GUI
		bool drawGui = false;

		// Gui object
		ofxPanel gui;

		// Parameters we have control over.
		ofParameter<float> winChance;
		ofParameter<float> gpChance;

		// Buffers for drawing our screen at different sizes.
		ofFbo buffer, fullScreen;
};
