//
//  TextArea.h
//  font_distortion
//
//  Created by Gal Sasson on 6/29/14.
//
//

#ifndef __font_distortion__TextArea__
#define __font_distortion__TextArea__

#include <iostream>
#include "ofMain.h"

#include "ResourceManager.h"
#include "Params.h"

class CharParams;

class TextArea
{
public:
	~TextArea();
	void setup(vector<std::string> quote);
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	
	ofTexture& getTextureRef() { return stateImage.getTextureReference(); }
	
private:
	
	bool bEnableCursor;
	bool bAutoMode;
	int cursorX, cursorY;
	
	ofImage stateImage;
	vector< vector<CharParams*> > charParams;
	void updateTexture();
};




class CharParams
{
public:
	CharParams(const vector<ofVec3f>& corner);
	void reset() { distortionAmount=0; lineShiftAmount=0; greenDistAmount = 0; }
	void randomize();
	
	void draw();
	
	float distortionAmount;
	float lineShiftAmount;
	float greenDistAmount;
	
	vector<ofVec3f> cornerVerts;
	CharParams *up, *down, *left, *right;
	
	// TODO:
	// vector<Effect*> effects;
	// void addEffect(effect, keyCode);
	
};
#endif /* defined(__font_distortion__TextArea__) */
