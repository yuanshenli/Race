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
    };
    const float initialStarZ = 1000;
    const float starSpeed = 10;
    const float moveSpeed = 3;
    const int starsCount = 100;
    const float boxSize = 10.0;
    const float bumpRange = 5.0;
    std::vector<Star> stars;
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution;

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
    
    ofCamera cam;
};
