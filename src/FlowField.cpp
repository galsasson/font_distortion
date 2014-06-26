//
//  FlowField.cpp
//  performance
//
//  Created by Gal Sasson on 4/17/14.
//
//

#include "FlowField.h"

FlowField::FlowField()
{
}

FlowField::~FlowField()
{
}

void FlowField::setup(int w, int h)
{
	tmpFbo.allocate(w, h, GL_RGB);

	field.setUseTexture(true);
	field.allocate(w, h, OF_IMAGE_COLOR_ALPHA);
	reset();
}

void FlowField::update()
{
	if (Params::mouseRepulsion)
	{
		reset();
		ofVec2f mousePos(ofGetMouseX(), ofGetMouseY());
		addRepulsion(ofVec2f(ofGetMouseX(), ofGetMouseY()), 100, 10);
//		addLineRepulsion(mousePos - ofVec2f(0, 100), mousePos + ofVec2f(0, 100), 5);
//		for (int y=ofGetMouseY()-100; y<ofGetMouseY()+100; y++)
//		{
//			for (int x=ofGetMouseX()-100; x<ofGetMouseX()+100; x++)
//			{
//				ofFloatColor c = field.getColor(x, y);
//				c += ofFloatColor(ofRandom(0.5), ofRandom(0.5), ofRandom(0.5));
//				field.setColor(x, y, c);
//			}
//			
//		}
	}
}

void FlowField::draw(float x, float y, float w, float h)
{
	if (w == 0 || h == 0) {
		w = field.getWidth();
		h = field.getHeight();
	}

	field.draw(x, y, w, h);
}

void FlowField::randomizeForces()
{
	for (int i=0; i<500; i++)
	{
		float x = ofRandom(field.getWidth());
		float y = ofRandom(field.getHeight());
		float rad = ofRandom(30, 120);
		float force = ofRandom(1, 5);
		if (ofRandom(1)>0.5) {
			addRepulsion(ofVec2f(x, y), rad, force);
		}
		else {
			addAttractor(ofVec2f(x, y), rad, force);
		}
	}
	field.update();
}

void FlowField::randomize()
{
	for (int y=0; y<field.getHeight(); y++)
	{
		for (int x=0; x<field.getWidth(); x++)
		{
			ofFloatColor c(ofRandom(1), ofRandom(1), ofRandom(1), 1);
			field.setColor(x, y, c);
		}
	}
	field.update();
}

void FlowField::reset()
{
	tmpFbo.begin();
	ofClear(127, 127, 127, 255);
	tmpFbo.end();
	ofPixels pixels;
	tmpFbo.readToPixels(pixels);
	field.setFromPixels(pixels);
	field.update();
}

void FlowField::addAttractor(ofVec2f p, float rad, float force)
{
    int minX = MAX((int)(p.x-rad), 0);
    int minY = MAX((int)(p.y-rad), 0);
    int maxX = MIN((int)(p.x+rad), field.getWidth()-1);
    int maxY = MIN((int)(p.y+rad), field.getHeight()-1);

    for (int x=minX; x<maxX; x++)
    {
        for (int y=minY; y<maxY; y++)
        {
            ofVec2f pos = ofVec2f(x, y);
            ofVec2f offset = p-pos;
            float distance = offset.length();
            if (distance < rad) {
                float f = ((rad - distance) / rad) * force;
                offset.normalize();
				ofVec2f fVec = offset*f;
				field.setColor(x, y, COLORZ(fVec.x, fVec.y, (rad-distance)));
            }
        }
    }
	field.update();
}

void FlowField::addForce(ofVec2f p, float rad, ofVec2f force, bool bUpdate)
{
    int minX = MAX((int)(p.x-rad), 0);
    int minY = MAX((int)(p.y-rad), 0);
    int maxX = MIN((int)(p.x+rad), field.getWidth()-1);
    int maxY = MIN((int)(p.y+rad), field.getHeight()-1);

    for (int x=minX; x<maxX; x++)
    {
        for (int y=minY; y<maxY; y++)
        {
			ofVec2f pos = ofVec2f(x, y);
            ofVec2f offset = p-pos;
            float distance = offset.length();
            if (distance < rad) {
                ofVec2f f = force * ((rad - distance) / rad);
				field.setColor(x, y, COLOR(f.x, f.y));
            }
        }
    }
	
	if (bUpdate) {
		field.update();
	}
}

void FlowField::addRepulsion(const ofVec2f &p, float rad, float strength)
{
    int minX = MAX((int)(p.x-rad), 0);
    int minY = MAX((int)(p.y-rad), 0);
    int maxX = MIN((int)(p.x+rad), field.getWidth()-1);
    int maxY = MIN((int)(p.y+rad), field.getHeight()-1);
    
    for (int x=minX; x<maxX; x++)
    {
        for (int y=minY; y<maxY; y++)
        {
            ofVec2f offset = ofVec2f(x, y) - p;
            float distance = offset.length();
            if (distance < rad) {
                ofVec2f f = offset.normalize() * ((rad - distance) / rad * strength);
				field.setColor(x, y, COLORZ(f.x, f.y, -(rad-distance)));
            }
        }
    }
	field.update();
}

void FlowField::addLineRepulsion(const ofVec2f &p, const ofVec2f &q, float strength)
{
    ofVec2f line = q-p;
    ofVec2f perp = line.getPerpendicular();
    ofVec2f perp2 = line.getPerpendicular() * -1;
    
    for (float t=0; t<1; t+=0.01)
    {
        addForce(p + line*t + perp*5, 10, perp*strength, false);
        addForce(p + line*t + perp2*5, 10, perp2*strength, false);
    }
	
	field.update();
}

ofVec2f FlowField::getForce(const ofVec2f& p) const
{
	int x = (int)p.x;
	int y = (int)p.y;
	
	ofFloatColor c = field.getColor(x, y);
	return ofVec2f(c.r, c.g);
}
