#include "ofxFgr/FuzzySugenoIntegral.h"
#include "ofxFgr/Obsrvr.h"

namespace ofxFgr {

    Obsrvr::Obsrvr() {
        minArea.set("min area", 60, 1, 500);
        maxArea.set("max area", 1500, 1, 2000);
        threshold.set("Threshold", 16, 0, 255);
        figErode.set("erode", 4, 1, 32);
        figDilate.set("dilate", 20, 1, 32);
        holes.set("holes", false);
    }

    Obsrvr::~Obsrvr() {}

    void Obsrvr::update(cv::Mat mat) {
        frame = mat;

        cv::GaussianBlur(frame, frame, cv::Size(7,7), 1.5);

        bgs.process(frame, foreground, bkgmodel);

        // if foreground is returned process foreground mask from it
        if(!foreground.empty()) {
            if(foreground.channels()==1) {
                foregroundBW = foreground;
            } else {
                cv::cvtColor(foreground, foregroundBW, CV_BGR2GRAY);
            }

            cv::threshold(foregroundBW, foregroundMask, 30, 255, cv::THRESH_BINARY);

            // filter out small bits & connect fragments into larger blobs
            cv::Mat kd = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(figErode, figErode), cv::Point(-1,-1));
            cv::Mat ke = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(figDilate, figDilate), cv::Point(-1,-1));
            cv::erode(foregroundMask, foregroundMask, kd, cv::Point(-1, -1), 1, 1, 1);
            cv::dilate(foregroundMask, foregroundMask, ke, cv::Point(-1, -1), 2, 1, 1);

            // find contours in foregound mask
            fndr.setMinAreaRadius(minArea);
            fndr.setMaxAreaRadius(maxArea);
            fndr.setThreshold(threshold);
            fndr.setFindHoles(holes);
            fndr.findContours(foregroundMask);

            // smooth contours
            fgrs.clear();
            for(int i = 0; i < fndr.size(); i++){
                ofPolyline fgr = fndr.getPolyline(i);
                fgr.simplify(0.5);
                fgr = fgr.getSmoothed(3, 0.5);
                fgrs.push_back(fgr);
            }
        }

    }

    const vector<ofPolyline>& Obsrvr::getFigures() const {
        return fgrs;
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

    bool Obsrvr::getForegroundMask(ofImage & img) {
        if (foregroundMask.empty()) return false;

        ofxCv::toOf(foregroundMask, img);
        img.update();
        return true;
    }

    bool Obsrvr::getBackgroundModel(ofImage & img) {
        if (bkgmodel.empty()) return false;

        ofxCv::toOf(bkgmodel, img);
        img.update();
        return true;
    }
}
