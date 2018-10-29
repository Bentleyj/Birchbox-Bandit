#pragma once

// A Panel describes a single panel in our slot machine.
// A Panel can draw itself and update it's own position.
// A Panel's position can be controlled in one of two ways, either it updates it's own position directly,
// or there is a parent panel (called nextPanel) to which is it glued.

#include "ofMain.h"
#include "ofxIntegrator.h"

class Panel {
public:
	Panel() {
		nextPanel = nullptr;
	}
	~Panel() {}
	void update();
	void draw();

	Panel* nextPanel;
	ofColor col;
	float width;
	float height;
	Integrator<ofVec2f> pos;
	ofVec2f target;
	bool stopped;
	ofVec2f vel;
	ofVec2f acc;
	ofVec2f maxVel;
	ofImage* img;
private:
};