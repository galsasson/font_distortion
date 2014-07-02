//
//  Animation.cpp
//  font_distortion
//
//  Created by Gal Sasson on 7/1/14.
//
//

#include "Animation.h"

void Animation::setup()
{
	param1.animateFromTo(0, 255);
	param1.setCurve(EASE_IN);
	param1.setDuration(50);
}

void Animation::update(float dt)
{
	param1.update(dt);
	ofColor c = Params::backgroundColor;
	c.r = param1.val();
	Params::backgroundColor = c;
}
