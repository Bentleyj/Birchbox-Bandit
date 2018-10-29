#include "PanelColumn.h"

// Update all the panels and the emitter. Regularly create and delete panels.
void PanelColumn::update() {
	if (panels.size() > 0) {
		Panel* lastPanel = panels[panels.size() - 1];
		if (lastPanel->pos.val.y > emitter.pos.y + emitter.height) {
			if (ofGetElapsedTimef() - spinStartTime > spinDuration) {
				createFinalPanel();
				spinStartTime = ofGetElapsedTimef();
			}
			createPanel();
		}
		Panel* firstPanel = panels[0];
		if (firstPanel->pos.val.y > killDistance) {
			removeFirstPanel();
		}
		if (finalPanel != nullptr) {
			if (finalPanel->pos.val.y > finalPanel->height / 2.0 - emitter.initialVel.y) {
				stop();
				finalPanel = nullptr;
			}
		}
	}
	emitter.update();
	for (int i = 0; i < panels.size(); i++) {
		panels[i]->update();
	}
}

// Draw all the panels and the emitter
void PanelColumn::draw() {
	emitter.draw();
	for (int i = 0; i < panels.size(); i++) {
		panels[i]->draw();
	}
	//frame->draw(emitter.pos.x, emitter.pos.y + emitter.height * 1.5);
}

// create a new panel and add it to your list of panels.
void PanelColumn::createPanel() {
	panels.push_back(emitter.createPanel());
	if (panels.size() > 1) {
		panels[panels.size()-1]->nextPanel = panels[panels.size()-2];
	}
}

void PanelColumn::createFinalPanel() {
	Panel* p = emitter.createPanel(stopColIndex, stopImg);
	finalPanel = p;
	panels.push_back(p);
	if (panels.size() > 1) {
		panels[panels.size() - 1]->nextPanel = panels[panels.size() - 2];
	}
}

// remove the first panel from the list, like a pop_front command for a vector.
// Also don't forget to set the nextPanel field of the new first panel to nullptr.
void PanelColumn::removeFirstPanel() {
	Panel* p1 = panels[0];
	if (!p1->stopped) {
		delete p1;
		panels.erase(panels.begin());
		p1 = panels[0];
		p1->nextPanel = nullptr;
	}
}

void PanelColumn::spin(int colIndex, ofImage* img) {
	stopColIndex = colIndex;
	stopImg = img;
	spinStartTime = ofGetElapsedTimef();
	start();
}

// Stop the motion of the lead panel and have it settle in to position.
void PanelColumn::stop() {
	if (panels.size() > 0) {
		panels[0]->pos.target(ofVec2f(emitter.pos.x, killDistance - 10));
		panels[0]->stopped = true;
		stopped = true;
		stopSound.play();
	}
}

// Start the motion of the lead panel with it's initial velocity.
void PanelColumn::start() {
	if (panels.size() > 0) {
		panels[0]->vel = ofVec2f(0, -10);
		panels[0]->acc = ofVec2f(0, emitter.initialVel.y / 200.0);
		panels[0]->stopped = false;
		stopped = false;
	}
}