//
//  FlowField.h
//  performance
//
//  Created by Gal Sasson on 4/17/14.
//
//

#ifndef __performance__FlowField__
#define __performance__FlowField__

#include <iostream>
#include "ofMain.h"

#define COLOR(fx, fy) \
	ofFloatColor(0.5+fx*0.1, 0.5+fy*0.1, 0.0, 1.0)

#define COLORZ(fx, fy, fz) \
	ofFloatColor(0.5+fx*0.1, 0.5+fy*0.1, 0.5+fz*0.01, 1.0)

class FlowField
{
public:
    FlowField();
    ~FlowField();
    void setup(int w, int h);
    void update();
    void draw(float x=0, float y=0, float w=0, float h=0);
    ofTexture& getTextureRef() { return field.getTextureReference(); }
	
	void randomizeForces();
	void randomize();
	
    void reset();
    
    void addAttractor(ofVec2f p, float rad, float force);
    void addForce(ofVec2f p, float rad, ofVec2f force, bool bUpdate = true);
    void addRepulsion(const ofVec2f& p, float rad, float strength=1);
    void addLineRepulsion(const ofVec2f& p, const ofVec2f& q, float strength);
    
//    void applyStrokeForces(const vector<Particle*>& points);
    ofVec2f getForce(const ofVec2f& p) const;
    
private:
	ofImage field;
	ofFbo tmpFbo;
	
	
};

#endif /* defined(__performance__FlowField__) */
