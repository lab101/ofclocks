#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    
    image.load("02_Its-Nice-That_Barbican-ArticleINT.jpg");
    image.load("Untitled-6.png");
    
    ofSetBackgroundColor(120);
    
    // credits https://jonny-doomsday.deviantart.com/art/Hands-Vector-Pack-187038799
    
    hand.load("hand.png");
    hand2.load("hand2.png");

    //ofSetWindowShape(1800, 1600);
    
    gui.setup(); // most of the time you don't need a name
    gui.setWidthElements(1000);
    gui.setDefaultWidth(1000);
    gui.setSize(1000, 1000);
    gui.setPosition(1000, 10);


    gui.add(paramRMin.setup("paramRMin", 140.0, 0.0, 300.0));
    gui.add(paramRMax.setup("paramRMax", 140.0, 0.0, 300.0));
    gui.add(param3.setup("p3", 4.0, 0.0, 300.0));
    gui.add(param4.setup("p4", 4.0, 0.0, 300.0));
   
    gui.add(pctmin.setup("pctmin", 0, 0, 1));
    gui.add(pctax.setup("pctmax", 0, 0, 1));

    
    gui.loadFromFile("settings.xml");
    
    ofSetFullscreen(true);
    // 4 // 10
    
    blockSize.x = image.getWidth() / 8;
    blockSize.y = image.getHeight() / 15;

//    blockSize.x = image.getWidth() / 4;
//    blockSize.y = image.getHeight() / 10;

    
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
    
    countDownInSeconds = 60 * 5;
    popInterval = countDownInSeconds / blocks.size();
    nextPopTime = popInterval;

    
    startTime = ofGetElapsedTimef();

    
}


void ofApp::addRectangle(Block& b){
    
//
//    float n1= ofNoise(ofGetElapsedTimef() * 0.1, b.targetPosition.x) * 20;
//    float n2= ofNoise(ofGetElapsedTimef() * 0.1, b.targetPosition.x) * 4;
//    ofVec2f n(n1,n2);
//
    ofVec2f d2= b.drawPosition ;

    
    ofVec3f topRight = ofVec3f(blockSize.x, 0 ,0);
    ofVec3f bottomRight = ofVec3f(blockSize.x,blockSize.y,0);
    ofVec3f bottomLeft = ofVec3f(0,blockSize.y,0);


    topRight.rotate(b.rotation ,ofVec3f(0,0,1));
    bottomRight.rotate(b.rotation,ofVec3f(0,0,1));
    bottomLeft.rotate(b.rotation,ofVec3f(0,0,1));
    
    topRight += d2;
    bottomRight += d2;
    bottomLeft += d2;

    // left top
    mesh.addVertex(ofVec3f(d2));
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
    mesh.addVertex(ofVec3f(d2));
    mesh.addTexCoord(b.textureRectangle.getTopLeft());

}




ofVec2f ofApp::startABlock(bool random){
    // look for a free dot starting from a random index.
    
    Block* mostLeft = nullptr;
    
    int index = ofRandom(blocks.size());
    for(int i = 0; i < blocks.size(); i++ ){
        if(blocks[index].currentState == Block::WAITING){
//            blocks[index].startMoving();
            
            if(random){
                blocks[index].startMoving();
                return;
            }
            
            if(mostLeft == nullptr || (blocks[index].targetPosition.x < mostLeft->targetPosition.x)){
                mostLeft = &(blocks[index]);
            }
            //return blocks[index].startPosition;
        }
        ++index;
        if(index > blocks.size()) index = 0;
    }
    
    if(mostLeft != nullptr){
        mostLeft->startMoving();
        return mostLeft->startPosition;
    }
    
    return ofVec2f(-100,-100);
}

//--------------------------------------------------------------
void ofApp::update(){

    if(startTime == -1) return;
    
        nextPopTime -= ofGetLastFrameTime();
        if(nextPopTime < 0){
            
            nextPopTime += popInterval;
            startABlock(true);
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
            addRectangle(b);
        }

    }
    
   

}


void ofApp::drawFoldLine(ofVec2f p1,ofVec2f p2, float pct,float steps, Block& block){
   
//    ofVec2f p1(100,100);
//    ofVec2f p2(400,500);
//    float d = sin(ofGetElapsedTimef()) * 200;
//    p2 += ofVec2f(d,d);
    
   // ofSetColor(255, 0, 0 );
   // ofDrawLine(p1, p2);

    int direction = block.direction;
    ofVec2f prevP;
    
    float distance = p2.distance(p1);
    float rMax = ofMap(pct, pctmin, pctax, paramRMin, paramRMax,true);
    float ss = ofMap(pct, pctmin, pctax, param3, param4,true);
    float radius;
    
    for(float i=0; i < 1; i += steps){
        
        
        radius = sin(i * ss) * rMax;

        ofVec2f p = p2.getInterpolated(p1, i);
//
     //   ofDrawCircle(p, 3);
        ofVec2f pp =  (p2-p1).getPerpendicular();
        ofVec2f pp2 = p+ (pp * radius * direction);
        
    //    ofSetColor(255, 0, 0 );
      //  ofNoFill();
      //  ofDrawCircle(pp2, 6);
//
       // direction = - direction;
        
               // ofSetColor(0, 0, 255 );

        if(i!=0){
            
//            ofVec2f inter;
//            for(float j=0; j < 1; j+=0.1){
//                ofVec2f inter2 = pp2.getInterpolated(prevP, i);
//                if(j!=0){
//                    ofDrawLine(inter, inter2);
//                }
//                inter  = inter2;
//            }
            
            ofDrawLine(prevP, pp2);
        }
        prevP = pp2;
        
    }
    
    ofDrawLine(prevP, p1);
}


//--------------------------------------------------------------
void ofApp::draw(){

    

   // drawFoldLine(ofVec2f(210,800), ofVec2f(1010,80),0.01);
    //return;



    ofEnableAlphaBlending();

    ofPushMatrix();
    ofTranslate(10, 10);
    
    ofSetColor(0,0,0,40);

    
    image.bind();

    mesh.draw();

    

    ofSetColor(255);
    mesh.draw();
    image.unbind();
    
    
   
    ofSetColor(0,0,0,40);
    
    for(Block& b: blocks){
        
        // b.handPosition += blockSize;
        
        if(b.currentState != Block::WAITING && b.currentState != Block::DONE){
            ofSetLineWidth(b.size );
            
            // ofSetColor(0);
            
            float steps =  ofMap(fabsf(b.startPosition.distance(b.targetPosition)), 0, 8000, 0.003, 0.001,true);
            
            
            drawFoldLine(b.startPosition ,b.handPosition + (blockSize *0.5), b.movingPct, steps, b);
            
        }
    }

    ofPopMatrix();

    
    
    for(Block& b: blocks){

        if(b.currentState != Block::MOVEIN){
            ofSetColor(23);
            ofDrawCircle(b.drawPosition + ofVec2f(20,20), 4);
        }
        
        if(b.currentState != Block::WAITING && b.currentState != Block::DONE){
            ofSetLineWidth(b.size);

            ofSetColor(25);

            float steps =  ofMap(fabsf(b.startPosition.distance(b.targetPosition)), 0, 8000, 0.003, 0.001,true);
           
            
            drawFoldLine(b.startPosition ,b.handPosition + (blockSize *0.5), b.movingPct, steps, b);
            
           // if(b.currentState != Block::MOVE_HAND_BACK){
                
                ofVec2f div =  (b.targetPosition - b.startPosition);
                div.normalize();
                ofVec2f p(1,0);
                float angle = p.angle(div);
                
                ofPushMatrix();
                ofTranslate(b.handPosition + blockSize *0.5);

                ofRotate(angle +90);

                ofTranslate(hand.getWidth() * -0.5, hand.getHeight() * -1);
            ofSetColor(255);

             if(b.currentState == Block::MOVE_HAND_BACK || b.movingPct > 0.9997){

                hand2.draw(0,0);
             }else{
                hand.draw(0,0);
                 
             }
                ofPopMatrix();
            
          
        }
        
      
        
    }
    
    ofSetColor(255);


    if(debug){
    float div =  (ofGetElapsedTimef() - startTime) / countDownInSeconds;
    ofDrawBitmapString((ofGetElapsedTimef() - startTime), 20, 40);
    ofDrawBitmapString(div, 20, 20);
   
    
    gui.draw();
        
        ofSetColor(10);
        ofSetLineWidth(1);
        mesh.drawWireframe();

    }
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
    }else if(key == 'd'){
        debug = !debug;
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
