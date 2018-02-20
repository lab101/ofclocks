//
//  Dot.cpp
//  2_particle_clock
//
//  Created by Kris Meeusen on 19/02/2018.
//

#include "Dot.hpp"


void Dot::setup(ofVec2f position,float radius){
    mPosition = position;
    mRadius = radius;
    
    mIsDead = false;
}

void Dot::update(ofVec2f noise){
    if(!mIsDead){

        mDrawPosition = mPosition;
        mDrawPosition += noise;
        mDrawPosition += mIncomingForce;
    }
    
     mIncomingForce.interpolate(ofVec2f(0.0f,0.0f), 0.025f);
}

void Dot::incomingForce(ofVec2f force){
    mIncomingForce += force;
}

void Dot::reset(){
    mIsDead = false;
    mIncomingForce.set(0, 0);
}

void Dot::pop(){
    mIsDead = true;
}

void Dot::draw(){
    if(!mIsDead){
        float i = mIncomingForce.length();
        float r2 =  ofMap(i, 0, 10, 0, mRadius * 0.25,true);
        
        float c =  ofMap(i, 0, 10, 190, 255,true);
        ofSetColor(c);
        r2 = pow(r2, 1.4);
        
        ofDrawCircle(mDrawPosition, mRadius + r2);
    }
    
    //ofDrawBitmapString(mIncomingForce, mPosition.x + 20, mPosition.y + 20);
}
