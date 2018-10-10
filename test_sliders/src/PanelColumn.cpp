#include "PanelColumn.h"

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

void PanelColumn::draw() {
	emitter.draw();
	for (int i = 0; i < panels.size(); i++) {
		panels[i]->draw();
	}
}

void PanelColumn::createPanel() {
	panels.push_back(emitter.createPanel());
	if (panels.size() > 1) {
		panels[panels.size()-1]->nextPanel = panels[panels.size()-2];
	}
}

void PanelColumn::removeFirstPanel() {
	Panel* p1 = panels[0];
	if (!p1->stopped) {
		delete p1;
		panels.erase(panels.begin());
		p1 = panels[0];
		p1->nextPanel = nullptr;
	}
}

void PanelColumn::stop() {
	panels[0]->pos.target(ofVec2f(emitter.pos.x, killDistance - 10));
	panels[0]->stopped = true;
}

void PanelColumn::start() {
	panels[0]->vel = emitter.initialVel;
	panels[0]->stopped = false;
}