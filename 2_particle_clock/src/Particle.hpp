//
//  Particle.hpp
//  2_particle_clock
//
//  Created by Kris Meeusen on 20/02/2018.
//

#pragma once
#include "ofMain.h"

class Particle{
    
public:
    ofVec2f mPosition;
    ofVec2f mDirection;
    float mSpeed;
    float mRadius;
    
    bool mIsDead;
    
    Particle();
    
    
    void update();
    void draw();
    
};
