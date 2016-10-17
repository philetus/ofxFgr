#include "ofxFgr/FuzzySugenoIntegral.h"
#include "ofxFgr/Obsrvr.h"

namespace ofxFgr {

    Obsrvr::Obsrvr() {}

    Obsrvr::~Obsrvr() {}

    void Obsrvr::update(cv::Mat mat) {
        frame = mat;

        cv::GaussianBlur(frame, frame, cv::Size(7,7), 1.5);

        bgs.process(frame, foreground, bkgmodel);
        if(foreground.channels()==1) {
            foregroundBW = foreground;
        } else {
            cv::cvtColor(foreground, foregroundBW, CV_BGR2GRAY);
        }

        //cv::erode(foregroundMask, foregroundMask,cv::Mat(), cv::Point(-1,-1), 4);
        cv::threshold(foregroundBW, foregroundMask, 30, 255, cv::THRESH_BINARY);
        //cv::GaussianBlur(foregroundMask, foregroundMask, cv::Size(3,3), 1.5);
        //cv::erode(foregroundMask, foregroundMask, cv::Mat(), cv::Point(-1,-1), 4);
        //cv::dilate(foregroundMask, foregroundMask, cv::Mat(), cv::Point(-1,-1));
    }

    void Obsrvr::update(ofPixels pixels) {
        update(ofxCv::toCv(pixels));
    }

    void Obsrvr::draw(int x, int y) {
        ofPushMatrix();
        ofTranslate(x, y);
        ofPushStyle();

        ofSetColor(0);

        ofSetColor(255);
        ofxCv::drawMat(frame, 0, 0, 320, 240);
        ofDrawBitmapStringHighlight("INPUT FRAME", 5, 15);

        frameMasked.setTo(cv::Scalar::all((0)));
        frame.copyTo(frameMasked, foregroundMask);

        ofxCv::drawMat(frameMasked, 320, 0, 320, 240);
        ofDrawBitmapStringHighlight("FOREGROUND", 325, 15);

        ofxCv::drawMat(foregroundBW, 0, 240, 320, 240);
        ofDrawBitmapStringHighlight("FOREGROUND B&W", 5, 255);

        ofxCv::drawMat(foregroundMask, 320, 240, 320, 240);
        ofDrawBitmapStringHighlight("FOREGROUND MASK", 325, 255);

        ofPopStyle();
        ofPopMatrix();
    }

    ofImage Obsrvr::getForegroundImage() {
        ofxCv::toOf(foregroundMask, foregroundImg);
        return foregroundImg;
    }

    cv::Mat& Obsrvr::getForegroundMat() {
        return foregroundMask;
    }

    ofPixels Obsrvr::getForegroundPixels() {
        ofxCv::toOf(foregroundMask, foregroundImg);
        return foregroundImg.getPixels();
    }

}
