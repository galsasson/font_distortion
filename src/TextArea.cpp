//
//  TextArea.cpp
//  font_distortion
//
//  Created by Gal Sasson on 6/29/14.
//
//

#include "TextArea.h"

TextArea::~TextArea()
{
	for (int line=0; line<charParams.size(); line++)
	{
		for (int chr=0; chr<charParams[line].size(); chr++)
		{
			delete charParams[line][chr];
		}
	}
}

void TextArea::setup(vector<std::string> quote)
{
	cursorX = 0;
	cursorY = 0;
	
	// setup char params
	for (int line=0; line<quote.size(); line++)
	{
		vector<CharParams*> lineVector;
		
		// skip empty lines
//		if (quote[line].length() == 0) {
//			continue;
//		}
		
		for (int chr=0; chr<quote[line].length(); chr++)
		{
			if (quote[line][chr] == ' ') {
				continue;
			}
			
			CharParams *cp = new CharParams();
//			if (line==cursorY && chr==cursorX) {
//				cp->distortionAmount = 1;
//			}
//			else {
//				cp->distortionAmount = 0;
//			}
			lineVector.push_back(cp);
		}
		
		charParams.push_back(lineVector);
	}
	
	stateImage.allocate(128, 128, OF_IMAGE_COLOR_ALPHA);
	stateImage.setCompression(OF_COMPRESS_NONE);
	for (int i=0; i<stateImage.getHeight(); i++) {
		for (int j=0; j<stateImage.getWidth(); j++) {
			stateImage.setColor(j, i, ofFloatColor(0, 0, 0, 0));
		}
	}
	stateImage.update();
	updateTexture();
}

void TextArea::keyPressed(int key)
{
	if (key == OF_KEY_UP) {
		if (cursorY > 0) {
			cursorY--;
			while (charParams[cursorY].size() == 0) {
				cursorY--;
			}
			if (cursorX > charParams[cursorY].size()-1) {
				cursorX = charParams[cursorY].size()-1;
			}
		}
		
	}
	else if (key == OF_KEY_DOWN) {
		if (cursorY < charParams.size()-1) {
			cursorY++;
			while (charParams[cursorY].size() == 0) {
				cursorY++;
			}
			if (cursorX > charParams[cursorY].size()-1) {
				cursorX = charParams[cursorY].size()-1;
			}
		}
	}
	else if (key == OF_KEY_LEFT) {
		if (cursorX > 0) {
			cursorX--;
		}
	}
	else if (key == OF_KEY_RIGHT) {
		if (cursorX < charParams[cursorY].size()-1) {
			cursorX++;
		}
	}
	else if (key == 'z') {
		// 'z' clear all distortions
		CharParams *cp = charParams[cursorY][cursorX];
		cp->distortionAmount = 0;
		cp->lineShiftAmount = 0;
		cp->greenDistAmount = 0;
	}
	else if (key == 'x') {
		// 'x' toggles distortion
		CharParams *cp = charParams[cursorY][cursorX];
		if (cp->distortionAmount > 0) {
			cp->distortionAmount = 0;
		}
		else {
			cp->distortionAmount = 1;
		}
	}
	else if (key == 'c') {
		// 'c' toggles line shift distortion
		CharParams *cp = charParams[cursorY][cursorX];
		if (cp->lineShiftAmount > 0) {
			cp->lineShiftAmount = 0;
		}
		else {
			cp->lineShiftAmount = 1;
		}
	}
	else if (key == 'v') {
		// 'c' toggles line shift distortion
		CharParams *cp = charParams[cursorY][cursorX];
		if (cp->greenDistAmount > 0) {
			cp->greenDistAmount = 0;
		}
		else {
			cp->greenDistAmount = 1;
		}
	}
	
	updateTexture();
}


void TextArea::updateTexture()
{
	for (int line=0; line<charParams.size(); line++)
	{
		for (int chr=0; chr<charParams[line].size(); chr++)
		{
			CharParams *cp = charParams[line][chr];
			float isCursor = 0;
			if (line == cursorY && chr == cursorX) {
				isCursor = 1;
			}
			stateImage.setColor(chr*2, line*2, ofFloatColor(cp->distortionAmount, cp->lineShiftAmount, cp->greenDistAmount, isCursor));
		}
	}
	
	// add cursor color
//	CharParams* cp = charParams[cursorY][cursorX];
//	stateImage.setColor(cursorX*2, cursorY*2, cursorColo);
	
	stateImage.update();
}


CharParams::CharParams()
{
	up = down = left = right = NULL;
	distortionAmount = 0;
	lineShiftAmount = 0;
	greenDistAmount = 0;
}