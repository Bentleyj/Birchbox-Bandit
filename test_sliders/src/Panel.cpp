#include "Panel.h"

// Update the panel's position.
void Panel::update() {
	// If we have a nextpanel, justs tick to it.
	if (nextPanel != nullptr) {
		pos.set(nextPanel->pos.val - ofVec2f(0, nextPanel->height));
	} else { // If we don't have a nextPanel then we are controlling our own motion.
		// If we're stopped seek to a target.
		if (stopped) {
			pos.update();
		} else { // If not spin at a speed!
			vel += acc;
			if (vel.x > maxVel.x) {
				vel.x = maxVel.x;
			}
			if (vel.y > maxVel.y) {
				vel.y = maxVel.y;
			}
			pos.set(pos.val + vel);
		}
	}
}

// Draw the panel.
void Panel::draw() {
	ofPushStyle();
	ofPushMatrix();
	ofSetColor(col);
	ofTranslate(pos.val);
	ofDrawRectangle(ofVec2f(0, 0), width, height);
	float scale = height / img->getHeight();
	float imgWidth = img->getWidth() * scale;
	float imgHeight = height;
	ofSetColor(255);
	img->draw((width - imgWidth) / 2.0, (height - imgHeight) / 2.0, imgWidth, imgHeight);
	ofNoFill();
	ofSetColor(0);
	ofDrawRectangle(ofVec2f(0, 0), width, height);
	ofPopMatrix();
	ofPopStyle();
}