#pragma once

// A Panel Emitter creates new panels that go in to the panel column. It contains a list of all the colors
// And images that a panel can have and creates them randomly or in winning order.

#include "ofMain.h"
#include "Panel.h"

class PanelEmitter {
public:
	PanelEmitter() {
		palette.resize(5);
		palette[0] = ofColor(241, 217, 177);
		palette[1] = ofColor(236, 175, 191);
		palette[2] = ofColor(184, 223, 229);
		palette[3] = ofColor(192, 210, 198);
		palette[4] = ofColor(244, 160, 147);
	};
	~PanelEmitter() {};
	void update();
	void draw();
	Panel* createPanel();
	Panel* createPanel(int colIndex, int imgIndex);

	ofVec2f pos;
	ofVec2f initialVel;
	ofVec2f initialAcc;
	float width, height;

	ofSoundPlayer spawnSound;

	vector<ofColor> palette;
	vector<ofImage*>* images;

private:
};