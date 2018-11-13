/**
 * == Simplified BSD *** MODIFIED FOR NON-COMMERCIAL USE ONLY!!! *** ==
 * Copyright (c) 2011, Cariad Interactive LTD
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice, this list of
 *     conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright notice, this list of
 *     conditions and the following disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *   * Any redistribution, use, or modification is done solely for personal benefit and not for any
 *     commercial purpose or for monetary gain
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "Sparticles.h"

Sparticles::Sparticles(){
    pos = 0;
    maxAge = 200;
    numParticles = 2000;
    positions = new ofVec2f[numParticles];
	velocities = new ofVec2f[numParticles];
	accelerations = new ofVec2f[numParticles];
    rotations = new float[numParticles];
    rotationSpeeds = new float[numParticles];
    ages = new int[numParticles];
    sizes = new float[numParticles];
	delays = new float[numParticles];
	images.resize(numParticles);
    
    for(int i = 0; i < numParticles; i++) {
        positions[i] = ofVec2f(-10, -10);
        velocities[i] = ofVec2f(0, 0);
        ages[i] = maxAge;
        sizes[i] = ofRandom(10, 52);
        rotations[i] = ofRandom(0, 360);
        rotationSpeeds[i] = ofRandom(-10, 10);
		delays[i] = 40;
    }
    
    int i = 0;
    colors[i++] = 0xFFFFFF;
    colors[i++] = 0xFFFFFF;
    colors[i++] = 0xFFFFFF;
    colors[i++] = 0xFFFFFF;
    colors[i++] = 0xFFFFFF;
    colors[i++] = 0xFFFFFF;
}

void Sparticles::setup(string imagePath) {
    ofxNestedFileLoader loader;
    vector<string> names = loader.load(imagePath);
    loader.printPaths();
    vector<ofImage> categoryImages;
    string lastCategory = "";
    string currentCategory = "";
    for(int i = 0; i < names.size(); i++) {
        vector<string> split = ofSplitString(names[i], "/");
        ofImage* img = new ofImage();
        img->load(names[i]);
        imageLibrary.push_back(img);
    }
}


Sparticles::~Sparticles(){
    delete [] positions;
    delete [] velocities;
    delete [] ages;
    delete [] sizes;
}


void Sparticles::update(){
    for(int i = 0; i < numParticles; i++) {
		if (ages[i] < maxAge) {
			ages[i]++;
			if (ages[i] > delays[i] && positions[i].y < ofGetHeight()) {
				velocities[i] += accelerations[i];
				positions[i] += velocities[i];
				rotations[i] += rotationSpeeds[i];
			}
		}
    }
}

void Sparticles::killAllSparticles() {
	for (int i = 0; i < numParticles; i++) {
		ages[i] = maxAge;
	}
}

void Sparticles::draw(float xFactor, float yFactor){
    ofPushStyle();
    ofColor c;
    ofEnableAlphaBlending();
    for(int i = 0; i < numParticles; i++) {
        if(ages[i]<maxAge && ages[i]>delays[i]) {
            float size = ofMap(ages[i], 0, maxAge, sizes[i], 0);
            c.setHex(colors[i%6]);
            ofSetColor(c.r, c.g, c.b, ofMap(ages[i], 0.5, maxAge, 255, 0) );
            ofPushMatrix();
            ofTranslate(positions[i].x, positions[i].y);
            ofScale(xFactor, yFactor);
			ofTranslate(size / 2, size / 2);
			ofRotate(rotations[i], 0, 0, 1);
			ofTranslate(-size / 2, -size / 2);
            images[i]->draw(0, 0, size, size);
            ofPopMatrix();
        }
    }
    ofPopStyle();
}

// This sets the position, acceleration, velicty, image and delay of the "next" particle and increments the next particle index.
void Sparticles::spawn(float x, float y, float dx, float dy, float ddx, float ddy, float _delay){
    ages[pos] = 0;
    positions[pos] = ofVec2f(x, y);
    velocities[pos] = ofVec2f(dx, dy);
	accelerations[pos] = ofVec2f(ddx, ddy);
	images[pos] = imageLibrary[ofRandom(imageLibrary.size())];
	delays[pos] = _delay;

    pos++;
    pos %= numParticles;
}