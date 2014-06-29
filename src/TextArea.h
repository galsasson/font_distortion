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

class CharParams;

class TextArea
{
public:
	~TextArea();
	void setup(vector<std::string> quote);
	
	void keyPressed(int key);
	void keyReleased(int key);
	
	ofTexture& getTextureRef() { return stateImage.getTextureReference(); }
	
private:
	
	int cursorX, cursorY;
	
	ofImage stateImage;
	vector< vector<CharParams*> > charParams;
	void updateTexture();
};

class CharParams
{
public:
	CharParams();
	
	float distortionAmount;
	
	CharParams *up, *down, *left, *right;
};
#endif /* defined(__font_distortion__TextArea__) */
