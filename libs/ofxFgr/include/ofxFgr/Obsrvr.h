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

		ofPixels getForegroundPixels();
		bool getForegroundMask(ofImage & img);
		cv::Mat& getForegroundMat();

	private:

		FuzzySugenoIntegral bgs;
		cv::Mat frame;
		cv::Mat frameBW;
		cv::Mat foreground;
		cv::Mat foregroundBW;
		cv::Mat foregroundMask;
		cv::Mat frameMasked;
		cv::Mat bkgmodel;
		ofImage foregroundImg;
	};

}
