//
//  WildChar.h
//  font_distortion
//
//  Created by Gal Sasson on 5/27/14.
//
//

#ifndef __font_distortion__WildChar__
#define __font_distortion__WildChar__

#include <iostream>
#include "ofMain.h"

#include "Params.h"
#include "ResourceManager.h"

class WildChar
{
public:
    void setup(float x, float y, char c);
    void update(float time);
    void draw();
    
private:
    char chr;
    ofVec2f pos;
//    float resolution;
    
    
    void calcPoints(float time);
    
    ofTTFCharacter ttfchar;
    vector<ofPolyline> charOutline;
    vector<vector<ofPoint> > charPoints;
    vector<vector<ofVec2f> > offsets;

};

#endif /* defined(__font_distortion__WildChar__) */
