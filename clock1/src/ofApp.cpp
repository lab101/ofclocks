#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    // the actual time you will be counting to.
    countDownInSeconds = 0.5 * 60;
    
    
    ofColor backgroundColor;
    backgroundColor.setHsb(140, 140, 125);
    ofSetBackgroundColor(backgroundColor);
    
    startTime = -1;
    ofSetCircleResolution(40);
    
    float maxRotation = M_PI;

    for(int i=0; i < 1000; ++i){

        TimeDot dot;
        float rotations = ofRandom(10, maxRotation);
        dot.setup(rotations ,ofRandom(10.0f, ofGetWindowWidth() * 0.5));
        dots.push_back(dot);

    }
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (startTime > 0)
    {
        float div = countDownInSeconds - (ofGetElapsedTimef() - startTime);
        
        
        float pctToGo = ofMap(div,0,countDownInSeconds,0,1.0,true);
        
        ofVec2f center = ofGetWindowSize() * 0.5f;
        center.y += sin(ofGetElapsedTimef() * 1.2) * 14;
        center.x += cos(ofGetElapsedTimef() * 0.4) * 10;

        for(TimeDot& d : dots){
            if(div <= 0){
                d.goRed();
            }
            d.draw(center, pctToGo);
        }
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'f'){
        ofToggleFullscreen();
    }else{
        startTime = ofGetElapsedTimef();
        ofSeedRandom();
        for(TimeDot& d : dots){
            d.reset();
        }
    }
}

