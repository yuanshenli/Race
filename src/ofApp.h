#pragma once

#include "ofMain.h"
#include <random>

class ofApp : public ofBaseApp{
    float offsetX = 0;
    float offsetY = 0;
    float windowWidth = 0;
    float windowHeight = 0;
    float starRangeWidth = 0;
    float starRangeHeight = 0;
    struct Star {
        float timestamp;
        ofBoxPrimitive obj;
        ofVec3f location;
        void generateLocation(std::uniform_real_distribution<float> & distribution, std::default_random_engine & generator, float width, float height, float depth) {
            location.x = 2 * distribution(generator) * width - width;
            location.y = 2 * distribution(generator) * height - height;
            location.z = depth;
        }
        ofVec3f color;
        void generateColor(std::uniform_real_distribution<float> & distribution, std::default_random_engine & generator) {
            color.x = distribution(generator) * 255;
            color.y = distribution(generator) * 255;
            color.z = distribution(generator) * 255;
        }
    };
    const float initialStarZ = 1000;
    float starSpeed = 10;
    const float moveSpeed = 3;
    const int starsCount = 50;
    const float boxSize = 10.0;
    const float bumpRange = 5.0;
    std::vector<Star> stars;
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution;
    bool directionUp = false;
    bool directionDown = false;
    bool directionLeft = false;
    bool directionRight = false;
    // guard parameter
    bool isGuarded = false;
    const int guardCountMax = 50;
    int guardCount = guardCountMax;
    // hit parameter
    bool isHit = false;
    const int hitCountMax = 25;
    int hitCount = 0;
    const int scoreMax = 100;
    int score;
    
    // game states
    bool isStart = false;
    bool isPause = false;
    bool isContinue = false;
    bool isInitialState = true;
    bool isWin = false;
    bool isLose = false;
    int finalTimeLeft = 0;
    int finalScoreLeft = 0;
    int gameTime = 60;
    
    // time parameters
    int lastTime = 0;
    int currentTime = 0;
    int timePassed = 0;
    int lastSpeedUpdateTime = 0;
    // displacement
    float k1,k2;
    int scaleDis = 3000;
    
    
    ofTrueTypeFont myfont;
    

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    // OpenFrameworks has many handy graphical primitive objects
    ofSoundPlayer sound;
    ofSoundPlayer laughSound;
    ofSoundPlayer cheerSound;
    ofSoundPlayer bumpSound;
    ofCamera cam;
};
