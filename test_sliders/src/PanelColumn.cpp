#include "PanelColumn.h"

// Update all the panels and the emitter. Regularly create and delete panels.
void PanelColumn::update() {
	if (panels.size() > 0) {
		Panel* lastPanel = panels[panels.size() - 1];
		if (lastPanel->pos.val.y > emitter.pos.y + emitter.height) {
			lastPanel->pos.val.y = emitter.pos.y + emitter.height;
			createPanel();
		}
		Panel* firstPanel = panels[0];
		if (firstPanel->pos.val.y > killDistance) {
			removeFirstPanel();
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
}

// create a new panel and add it to your list of panels.
void PanelColumn::createPanel() {
	panels.push_back(emitter.createPanel());
	if (panels.size() > 1) {
		panels[panels.size()-1]->nextPanel = panels[panels.size()-2];
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

// Stop the motion of the lead panel and have it settle in to position.
void PanelColumn::stop() {
	panels[0]->pos.target(ofVec2f(emitter.pos.x, killDistance - 10));
	panels[0]->stopped = true;
}

// Start the motion of the lead panel with it's initial velocity.
void PanelColumn::start() {
	panels[0]->vel = emitter.initialVel;
	panels[0]->stopped = false;
}