//
//  TimeDot.cpp
//  clock1
//
//  Created by Kris Meeusen on 13/02/2018.
//

#include "TimeDot.h"


void TimeDot::setup(float rotations, float distance){
    
    mRotations = rotations;
    mDistance = distance;
    
    reset();
}

void TimeDot::reset(){
    
    mRadius =  ofRandom(10, 84);

    float saturation = 240 + ofRandom(-10,10);
    float brigthness = 200 + ofRandom(-10,10);
    float hue = ofRandom(130,140);

    mColor.setHsb(hue, saturation, brigthness);
    mDarkerColor.setHsb(hue, saturation, brigthness + 10);
    mRedColor.setHsb(ofRandom(240,255), saturation, brigthness);
    
}

void TimeDot::goRed(){
    mColor = mColor.lerp(mRedColor, 0.01f);
    mDarkerColor = mColor.lerp(mRedColor, 0.02f);
}



void TimeDot::draw(ofVec2f center, float amount){
    

    float adjustedDistance = mDistance * amount;
    float angle = (mRotations * amount);
    
    ofVec2f offset(sin(angle) *  adjustedDistance,cos(angle) * adjustedDistance );
    
    float d2 = mRadius * 0.08;
    float angleEye = ofGetElapsedTimef() + mRadius;
    ofVec2f offset2(sin(angleEye) * d2, cos(angleEye*0.1) * d2);
    
    ofVec2f offsetPosition  =  center + offset;
    ofVec2f eyeballPosition =  offsetPosition + offset2;
    

    // drawing the background
    ofSetColor(mColor);
    ofDrawCircle(offsetPosition,mRadius);
    
    
    // drawing inner eye border.
    ofSetColor(mDarkerColor);

    float m3 = fmaxf(4, mRadius * 0.33);
    ofDrawCircle(offsetPosition,m3);

    
    // drawing the white part
    ofSetColor(255);
    float m2 = fmaxf(4, mRadius * 0.2);
    ofDrawCircle(offsetPosition,m2);
    
    
    // drawing the black part
    ofSetColor(0);
    float eyeballRadius = fmaxf(4, mRadius * 0.1);
    ofDrawCircle(eyeballPosition,eyeballRadius);
}
