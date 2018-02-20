#pragma once

#include "ofMain.h"
#include "TimeDot.h"

class ofApp : public ofBaseApp{

    std::vector<TimeDot> dots;
    int countDownInSeconds;
    float startTime;

    
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
	
		
};
