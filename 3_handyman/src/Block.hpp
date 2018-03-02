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

    enum blockState{ WAITING, MOVEIN, IN_PLACE, MOVE_HAND_BACK ,DONE , HANG , DROP };
	string getStateString();

    blockState currentState;
    
    float startAnimationTime;
    float startRotationAnimationTime;

	float easeIn(float t, float b, float c, float d);
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
	void hang();
    
    float rotation;
	float startRotation;
	float targetRotation;

    ofVec2f targetPosition;
    ofVec2f startPosition;
    ofVec2f drawPosition;
    ofVec2f handPosition;

    ofRectangle textureRectangle;


};
