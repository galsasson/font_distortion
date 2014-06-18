//
//  FlowField.cpp
//  font_distortion
//
//  Created by Gal Sasson on 6/17/14.
//
//

#include "FlowField.h"

FlowField::FlowField()
{
    
}

void FlowField::setup(float width, float height)
{
	fbo.allocate(width, height, GL_RGBA);
	fbo.begin();
	ofClear(0, 0, 0, 255);
	fbo.end();
}

void FlowField::randomize()
{
	fbo.begin();
	for (int i=0; i<5000; i++)
	{
		ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
		ofRect(ofRandom(fbo.getWidth()), ofRandom(fbo.getHeight()),
			   ofRandom(50), ofRandom(50));
	}
	fbo.end();
}

void FlowField::draw(float x, float y, float width, float height)
{
	if (width < 0 || height < 0)
	{
		width = fbo.getWidth();
		height = fbo.getHeight();
	}

	fbo.draw(x, y, width, height);
}
