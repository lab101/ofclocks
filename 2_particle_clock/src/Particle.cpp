//
//  Particle.cpp
//  2_particle_clock
//
//  Created by Kris Meeusen on 20/02/2018.
//

#include "Particle.hpp"


Particle::Particle(){
    mIsDead = true;
}


void Particle::update(){
    if(!mIsDead){

    mPosition += (mDirection * mSpeed);
    mSpeed *= 1.01;
    
    mDirection.interpolate(ofVec2f(0,1), 0.028);
    }
}


void Particle::draw(){
    if(!mIsDead){
        ofDrawCircle(mPosition, mRadius);
    }
}
