#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // This gets called once when we start our app

    sound.load("beats.mp3");
//    sound.load("ninja.mp3");
    sound.setLoop( true );
    
    laughSound.load("laugh.wav");
    cheerSound.load("cheer.mp3");
    cheerSound.setMultiPlay(true);
    bumpSound.load("bump.mp3");
    
    
    myfont.load("arial.ttf", 14, true, false, true, 0.1);
    ofSetFullscreen(false);
    
    // Turn on depth testing so rendering happens according to z-depth rather
    // than draw order.
    ofEnableDepthTest();
    
    // Black background
    ofBackground(0);

    windowWidth = ofGetWindowWidth();
    windowHeight = ofGetWindowHeight();
    starRangeWidth = windowWidth/8;
    starRangeHeight = windowHeight/8;
    
    for(int i = 0; i < starsCount; i++) {
        Star star;
        star.timestamp = 0;
        star.generateLocation(distribution, generator, starRangeWidth, starRangeHeight, initialStarZ * distribution(generator));
        star.generateColor(distribution, generator);
        star.obj.set(boxSize, boxSize, boxSize);
        stars.push_back(star);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    // This gets called once for each graphical frame, right before draw()
    sound.setPan(offsetX / starRangeWidth); // pan the sound according to position
    currentTime = ofGetElapsedTimeMillis(); // read time
    // Initial waiting screen;
    if (isInitialState) {
        timePassed = 0;
        score = scoreMax;
        isStart = false;
        isPause = false;
        isContinue = false;
        isHit = false;
        isGuarded = false;
        ofBackground(200, 200, 200);
        starSpeed = 8.0;
        lastSpeedUpdateTime = 0;
    }
    
    if (!isStart) {
        ofBackground(200, 200, 200);    // pause
        sound.setPaused(true);
    } else {
        sound.setPaused(false);
//        sound.play();
        ofBackground(0, 0, 0);          // play
        timePassed += currentTime - lastTime;   // Update timer if the game is running
        if (timePassed/1000 == (lastSpeedUpdateTime + 10)) {
            starSpeed += 1;
            lastSpeedUpdateTime += 10;
        }
//        cout << timePassed << endl;
    }
    if (score <= 0) {
        finalTimeLeft = gameTime - timePassed/1000;
        isInitialState = true;
        isLose = true;
        laughSound.play();
    }
    if (timePassed > (gameTime + 1) * 1000) {
        isInitialState = true;
        isWin = true;
        cheerSound.play();
    }
    // Update the background color when a start is hit
    if (isStart && isHit) {
        ofBackground(255 - 10*hitCount, 255 - 10*hitCount, 255 - 10*hitCount);
        hitCount++;
        if (hitCount == hitCountMax) {
            hitCount = 0;
            isHit = false;
        }
        
    }
    // Avoid hitting multiple stars at the same time.
    if (isGuarded) {
        guardCount--;
        if (guardCount == 0) {
            guardCount = guardCountMax;
            isGuarded = false;
        }
    }
    // Check individual stars
    for(Star& star: stars) {
        star.location.z -= starSpeed;
        // check if hit the stars
        float distance2 = star.location.z * star.location.z
        + (star.location.x+offsetX) * (star.location.x+offsetX)
        + (star.location.y+offsetY) * (star.location.y+offsetY);
        if (isStart && (distance2 < 200) ) {
            isHit = true;
            isFOVChange = true;
            bumpSound.play();
            if (!isGuarded){
                score -= 5;
                finalScoreLeft = score;
            }
            isGuarded = true;
        }
        // Check if the start is behind
        if (star.location.z < 0) {
            star.generateLocation(distribution, generator, starRangeWidth, starRangeHeight, initialStarZ);
            star.timestamp = ofGetFrameNum();
        }
    }
    
    // check key press
    if (isStart) {
        if (directionLeft){
            if (offsetX < starRangeWidth * 0.95){
                offsetX += moveSpeed;
            }
        }
        if (directionRight){
            if (offsetX > -starRangeWidth * 0.95){
                offsetX -= moveSpeed;
            }
        }
        if (directionUp){
            if (offsetY > -starRangeHeight * 0.75){
                offsetY -= moveSpeed;
            }
        }
        if (directionDown){
            if (offsetY < starRangeHeight * 0.75){
                offsetY += moveSpeed;
            }
        }
    }
      // Update Timer
    lastTime = currentTime;
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255, 255, 255);
//    ofDrawAxis(10000);
    // This gets called once for each graphical frame, right after update()
    // This is where you draw all the graphics
    cam.begin();
    if (isInitialState) {
        cam.setFov(100);
    }
    cam.setNearClip(1);
    cam.setFarClip(20000);
    // Draw stars
    fov = cam.getFov();
    if (isFOVChange) {
//        if (fovCount <= hitCountMax/2) {
//            fov -= 3;
//            cam.setFov(fov);
//        } else if (fovCount <= hitCountMax) {
//            fov += 3;
//            cam.setFov(fov);
//        }
//        if (fovCount == 0) {
//            fov = 25;
//            cam.setFov(fov);
//            
//        } else if(fovCount <= hitCountMax){
//            fov += 3;
//            cam.setFov(fov);
//        }
//        if (fovCount == hitCountMax) {
//            fovCount = 0;
//            if (fov != 100) {
//                fov = 100;
//                cam.setFov(fov);
//            }
//        } else {
//            fovCount++;
//        }
        fov = 50;
        cam.setFov(fov);
        isFOVChange = false;
        
        
    } else if (fov != 100) {
//        fovCount = 0;
        fov += 1;
        cam.setFov(fov);
    }
    cout << fov  << endl;
    ofPushMatrix(); // Save our state for later
    
    {
        float centerX = offsetX;
        float centerY = offsetY;
        float currentFrame = ofGetFrameNum();
        // Since our coordinates start in the top left, translate to the center.
        ofTranslate( centerX, centerY, 0);
        // tilt the trajectory
        k1 = ofSignedNoise(1,currentFrame/100);
        k2 = ofSignedNoise(currentFrame/100,1);
        ofPolyline line1;
        ofPolyline line2;
        ofPolyline line3;
        ofPolyline line4;
        for (int z = 0; z < 100; z++) {
            int trueZ = z * 10;
            int x1 = -140 + k1 * trueZ * trueZ/scaleDis;
            int x2 = 140 + k1 * trueZ * trueZ/scaleDis;
            int y1 = -140 + k2 * trueZ * trueZ/scaleDis;
            int y2 = 140 + k2 * trueZ * trueZ/scaleDis;
            line1.addVertex(x1, y1, -trueZ);
            line2.addVertex(x2, y1, -trueZ);
            line3.addVertex(x1, y2, -trueZ);
            line4.addVertex(x2, y2, -trueZ);
            if (z % 20 == 20 - ((int)floor(currentFrame)* (int)starSpeed/10) % 20) {
                ofDrawLine(x1, y1, -trueZ, x2, y1, -trueZ);
                ofDrawLine(x2, y1, -trueZ, x2, y2, -trueZ);
                ofDrawLine(x2, y2, -trueZ, x1, y2, -trueZ);
                ofDrawLine(x1, y2, -trueZ, x1, y1, -trueZ);
            }
        }
        line1.draw();
        line2.draw();
        line3.draw();
        line4.draw();
        for (Star& star : stars) {
            ofPushMatrix();
            {
                float zScaled = (star.location.z * star.location.z)/ scaleDis;
                ofSetColor(star.color.x, star.color.y, star.color.z);
                ofTranslate(star.location.x + k1 * zScaled, star.location.y+ k2 * zScaled, - star.location.z);
                star.obj.draw();
            }
            ofPopMatrix();
        }
    }
    ofPopMatrix();
    
    // Print score
    ofPushMatrix();
    {
//        float currentFrame = ofGetFrameNum();
        ofTranslate( -windowWidth / 4, windowHeight / 4-10, -180);
        ofSetColor(255,255,255);
        myfont.drawStringAsShapes("Your Score:", 0, 0);
        myfont.drawStringAsShapes(std::to_string(score), 100, 0);
    }
    ofPopMatrix();
    // Print timer
    ofPushMatrix();
    {
        int timeLeft = gameTime - timePassed/1000;
//        float currentFrame = ofGetFrameNum();
        ofTranslate( windowWidth / 4-100, windowHeight / 4-10, -180);
        ofSetColor(255,255,255);
        myfont.drawStringAsShapes("Time left:", 0, 0);
        myfont.drawStringAsShapes(std::to_string(timeLeft), 90, 0);
        
    }
    ofPopMatrix();
    // Print instructions
    ofPushMatrix();
    {
//        float currentFrame = ofGetFrameNum();
        
        ofSetColor(255,255,0);
        if (!isStart && !isWin && !isLose) {
            if (!isPause) {
                ofTranslate(-140, 0, -180);
                myfont.drawStringAsShapes("Press [s] to start, Press [p] to pause", 0, 0);
                myfont.drawStringAsShapes("[1]: Hard", -windowWidth / 5 + 80, -windowWidth / 5 + 40);
                myfont.drawStringAsShapes("[2]: Harder", -windowWidth / 5 + 80, -windowWidth / 5 + 10);
            } else {
                ofTranslate(-70, 0, -180);
                myfont.drawStringAsShapes("Press [s] to resume", 0, 0);
            }
        } else if (isWin) {
            ofTranslate(-70, 0, -180);
            myfont.drawStringAsShapes("YOU ARE AWESOME!", 0, 20);
            myfont.drawStringAsShapes("Your score: ", 20, -20);
            myfont.drawStringAsShapes(std::to_string(finalScoreLeft), 140, -20);
        } else if (isLose) {
            ofTranslate(-80, 0, -180);
            myfont.drawStringAsShapes("YOU CAN DO BETTER!", 0, 20);
            myfont.drawStringAsShapes("Time left: ", 20, -20);
            myfont.drawStringAsShapes(std::to_string(finalTimeLeft), 140, -20);
        }
    }
    ofPopMatrix();
    cam.end();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == '1'){
        if (isInitialState){
            sound.unload();
            sound.load("beats.mp3");
            gameTime = 60;
            starSpeed = 7.0;
        }
        
    }
    if (key == '2'){
        if (isInitialState){
            sound.unload();
            sound.load("ninja.mp3");
            gameTime = 155;
            starSpeed = 5.0;
        }
        
    }
    if (key == 's') {
        if (isPause) {
            isPause = false;
            isContinue = true;
        } else {
            // the first time to start
            isContinue = false;
            ofGetElapsedTimeMillis();
            isWin = false;
            isLose = false;
            if (!isStart){
                sound.play();
            }
        }
        isStart = true;
        isInitialState = false;
    }
    if (key == 'p') {
        isStart = false;
        isPause = true;
    }
    if (key == 'r') {
        isInitialState = true;
        isWin = false;
        isLose = false;
    }
    if (key == OF_KEY_LEFT){
        directionLeft = true;
    }
    if (key == OF_KEY_RIGHT){
        directionRight = true;
    }
    if (key == OF_KEY_UP){
        directionUp = true;
    }
    if (key == OF_KEY_DOWN){
        directionDown = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == OF_KEY_LEFT){
        directionLeft = false;
    }
    if (key == OF_KEY_RIGHT){
        directionRight = false;
    }
    if (key == OF_KEY_UP){
        directionUp = false;
    }
    if (key == OF_KEY_DOWN){
        directionDown = false;
    }
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
