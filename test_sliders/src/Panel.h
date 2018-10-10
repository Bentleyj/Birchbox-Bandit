#pragma once

// A Panel describes a single 

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
	void start();
	void stop();

	Panel* nextPanel;
	ofColor col;
	float width;
	float height;
	Integrator<ofVec2f> pos;
	ofVec2f target;
	ofVec2f originalPos;
	bool stopped;
	ofVec2f vel;
	ofVec2f acc;
	ofVec2f maxVel;
	float deathThreshold;
private:
};