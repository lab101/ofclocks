//
//  Block.hpp
//  3_handyman
//
//  Created by Kris Meeusen on 28/02/2018.
//

#pragma once
#include "ofMain.h"



class Block{

    
public:

    enum blockState{ WAITING, MOVEIN, IN_PLACE, MOVE_HAND_BACK ,DONE };

    blockState currentState;
    
    float startAnimationTime;
    float startRotationAnimationTime;

    float easeOut(float t,float b , float c, float d);
    float elasticOut(float t,float b , float c, float d);


    Block(ofVec2f newPosition, ofRectangle newTextureRectangle);
    
    ofColor color;
    float size;
    float movingPct;
    int direction;
    
    void reset();
    void update();
    void lerpRotation();
    
    void startMoving();
    
    float rotation;
    float startRotation;

    ofVec2f targetPosition;
    ofVec2f startPosition;
    ofVec2f drawPosition;
    ofVec2f handPosition;

    ofRectangle textureRectangle;


};
