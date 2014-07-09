//
//  WildChar.cpp
//  font_distortion
//
//  Created by Gal Sasson on 5/27/14.
//
//

#include "WildChar.h"


void WildChar::setup(float x, float y, char c)
{
	pos = ofVec2f(x, y);
	chr = c;

	calcPoints(0);
}

void WildChar::update(float time)
{
	calcPoints(time);
}

void WildChar::draw()
{
	if (chr == ' ') {
		return;
	}

	ofPushMatrix();
	ofTranslate(pos);

	for (int i=0; i<charPoints.size(); i++)
	{
		for (int p=0; p<charPoints[i].size(); p++)
		{
			if (p>0) {
				ofLine(charPoints[i][p-1] + offsets[i][p-1], charPoints[i][p] + offsets[i][p]);
			}
		}

		if (charPoints[i].size() > 1) {
			int lastIndex = charPoints[i].size()-1;
			ofLine(charPoints[i][lastIndex] + offsets[i][lastIndex], charPoints[i][0] + offsets[i][0]);
		}
	}

	ofPopMatrix();
}


void WildChar::calcPoints(float time)
{
	charPoints.clear();
	charOutline.clear();
	offsets.clear();

	ttfchar = ResourceManager::getInstance().fontMedium.getCharacterAsPoints(chr);

	charOutline = ttfchar.getOutline();
	if (charOutline.size() <= 0) {
		return;
	}

	float res = 10;
	if (ofGetMouseX()>0) {
		res = (float)ofGetMouseX()/20;
	}

#if 1
	for (int i=0; i<charOutline.size(); i++)
	{
		charPoints.push_back(vector<ofPoint>());
		offsets.push_back(vector<ofVec2f>());
		for (float p=0; p<=1; p+=(float)1/50)//Params::resolution)
		{
			charPoints[i].push_back(charOutline[i].getPointAtPercent(p));
			offsets[i].push_back(
								 ofVec2f(
										 (ofNoise(1*time+p*10)-0.5)*10,
										 (ofNoise(1*time+p*10+10000)-0.5)*10
										 )
								 );
		}
	}
#endif

}
