#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    
   /// image.load("02_Its-Nice-That_Barbican-ArticleINT.jpg");
    image.load("Untitled-1.png");

    //ofSetWindowShape(1800, 1600);

    ofSetFullscreen(true);
    
    blockSize.x = image.getWidth() / 10;
    blockSize.y = image.getHeight() / 10;
    startTime = -1;
    
    float h = 0;
    ofVec2f startDrawPosition(ofGetWindowWidth() * 0.5 - (image.getWidth() * 0.5), ofGetWindowHeight() * 0.5 - (image.getHeight() * 0.5));

    for(float h =0; h < image.getHeight(); h+= blockSize.y){

        for(float w =0; w < image.getWidth(); w+= blockSize.x){

            ofVec2f position = startDrawPosition + ofVec2f(w,h);
            Block newBlock(position,ofRectangle(ofVec2f(w,h), blockSize.x, blockSize.y));
            blocks.push_back(newBlock);
            
        }
    }
    
    countDownInSeconds = 80;
    popInterval = countDownInSeconds / blocks.size();
    nextPopTime = popInterval;


    
}


void ofApp::addRectangle(Block& b){
    
    
    ofVec3f topRight = ofVec3f(blockSize.x, 0 ,0);
    ofVec3f bottomRight = ofVec3f(blockSize.x,blockSize.y,0);
    ofVec3f bottomLeft = ofVec3f(0,blockSize.y,0);


    topRight.rotate(b.rotation ,ofVec3f(0,0,1));
    bottomRight.rotate(b.rotation,ofVec3f(0,0,1));
    bottomLeft.rotate(b.rotation,ofVec3f(0,0,1));
    
    topRight += b.drawPosition;
    bottomRight += b.drawPosition;
    bottomLeft += b.drawPosition;

    // left top
    mesh.addVertex(ofVec3f(b.drawPosition));
    mesh.addTexCoord(b.textureRectangle.getTopLeft());
    
    // right top
    mesh.addVertex(topRight);
    mesh.addTexCoord(b.textureRectangle.getTopRight());
    
    //right bottom
    mesh.addVertex(bottomRight);
    mesh.addTexCoord(b.textureRectangle.getBottomRight());

    // right bottom
    mesh.addVertex(bottomRight);
    mesh.addTexCoord(b.textureRectangle.getBottomRight());

    // left bottom
    mesh.addVertex(bottomLeft);
    mesh.addTexCoord(b.textureRectangle.getBottomLeft());

    //left top
    mesh.addVertex(ofVec3f(b.drawPosition));
    mesh.addTexCoord(b.textureRectangle.getTopLeft());

}




ofVec2f ofApp::startABlock(){
    // look for a free dot starting from a random index.
    int index = ofRandom(blocks.size());
    for(int i = 0; i < blocks.size(); i++ ){
        if(blocks[index].currentState == Block::WAITING){
            blocks[index].startMoving();
            return blocks[index].startPosition;
        }
        ++index;
        if(index > blocks.size()) index = 0;
    }
    
    return ofVec2f(-100,-100);
}

//--------------------------------------------------------------
void ofApp::update(){

    if(startTime == -1) return;
    
        nextPopTime -= ofGetLastFrameTime();
        if(nextPopTime < 0){
            
            nextPopTime += popInterval;
            startABlock();
        }
    
    float div =  (ofGetElapsedTimef() - startTime) ;
    
//    if(div >= countDownInSeconds){
//        startTime = -1;
//    }
    
    mesh.clear();
    
    float pct = div / countDownInSeconds;
    std::sort(blocks.begin(), blocks.end(), [](Block& a, Block& b) {
        return b.currentState < a.currentState;
    });
    
    for(Block& b: blocks){
        
    
        b.update();
        if(b.currentState != Block::WAITING){
            ofRectangle r(b.drawPosition ,blockSize.x,blockSize.y);
            addRectangle(b);
        }

    }
    
   

}

//--------------------------------------------------------------
void ofApp::draw(){
//    for(Block& b: blocks){
//        ofSetColor(0, 0, 0, 20);
//        ofRectangle r(b.drawPosition + ofVec2f(8,8) ,blockSize.x,blockSize.y);
//        ofDrawRectangle(r);
//
//
//    }

    ofEnableAlphaBlending();

    ofPushMatrix();
    ofTranslate(10, 10);
    
    ofSetColor(10,10,10,6);

    for(Block& b: blocks){
        ofDrawLine(b.startPosition ,b.handPosition );
    }
    
    ofSetColor(0,0,0,40);

    mesh.draw();
    ofPopMatrix();

    ofSetColor(255);
    image.bind();
    mesh.draw();
    image.unbind();
    
    ofSetLineWidth(6);
    
    for(Block& b: blocks){
        ofSetColor(0,0,0,40);
        ofDrawLine(b.startPosition + ofVec2f(10,10) ,b.handPosition+ ofVec2f(10,10));

        
        ofSetColor(255);
        ofDrawLine(b.startPosition ,b.handPosition);
        ofDrawCircle(b.handPosition,10);
        
    }
    
    ofSetColor(255);


    float div =  (ofGetElapsedTimef() - startTime) / countDownInSeconds;
    ofDrawBitmapString((ofGetElapsedTimef() - startTime), 20, 40);
    ofDrawBitmapString(div, 20, 20);
   // mesh.drawWireframe();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f'){
        ofToggleFullscreen();
    }else if (key == ' '){
        startTime = ofGetElapsedTimef();
        
        for(Block& d : blocks){
            d.reset();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
