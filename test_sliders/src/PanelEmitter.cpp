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

// Instantiate a panel on the heap and return a pointer to it.
Panel* PanelEmitter::createPanel() {
	Panel* p = new Panel();

	p->pos = pos;

	p->pos.attraction = 0.5;

	p->pos.damping = 0.8;

	p->width = width;

	p->height = height;

	p->maxVel = initialVel;

	p->vel = initialVel;

	p->acc = initialAcc;

	p->col = palette[int(ofRandom(5))];

	p->img = (*images)[int(ofRandom(images->size()))];

	p->stopped = false;

	if (spawnSound.isLoaded()) {
		spawnSound.play();
	}

	return p;
}

// Create a panel with a specific color and image.
Panel* PanelEmitter::createPanel(int colIndex, int imgIndex) {
	Panel* p = new Panel();

	p->pos = pos;

	p->width = width;

	p->height = height;

	p->maxVel = initialVel;

	p->vel = initialVel;

	p->acc = initialAcc;

	p->col = palette[colIndex];

	p->img = (*images)[imgIndex];

	p->stopped = false;

	return p;
}
