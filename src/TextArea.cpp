//
//  TextArea.cpp
//  font_distortion
//
//  Created by Gal Sasson on 6/29/14.
//
//

#include "TextArea.h"

CharParams::CharParams(const vector<ofVec3f>& corner)
{
	cornerVerts = corner;
	
	up = down = left = right = NULL;
	distortionAmount = 0;
	lineShiftAmount = 0;
	greenDistAmount = 0;
}

void CharParams::randomize()
{
	if (ofRandom(1) < 0.5) {
		distortionAmount = 1;
	}
	else {
		distortionAmount = 0;
	}

	if (ofRandom(1) < 0.5) {
		lineShiftAmount = 1;
	}
	else {
		lineShiftAmount = 0;
	}

	if (ofRandom(1) < 0.5) {
		greenDistAmount = 1;
	}
	else {
		greenDistAmount = 0;
	}
}

void CharParams::draw()
{
	ofFill();
	ofSetColor(255);
	for (int i=0; i<cornerVerts.size(); i++)
	{
		ofEllipse(cornerVerts[i].x, cornerVerts[i].y, 4, 4);
	}
}

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
	bEnableCursor = false;
	bAutoMode = false;
	cursorX = 0;
	cursorY = 0;
	
	// setup char params
	for (int line=0; line<quote.size(); line++)
	{
		vector<CharParams*> lineVector;
		
		float x = Params::quoteX;
		float y = Params::quoteY + Params::quoteLineSpace * line;
		ofMesh mesh = ResourceManager::getInstance().fontMedium.getStringMesh(quote[line], x, y);
		vector<ofVec3f> verts = mesh.getVertices();
		
		for (int i=0; i<verts.size(); i++)
		{
			std::vector<ofVec3f> charVerts(verts.begin()+i, verts.begin()+i+4);
			CharParams *cp = new CharParams(charVerts);
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

void TextArea::update()
{
	if (!bAutoMode) {
		return;
	}
	
	if (ofRandom(1) < 0.7) {
		// randmize key press
		int key = (int)ofRandom(8);
		switch (key) {
			case 0:
				keyPressed(OF_KEY_LEFT);
				break;
			case 1:
				keyPressed(OF_KEY_RIGHT);
				break;
			case 2:
				keyPressed(OF_KEY_UP);
				break;
			case 3:
				keyPressed(OF_KEY_DOWN);
				break;
			case 4:
				keyPressed('z');
				break;
			case 5:
				keyPressed('x');
				break;
			case 6:
				keyPressed('c');
				break;
			case 7:
				keyPressed('v');
				break;
		}
	}
}

void TextArea::draw()
{
	for (int i=0; i<charParams.size(); i++)
	{
		for (int j=0; j<charParams[i].size(); j++)
		{
			charParams[i][j]->draw();
		}
	}
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
	else if (key == 'e') {
		for (int i=0; i<charParams.size(); i++) {
			for (int j=0; j<charParams[i].size(); j++)
			{
				charParams[i][j]->reset();
			}
		}
	}
	else if (key == 'r') {
		for (int i=0; i<charParams.size(); i++) {
			for (int j=0; j<charParams[i].size(); j++)
			{
				charParams[i][j]->randomize();
			}
		}
	}
	else if (key == 't') {
		bEnableCursor = !bEnableCursor;
	}
	else if (key == 'a') {
		bAutoMode = !bAutoMode;
		bEnableCursor = true;
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
			if (bEnableCursor && line == cursorY && chr == cursorX) {
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
