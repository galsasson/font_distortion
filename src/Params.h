//
//  Params.h
//  font_distortion
//
//  Created by Gal Sasson on 5/28/14.
//
//

#ifndef __font_distortion__Params__
#define __font_distortion__Params__

#include <iostream>
#include "ofxGui.h"

class Params
{
public:
	static ofxFloatSlider distIntensity;
	static ofxFloatSlider distSpeed;

	static ofxColorSlider lineColor;
	static ofxColorSlider backgroundColor;

	static ofxFloatSlider tvColorSeparation;
	static ofxFloatSlider tvLinesIntensity;
	static ofxFloatSlider tvFlickerIntensity;

	static ofxFloatSlider flowFieldDistortion;
	static ofxFloatSlider zDistScale;
	static ofxFloatSlider flowFieldColor;

	static ofxFloatSlider tempVar;
};

#endif /* defined(__font_distortion__Params__) */
