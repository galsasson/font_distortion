//
//  MemRecorder.cpp
//  font_distortion
//
//  Created by Gal Sasson on 7/9/14.
//
//

#include "MemRecorder.h"

void MemRecorder::setup(float w, float h)
{
	size = ofVec2f(w, h);
	
	nFrames = 0;
	currentFrame = NULL;
	currentFrameIndex = 0;
}

void MemRecorder::preAllocate(int s)
{
	for (int i=0; i<s; i++)
	{
		ofFbo *f = new ofFbo();
		f->allocate((int)size.x, (int)size.y, GL_RGB);
		frames.push_back(f);
	}
	
	bPreAllocated = true;
	currentFrame = frames[currentFrameIndex];
}

void MemRecorder::startFrame()
{
	if (bPreAllocated) {
		if (currentFrame == NULL) {
			cout<<"error: target frame is not available"<<endl;
			return;
		}
		currentFrame->begin();
	}
	else {
		if (currentFrame != NULL) {
			delete currentFrame;
			currentFrame = NULL;
		}
		
		currentFrame = new ofFbo();
		currentFrame->allocate(size.x, size.y);
		currentFrame->begin();
	}
}

void MemRecorder::endFrame()
{
	if (currentFrame == NULL) {
		cout<<"error: endFrame called but currentFrame is NULL"<<endl;
		return;
	}
	
	currentFrame->end();
	nFrames++;
	currentFrameIndex++;
	
	if (bPreAllocated) {
		if (currentFrameIndex > frames.size()-1) {
			cout<<"warning: reached end of pre-allocated frames"<<endl;
			currentFrame = NULL;
		}
		else {
			currentFrame = frames[currentFrameIndex];
		}
	}
	else {
		frames.push_back(currentFrame);
		currentFrame = NULL;
	}
}

void MemRecorder::saveToFile(std::string path)
{
	
}
