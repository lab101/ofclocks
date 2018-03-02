#pragma once

#include "ofMain.h"

#include "Block.hpp"
#include "ofxGui.h"


class ofApp : public ofBaseApp{

    ofMesh mesh;
    ofImage image;
    
    vector<Block> blocks;
    ofVec2f blockSize;
    
    float countDownInSeconds;
    float startTime;
    float popInterval;
    float nextPopTime;

    
    ofxFloatSlider paramRMin;
    ofxFloatSlider paramRMax;
    ofxFloatSlider param3;
    ofxFloatSlider param4;
    ofxFloatSlider paramDistance;

    ofxFloatSlider pctmin;
    ofxFloatSlider pctax;
    
    ofImage hand;
    ofImage hand2;

    bool debug = false;
    
    ofxPanel gui;

    void startABlock(bool random);
	void hangABlock();

    void addRectangle(Block& b);
    
	public:
		void setup();
		void update();
		void draw();
        void drawFoldLine(ofVec2f p1,ofVec2f p2,float pct,float steps, Block& block);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
