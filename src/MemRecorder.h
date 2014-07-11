//
//  MemRecorder.h
//  font_distortion
//
//  Created by Gal Sasson on 7/9/14.
//
//

#ifndef __font_distortion__MemRecorder__
#define __font_distortion__MemRecorder__

#include <iostream>
#include "ofMain.h"

class MemRecorder
{
public:
	void setup(float w, float h);
	
	void preAllocate(int s);
	
	void startFrame();
	void endFrame();
	
	void saveToFile(std::string path);
	
private:
	int nFrames;
	bool bPreAllocated;
	
	ofVec2f size;
	vector<ofFbo*> frames;
	ofFbo* currentFrame;
	int currentFrameIndex;
};


#endif /* defined(__font_distortion__MemRecorder__) */
