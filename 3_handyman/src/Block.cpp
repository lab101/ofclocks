//
//  Block.cpp
//  3_handyman
//
//  Created by Kris Meeusen on 28/02/2018.
//

#include "Block.hpp"


Block::Block(ofVec2f newPosition, ofRectangle newTextureRectangle){
    
    targetPosition = newPosition;
    startPosition = newPosition;
    textureRectangle = newTextureRectangle;

    reset();
    
}

void Block::reset(){
    
    currentState = WAITING;
    float angle = ofRandom(360);
    
    startRotation = ofRandom(-120,120);
    rotation = startRotation;

    float radius = fmax(ofGetWindowWidth(), ofGetWindowHeight())  + 500;
    
    startPosition.x =  sin(angle) * radius;
    startPosition.y =  cos(angle) * radius;
    
    drawPosition = startPosition;
    handPosition = startPosition;

}

float Block::easeOut(float t,float b , float c, float d) {
    return c*((t=t/d-1)*t*t + 1) + b;

  //  return c*((t=t/d-1)*t*t + 1) + b;
}

float Block::elasticOut(float t,float b , float c, float d) {
    if (t==0) return b;  if ((t/=d)==1) return b+c;
    float p=d*.3f;
    float a=c;
    float s=p/4;
    return (a*pow(2,-10*t) * sin( (t*d-s)*(2*PI)/p ) + c + b);
}


void Block::startMoving(){
    startAnimationTime = ofGetElapsedTimef();
    startRotationAnimationTime = ofGetElapsedTimef();
    
    currentState = MOVEIN;
}


void Block::lerpRotation(){
    float div = ofGetElapsedTimef() - startRotationAnimationTime;
    
    float pct = ofMap(div, 0, 5, 0, 0.8 ,true);
    pct = elasticOut(pct, 0, 1, 1);

    rotation = ofLerp(startRotation, 0, pct);
    
   
}

void Block::update(){
    
    // MOVEIN state
    if(currentState == MOVEIN){
        float div = ofGetElapsedTimef() - startAnimationTime;
        float pct = ofMap(div, 0, 3, 0, 1, true);
        
        if(pct >= 1){
            currentState = IN_PLACE;
            startAnimationTime = ofGetElapsedTimef();
        }

        pct = easeOut(pct, 0, 1, 1);
        drawPosition = startPosition.getInterpolated(targetPosition, pct);
        handPosition = drawPosition;
        
        lerpRotation();
        
    }
    
    // IN PLACE
    else if(currentState == IN_PLACE){
        float div = ofGetElapsedTimef() - startAnimationTime;
        
        lerpRotation();
        
        if(div > 1){
            currentState = MOVE_HAND_BACK;
            startAnimationTime = ofGetElapsedTimef();
        }
    }
    
    // MOVE_HAND_BACK
    else if(currentState == MOVE_HAND_BACK){
        float div = ofGetElapsedTimef() - startAnimationTime;
        float pct = ofMap(div, 0, 2, 0, 1 ,true);
        pct = easeOut(pct, 0, 1, 1);

        handPosition = drawPosition.getInterpolated(startPosition, pct);

        lerpRotation();

    }

    
}

