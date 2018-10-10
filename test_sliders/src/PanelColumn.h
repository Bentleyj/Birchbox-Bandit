#pragma once

// A Panel column is the main thing that controls our game. We'll have three of them.
// Each panel column includes a vector of panels and a panel emitter which creates panels.
// It also knows at what distance down the page to kill a panel, ie delete it from the list.

#include "ofMain.h"
#include "PanelEmitter.h"

class PanelColumn {
public:
	PanelColumn() {
		finalPanel = nullptr;
	}
	~PanelColumn() {}

	void update();
	void draw();
	void createPanel();
	void createFinalPanel();
	void removeFirstPanel();
	void stop();
	void start();
	void spin(int colIndex, int imgIndex);

	PanelEmitter emitter;
	vector<Panel*> panels;

	Panel* finalPanel;
	int stopColIndex = 0;
	int stopImgIndex = 0;

	float spinStartTime;
	float spinDuration;

	float killDistance;
private:
};