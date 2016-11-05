#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // This gets called once when we start our app

    // Make the app full screen
    ofSetFullscreen(false);
    
    // Turn on depth testing so rendering happens according to z-depth rather
    // than draw order.
    ofEnableDepthTest();
    
    // Black background
    ofBackground(0);
    
    // Set the size of our sphere "planets"
    
    // Initialize orbit rotation angles to zero

    windowWidth = ofGetWindowWidth();
    windowHeight = ofGetWindowHeight();
    starRangeWidth = windowWidth/4;
    starRangeHeight = windowHeight/4;
    
    for(int i = 0; i < starsCount; i++) {
        Star star;
        star.timestamp = 0;
        star.generateLocation(distribution, generator, starRangeWidth, starRangeHeight, initialStarZ * distribution(generator));
        star.obj.set(boxSize, boxSize, boxSize);
        stars.push_back(star);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    // This gets called once for each graphical frame, right before draw()
    
    for(Star& star: stars) {
        star.location.z -= starSpeed;
        if (star.location.z < 0) {
            star.generateLocation(distribution, generator, starRangeWidth, starRangeHeight, initialStarZ);
            star.timestamp = ofGetFrameNum();
        }
        
        cout<<star.location.x << ", "<<star.location.y << ", "<< star.location.z << std::endl;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255, 255, 255);
//    ofDrawAxis(10000);
    // This gets called once for each graphical frame, right after update()
    // This is where you draw all the graphics
    cam.begin();
    cam.setFov(100);
    cam.setNearClip(1);
    cam.setFarClip(20000);
    ofPushMatrix(); // Save our state for later
    {
        float centerX = offsetX;
        float centerY = offsetY;
        float currentFrame = ofGetFrameNum();
        // Since our coordinates start in the top left, translate to the center.
        ofTranslate( centerX, centerY, 0);
        
        ofSetColor(255, 255, 255);
        
        for (Star& star : stars) {
            ofPushMatrix();
            {
                ofTranslate(star.location.x, star.location.y, - star.location.z);
                star.obj.draw();
//                moon.drawWireframe();
            }
            ofPopMatrix();
        }
    }
    ofPopMatrix(); // Back to initial state at the top-left of our window.
    cam.end();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == OF_KEY_LEFT){
        offsetX -= moveSpeed;
    }
    if (key == OF_KEY_RIGHT){
        offsetX += moveSpeed;
    }
    if (key == OF_KEY_UP){
        offsetY += moveSpeed;
    }
    if (key == OF_KEY_DOWN){
        offsetY -= moveSpeed;
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
