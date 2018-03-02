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
    
    
    color = ofColor::fromHsb(160,0, 255);
    size = 14;//ofRandom(10,10);

    currentState = WAITING;
    
    float angle = 0;
    float r = ofRandom(30);
    
    if(r < 2 ){
        angle = ofDegToRad(ofRandom(-170,-190));
    }else if(r < 4){
        angle = ofDegToRad(ofRandom(80,100));
        
    }else if(r < 20){
        angle = ofDegToRad(ofRandom(-80,-90));
    }else{
        angle = ofDegToRad(ofRandom(-10,10));

    }
    
    if(ofRandom(10) < 5 ){
        direction = -1;
    }else{
        direction = 1;
    }


    
    
    startRotation = ofRandom(-120,120);
    rotation = startRotation;

    float radius = fmax(ofGetWindowWidth(), ofGetWindowHeight());
    
    startPosition.x =  cos(angle) * radius;
    startPosition.y =  sin(angle) * radius;
    
    startPosition += (ofGetWindowSize() * 0.5);

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
        float pct = ofMap(div, 0, 1.5, 0, 1, true);
        
        if(pct >= 1){
            currentState = IN_PLACE;
            startAnimationTime = ofGetElapsedTimef();
        }

        movingPct = easeOut(pct, 0, 1, 1);
        drawPosition = startPosition.getInterpolated(targetPosition, pct);
        handPosition =  startPosition.getInterpolated(targetPosition, pct -0.02);
        
        lerpRotation();
        
    }
    
    // IN PLACE
    else if(currentState == IN_PLACE){
        float div = ofGetElapsedTimef() - startAnimationTime;
        
        lerpRotation();
        
        if(div > 0.8){
            currentState = MOVE_HAND_BACK;
            startAnimationTime = ofGetElapsedTimef();
        }
    }
    
    // MOVE_HAND_BACK
    else if(currentState == MOVE_HAND_BACK){
        float div = ofGetElapsedTimef() - startAnimationTime;
        float pct = ofMap(div, 0, 1, 0, 1 ,true);
        pct = easeOut(pct, 0, 1, 1);

        handPosition = drawPosition.getInterpolated(startPosition, pct);

        lerpRotation();
        
        if(div >= 2) {
            currentState = DONE;
        }

    }

    
}

