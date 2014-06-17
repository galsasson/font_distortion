//
//  FlowField.h
//  font_distortion
//
//  Created by Gal Sasson on 6/17/14.
//
//

#ifndef __font_distortion__FlowField__
#define __font_distortion__FlowField__

#include <iostream>
#include "ofMain.h"

class FlowField
{
public:
    FlowField();
    void setup(float width, float height);
    void randomize();
    
    void draw(float x = 0, float y = 0, float width = -1, float height = -1);
    
    ofTexture& getTextureRef() { return fbo.getTextureReference(); }
    
private:
    ofFbo fbo;
        
};

#endif /* defined(__font_distortion__FlowField__) */
