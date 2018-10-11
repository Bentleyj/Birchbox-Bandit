#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings settings;
	settings.width = 1080 * 3;
	settings.height = 1920;
	settings.decorated = false;
	settings.setPosition(ofVec2f(1920, 0));
	settings.windowMode = OF_WINDOW;

	ofCreateWindow(settings);
	//ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
}
