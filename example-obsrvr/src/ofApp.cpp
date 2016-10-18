#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
    camWdth = 1280;
    camHght = 720;
    setCam(&cam, "C920"); // try to find external webcam
	cam.setup(camWdth, camHght);
    
    gui.setup();
    gui.add(minArea.set("Min area", 100, 1, 500));
    gui.add(maxArea.set("Max area", 1500, 1, 2000));
    gui.add(threshold.set("Threshold", 16, 0, 255));
    gui.add(holes.set("Holes", true));
    /*
    gui.add(resetBackground.set("Reset Background", false));
    gui.add(learningTime.set("Learning Time", 30, 0, 30));
    gui.add(thresholdValue.set("Threshold Value", 10, 0, 255));
    */
}

void ofApp::update() {
	cam.update();
    /*
    if(resetBackground) {
        background.reset();
        resetBackground = false;
    }
    */
	if(cam.isFrameNew()) {
        obs.update(cam.getPixels());
        if(obs.getForegroundMask(fgmsk)) {
            contourFinder.setMinAreaRadius(minArea);
            contourFinder.setMaxAreaRadius(maxArea);
            contourFinder.setThreshold(threshold);
            contourFinder.findContours(fgmsk);
            contourFinder.setFindHoles(holes);
        }
        /*
        background.setLearningTime(learningTime);
        background.setThresholdValue(thresholdValue);
		background.update(cam, thresholded);
		thresholded.update();
        */
	}
}

void ofApp::draw() {
    if(obs.getBackgroundModel(bgmdl)) {
        bgmdl.draw(0, 0);
    }
    if(obs.getForegroundMask(fgmsk)) {
        fgmsk.draw(camWdth, 0);
    }

    contourFinder.draw();

    //gui.draw();


    //obs.draw();
    
    /*
	cam.draw(0, 0);
    if(thresholded.isAllocated()) {
        thresholded.draw(640, 0);
    }
    */
}
