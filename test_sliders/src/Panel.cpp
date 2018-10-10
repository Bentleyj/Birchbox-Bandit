#include "Panel.h"

void Panel::update() {
	if (!stopped) {
		if (nextPanel == nullptr) {
			vel += acc;
			if (vel.x > maxVel.x) {
				vel.x = maxVel.x;
			}
			if (vel.y > maxVel.y) {
				vel.y = maxVel.y;
			}
			pos.set(pos.val + vel);
		}
		else {
			pos.set(nextPanel->pos.val - ofVec2f(0, nextPanel->height));
		}
	}
	if (stopped) {
		if (nextPanel == nullptr) {
			pos.update();
		}
		else {
			pos.set(nextPanel->pos.val - ofVec2f(0, nextPanel->height));

		}
	}
}

void Panel::start() {
	acc = ofVec2f(0, 5);
	stopped = false;
}

void Panel::stop() {
	acc = ofVec2f(0);
	vel = ofVec2f(0);
	float tx = pos.val.x;
	float ty = (originalPos.y - pos.val.y);
	ty = fmod(ty, height);
	ty = pos.val.y + ty;
	pos.target(ofVec2f(tx, ty));
	stopped = true;
}

void Panel::draw() {
	ofPushStyle();
	ofSetColor(col);
	ofDrawRectangle(pos.val, width, height);
	ofNoFill();
	ofSetColor(0);
	ofDrawRectangle(pos.val, width, height);
	ofPopStyle();
}