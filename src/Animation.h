//
//  Animation.h
//  font_distortion
//
//  Created by Gal Sasson on 7/1/14.
//
//

#ifndef __font_distortion__Animation__
#define __font_distortion__Animation__

#include <iostream>
#include "ofMain.h"
#include "ofxAnimatableFloat.h"

#include "Params.h"

class Animation
{
public:
	
	void setup();
	
	void update(float dt);
	
	ofxAnimatableFloat param1;
	
};

#endif /* defined(__font_distortion__Animation__) */
