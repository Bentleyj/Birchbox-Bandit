#include "PanelEmitter.h"

// Update is empty for now.
void PanelEmitter::update() {

}

// Draw a debug version of our particle emitter
void PanelEmitter::draw() {
	ofPushStyle();
	ofNoFill();
	ofSetColor(255, 0, 0);
	ofDrawRectangle(pos, width, height);
	ofPopStyle();
}

Panel* PanelEmitter::createPanel( ofImage* img) {
	Panel* p = new Panel();

	p->pos = pos;

	p->width = width;

	p->height = height;

	p->maxVel = initialVel;

	p->vel = initialVel;

	p->acc = initialAcc;

	p->col = palette[ofRandom(0, palette.size())];

	p->img = img;

	p->stopped = false;

	return p;
}

Panel* PanelEmitter::createPanel() {
	Panel* p = new Panel();

	p->pos = pos;

	p->width = width;

	p->height = height;

	p->maxVel = initialVel;

	p->vel = initialVel;

	p->acc = initialAcc;

	p->col = palette[ofRandom(0, palette.size())];

	p->img = (*images)[ofRandom(0, images->size())];

	p->stopped = false;

	return p;
}

Panel* PanelEmitter::createPanel(int colIndex, ofImage* img) {
	Panel* p = new Panel();

	p->pos = pos;

	p->width = width;

	p->height = height;

	p->maxVel = initialVel;

	p->vel = initialVel;

	p->acc = initialAcc;

	p->col = palette[colIndex];

	p->img = img;

	p->stopped = false;

	return p;
}
