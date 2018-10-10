#pragma once

#include "ofMain.h"
#include "Panel.h"

class PanelEmitter {
public:
	PanelEmitter() {
		palette[0] = ofColor(255, 173, 173);
		palette[1] = ofColor(255, 239, 170);
		palette[2] = ofColor(190, 255, 194);
		palette[3] = ofColor(163, 253, 255);
		palette[4] = ofColor(230, 175, 255);
	};
	~PanelEmitter() {};
	void update();
	void draw();
	Panel* createPanel();
	ofVec2f pos;
	ofVec2f initialVel;
	ofVec2f initialAcc;
	float width, height;

	ofColor palette[5];

private:
};