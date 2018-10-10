#include "PanelEmitter.h"

void PanelEmitter::update() {

}

void PanelEmitter::draw() {
	ofPushStyle();
	ofNoFill();
	ofSetColor(255, 0, 0);
	ofDrawRectangle(pos, width, height);
	ofPopStyle();
}

Panel* PanelEmitter::createPanel() {
	Panel* p = new Panel();

	p->pos = pos;

	p->width = width;

	p->height = height;

	p->maxVel = initialVel;

	p->deathThreshold = ofGetHeight() + height;

	p->vel = initialVel;

	p->acc = initialAcc;

	p->col = palette[int(ofRandom(5))];

	p->stopped = false;

	return p;
}