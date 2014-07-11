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
	static ofxFloatSlider boxDistIntensity;
	static ofxFloatSlider boxDistTime;
	static ofxFloatSlider boxDistSpeed;
	static ofxFloatSlider distIntensity;
	static ofxFloatSlider dispAmount;
	static ofxFloatSlider distTime;
	static ofxFloatSlider distSpeed;
	
	static ofxFloatSlider mouseDistAmount;
	static ofxFloatSlider mouseDistRange;
	static ofxFloatSlider mouseDistSpeed;
	static ofxFloatSlider mouseDistTime;
	
	static ofxFloatSlider mouseBlowAmount;
	static ofxFloatSlider mouseBlowRange;

	static ofxFloatSlider linesDistFreq;
	static ofxFloatSlider linesDistAmount;
	
	static ofxColorSlider lineColor;
	static ofxColorSlider backgroundColor;

	static ofxFloatSlider tvColorSeparation;
	static ofxFloatSlider tvLinesIntensity;
	static ofxFloatSlider tvFlickerIntensity;
	static ofxFloatSlider tvVignetteIntensity;

	static ofxFloatSlider shaderColor;

	static ofxToggle bShader;
	static ofxFloatSlider bShaderTime;
	static ofxFloatSlider bShaderSpeed;
	static ofxFloatSlider bShaderColSpeed;
	static ofxFloatSlider bShaderZoom;
	
	static ofxFloatSlider globalTime;
	static ofxFloatSlider globalSpeed;
	
	
	static ofxFloatSlider quoteX;
	static ofxFloatSlider quoteY;
	static ofxFloatSlider quoteLineSpace;
};

#endif /* defined(__font_distortion__Params__) */
