//
//  Dot.hpp
//  2_particle_clock
//
//  Created by Kris Meeusen on 19/02/2018.
//

#pragma once
#include "ofMain.h"

class Dot{
    
    float mRadius;
    ofVec2f mIncomingForce;
    
public:
    ofVec2f mDrawPosition;
    ofVec2f mPosition;
    bool mIsDead;
    
    void setup(ofVec2f position,float radius);
    void update(ofVec2f noise);
    void draw();
    void pop();
    void reset();
    void incomingForce(ofVec2f force);

};
