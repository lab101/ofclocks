#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //ofSetFullscreen(true);
    
    ofSetWindowShape(1400, 1400);
  //  ofSetWindowPosition(1200, 0);
    
    // the actual time you will be counting to.
    
    // 1 min fast countdown
//    countDownInSeconds = 40;
//    float nrOfDots = 40 * 3;
//    int radius = 12;
//    int spacing = 80;
//    int maxWidth = 1000;

   //  15 min slow countdown
    countDownInSeconds = 15 * 60;
    float nrOfDots = countDownInSeconds * 2 ;
    int radius = 6;
    int spacing = 32;
    int maxWidth = 1300;

    
    
    ofColor backgroundColor;
    backgroundColor.setHsb(300, 255, 255);
    ofSetBackgroundColor(10);
    startTime = -1;
   // ofSetFullscreen(true);
    ofSetCircleResolution(14);


    
    popInterval = countDownInSeconds / nrOfDots;
    nextPopTime = popInterval;
    
    

    
    
    size.x = maxWidth;
    
    int x = 0;
    int y = 0;
    
    // short notation to fill up the vector with particles
    particles.assign(6000,Particle());
    
    
    for(int i = 0; i < nrOfDots;++i){
        
        Dot d;
        d.setup(ofVec2f(x,y),radius);
        dots.push_back(d);
        
        x += spacing;
        if(x + radius >= maxWidth){

            y += spacing;
            x= 0;
            
        }
    }
    
    size.y = y;
    

    soundplayer1.load("switch33.wav");
    soundplayer2.load("click1.wav");
    
    soundplayer1.setMultiPlay(true);
    soundplayer2.setMultiPlay(true);

}


ofVec2f ofApp::popADot(){
    // look for a free dot starting from a random index.
    int index = ofRandom(dots.size());
    for(int i = 0; i < dots.size(); i++ ){
        if(!dots[index].mIsDead){
            dots[index].pop();
            return dots[index].mDrawPosition;
        }
        ++index;
        if(index > dots.size()) index = 0;
    }
    
    return ofVec2f(-100,-100);
}


//--------------------------------------------------------------
void ofApp::update(){
    
    
    if(startTime >- 1){
        
        nextPopTime -= ofGetLastFrameTime();
        if(nextPopTime < 0){
            
            nextPopTime += popInterval;
            ofVec2f popPosition = popADot();
            
            // -100 means no free dots available.
            if(popPosition.x == -100){
                startTime = -1;
            }else{
                soundplayer2.play();
                launchParticles(popPosition);
                applyExplosionForce(popPosition);
                float pan = ofMap(popPosition.x, 0, size.x , -1 , 1);
                soundplayer2.setPan(pan);

            }
            
            
        }
    }

    
    for(Dot& d: dots){
        if(!d.mIsDead){
            float normx = ofNormalize(d.mPosition.x, 0, windowSize.x);
            float normy = ofNormalize(d.mPosition.y, 0, windowSize.y);

            float xNoise = ofNoise(normx * 1.5, ofGetElapsedTimef() * 0.15 );
            float yNoise = ofNoise(normy + 10 , ofGetElapsedTimef() * 0.1 );
            ofVec2f noise(xNoise * 42,yNoise * 42);
            d.update(noise);
        }
    }
    
    for(Particle& p  : particles ){
        if(!p.mIsDead){
            p.update();
            
            if(p.mPosition.x < - p.mRadius || p.mPosition.x > windowSize.x + p.mRadius || p.mPosition.y < -p.mRadius || p.mPosition.y > windowSize.y + p.mRadius){
                p.mIsDead = true;
           }

        }
    }


}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(255);
    
    ofPushMatrix();
    ofTranslate(windowCenter.x - (size.x * 0.5), windowCenter.y - (size.y * 0.5));

        for(Dot& d: dots){
            d.draw();
        }
    
        for(Particle& p  : particles ){
            p.draw();
        }
    
    ofPopMatrix();

}



void ofApp::launchParticles(ofVec2f& position){
    
    int foundFreeParticles =0;
    for(Particle& p  : particles ){
        if(p.mIsDead){
            if(++foundFreeParticles > 280) return;
            p.mIsDead = false;
            p.mPosition = position;
            p.mSpeed = ofRandom(1, 22);
            p.mDirection.set(ofRandom(-1, 1), ofRandom(-1, 1));
            p.mDirection.normalize();
            p.mRadius = ofRandom(0.8,4);

        }
    }
}

void ofApp::applyExplosionForce(ofVec2f& position){
    
    for(Dot& d: dots){
        if(!d.mIsDead){
            ofVec2f div = d.mDrawPosition - position;
            float length = div.length();
            
            float force = ofMap(length, 0, 400, 100, 0,true);
            force *= 0.8;
            ofVec2f exp = div.normalize() * force;
            
            d.incomingForce(exp);
        }
    }

}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f'){
        ofToggleFullscreen();
    }else if (key == ' '){
        ofVec2f  popPosition = popADot();
        
        soundplayer2.play();
        launchParticles(popPosition);
        applyExplosionForce(popPosition);
        float pan = ofMap(popPosition.x, 0, size.x , -1 , 1);
        soundplayer2.setPan(pan);

        return;
        
        startTime = ofGetElapsedTimef();
        nextPopTime = popInterval;

        for(Dot& d : dots){
            d.reset();
        }
    }
}


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    windowSize = ofGetWindowSize();
    windowCenter = windowSize * 0.5f;
}


