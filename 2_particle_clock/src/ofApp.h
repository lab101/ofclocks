#pragma once

#include "ofMain.h"
#include "Dot.hpp"
#include "Particle.hpp"

class ofApp : public ofBaseApp{

    
    int countDownInSeconds;
    float startTime;
    float popInterval;
    float nextPopTime;
    ofVec2f size;
    
    // going fullscreen causes a bug on ofGetWindowWidth & height there saving it.
    ofVec2f windowSize;
    
    vector<Dot> dots;
    vector<Particle> particles;

    
    ofVec2f popADot();
    void launchParticles(ofVec2f& position);
    void applyExplosionForce(ofVec2f& position);

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
        void windowResized(int w, int h);

		
};
