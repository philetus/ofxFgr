/*
track background and identify foreground elements in video stream
*/

#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFgr/FuzzySugenoIntegral.h"

namespace ofxFgr {

    class Obsrvr
    {

    public:

        Obsrvr();
        ~Obsrvr();

        void draw(int x=0, int y = 0);
        void update(cv::Mat mat);
        void update(ofPixels pixels);

        bool getForegroundMask(ofImage & img);
        bool getBackgroundModel(ofImage & img);

        const std::vector<ofPolyline>& getFigures() const;

        ofParameter<float> minArea, maxArea, threshold;
        ofParameter<bool> holes;   

    private:

        FuzzySugenoIntegral bgs;
        cv::Mat frame;
        cv::Mat frameBW;
        cv::Mat foreground;
        cv::Mat foregroundBW;
        cv::Mat foregroundMask;
        cv::Mat frameMasked;
        cv::Mat bkgmodel;

        ofxCv::ContourFinder fndr;
        std::vector<ofPolyline> fgrs;
    };

}
