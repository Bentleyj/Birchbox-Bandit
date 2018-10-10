#pragma once

#include "ofMain.h"
#include "PanelEmitter.h"

class PanelColumn {
public:
	PanelColumn() {};
	~PanelColumn() {};

	void update();
	void draw();
	void createPanel();
	void removeFirstPanel();
	void stop();
	void start();


	PanelEmitter emitter;
	vector<Panel*> panels;

	float killDistance;

private:
};