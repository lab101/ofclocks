#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	cout << "test" << endl;

    image.load("stop.png");
    
    ofSetBackgroundColor(120);
    
    // image credits https://jonny-doomsday.deviantart.com/art/Hands-Vector-Pack-187038799
    
    hand.load("hand.png");
    hand2.load("hand2.png");

    
    gui.setup(); 
    gui.setWidthElements(1000);
    gui.setDefaultWidth(1000);
    gui.setSize(1000, 1000);
    gui.setPosition(1000, 10);


	// params for tweaking the arm motion.
    gui.add(paramRMin.setup("paramRMin", 140.0, 0.0, 300.0));
    gui.add(paramRMax.setup("paramRMax", 140.0, 0.0, 300.0));
    gui.add(param3.setup("p3", 4.0, 0.0, 300.0));
    gui.add(param4.setup("p4", 4.0, 0.0, 300.0));
    gui.add(pctmin.setup("pctmin", 0, 0, 1));
    gui.add(pctax.setup("pctmax", 0, 0, 1));

    gui.loadFromFile("settings.xml");
    


	// CONFIG
   // ofSetFullscreen(true);
	ofSetWindowShape(1800, 1600);

	// small blocks
	//blockSize.x = image.getWidth() / 8;
    //blockSize.y = image.getHeight() / 15;

	// bigger blocks
    blockSize.x = image.getWidth() / 4;
    blockSize.y = image.getHeight() / 10;


	// the actual time to do the countdown.
	countDownInSeconds = 10 * 1;

    
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

	// timing for pop moments
    popInterval = countDownInSeconds / blocks.size();
    nextPopTime = popInterval;

    
    startTime = ofGetElapsedTimef();
   
}



// recreating the mesh block by block

void ofApp::addRectangle(Block& b){

	ofVec3f topLeft = ofVec3f(0, 0, 0);
    ofVec3f topRight = ofVec3f(blockSize.x, 0 ,0);
    ofVec3f bottomRight = ofVec3f(blockSize.x,blockSize.y,0);
    ofVec3f bottomLeft = ofVec3f(0,blockSize.y,0);

	topLeft.rotate(b.rotation, ofVec3f(10, 10, 0), ofVec3f(0, 0, 1));
    topRight.rotate(b.rotation, ofVec3f(10, 10, 0),ofVec3f(0,0,1));
    bottomRight.rotate(b.rotation, ofVec3f(10, 10, 0),ofVec3f(0,0,1));
    bottomLeft.rotate(b.rotation, ofVec3f(10, 10, 0),ofVec3f(0,0,1));
    
	topLeft += b.drawPosition;
    topRight += b.drawPosition;
    bottomRight += b.drawPosition;
    bottomLeft += b.drawPosition;

    // left top
    mesh.addVertex(topLeft);
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
    mesh.addVertex(topLeft);
    mesh.addTexCoord(b.textureRectangle.getTopLeft());

}



// pick random blocks or go from left to right.
void ofApp::startABlock(bool random){
    // look for a free block starting from a random index.
    
    Block* mostLeft = nullptr;
    
    int index = ofRandom(blocks.size());
    for(int i = 0; i < blocks.size(); i++ ){
        if(blocks[index].currentState == Block::WAITING){
           
            if(random){
                blocks[index].startMoving();
                return;
            }
            
            if(mostLeft == nullptr || (blocks[index].targetPosition.x < mostLeft->targetPosition.x)){
                mostLeft = &(blocks[index]);
            }
        }
        ++index;
        if(index >= blocks.size()) index = 0;
    }
    
    if(mostLeft != nullptr){
        mostLeft->startMoving();
    }
    
}


void ofApp::hangABlock() {
	int index = ofRandom(blocks.size());
	for (int i = 0; i < blocks.size(); i++) {
		if (blocks[index].currentState == Block::DONE) {
			blocks[index].hang();
			return;
		}
		++index;
		if (index >= blocks.size()) index = 0;
	}
}

//--------------------------------------------------------------
void ofApp::update(){

	// not running yet return.
    if(startTime == -1) return;

	float timeDiv = ofGetElapsedTimef() - startTime;

    nextPopTime -= ofGetLastFrameTime();
    
	if(nextPopTime < 0){
            nextPopTime += popInterval;

			if (timeDiv < (countDownInSeconds +20)) {
				startABlock(true);
			}
			else {
				if(ofRandom(8) < 3)	hangABlock();
			}
    }
    
     
    mesh.clear();
    
	// sort so we have the active blocks on top.
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
  

    int direction = block.direction;
    ofVec2f prevPoint;
    
    float distance = p2.distance(p1);
    float rMax = ofMap(pct, pctmin, pctax, paramRMin, paramRMax,true);
    float ss = ofMap(pct, pctmin, pctax, param3, param4,true);
    float radius;
    
    for(float i=0; i <= 1; i += steps){
        
        
        radius = sin(i * ss) * rMax;

        ofVec2f p = p2.getInterpolated(p1, i);
        ofVec2f pp =  (p2-p1).getPerpendicular();
        ofVec2f pp2 = p+ (pp * radius * direction);
        
  
        if(i!=0){
  
            ofDrawLine(prevPoint, pp2);
        }
		prevPoint = pp2;
        
    }
    
   // ofDrawLine(prevPoint, p1);
}


//--------------------------------------------------------------
void ofApp::draw(){

    


    ofEnableAlphaBlending();

	// draw shadows
	// performancewise it would be better to draw the whole scene in a fbo and use this for the shadwow.
    ofPushMatrix();
		ofTranslate(10, 10);
    	ofSetColor(0,0,0,40);
    
		image.bind();
		mesh.draw();
		image.unbind();
   
   
		for(Block& b: blocks){
			if(b.currentState != Block::WAITING && b.currentState != Block::DONE){
				ofSetLineWidth(b.size );
				float steps =  ofMap(fabsf(b.startPosition.distance(b.targetPosition)), 0, 8000, 0.003, 0.001,true);
				drawFoldLine(b.startPosition ,b.handPosition + (blockSize *0.5), b.movingPct, steps, b);
			}
		}

    ofPopMatrix();

	// draw the mesh
	image.bind();
	ofSetColor(255);
	mesh.draw();
	image.unbind();
    
    
    for(Block& b: blocks){

		// draw the screw.

		if (b.currentState == Block::DROP) {
			ofSetColor(23);
			ofDrawCircle(b.targetPosition + ofVec2f(10, 10), 4);

		}else if(b.currentState != Block::MOVEIN){
            ofSetColor(23);
            ofDrawCircle(b.drawPosition + ofVec2f(10,10), 4);
        }
        
        if(b.currentState != Block::WAITING && b.currentState != Block::DONE){
            ofSetLineWidth(b.size);

            ofSetColor(25);

            float steps =  ofMap(fabsf(b.startPosition.distance(b.targetPosition)), 0, 8000, 0.003, 0.001,true);
            drawFoldLine(b.startPosition ,b.handPosition + (blockSize *0.5), b.movingPct, steps, b);
            
			// drawing the hand with correct rotation.
                ofVec2f div =  (b.targetPosition - b.startPosition);
                div.normalize();
                //ofVec2f p(1,0);
                float angle = ofVec2f(1,0).angle(div);
                
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
        
      
		if (debug) {
			ofDrawBitmapStringHighlight(b.getStateString(), b.drawPosition);
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
	else if (key == 'h') {
		hangABlock();
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
