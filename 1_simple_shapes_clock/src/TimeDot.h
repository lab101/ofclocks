//
//  TimeDot.hpp
//  clock1
//
//  Created by Kris Meeusen on 13/02/2018.
//

#pragma once

#include <stdio.h>
#include "ofMain.h"

class TimeDot{
    
    float mRadius;
    float mDistance;
    
    
    ofColor mColor;
    ofColor mDarkerColor;
    ofColor mRedColor;
public:
    float mRotations;

    void reset();
    void goRed();
    void setup(float rotations, float distance);
    void draw(ofVec2f center, float amount);
};

