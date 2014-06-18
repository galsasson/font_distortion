#pragma once

#include "ofMain.h"
#include "ofxControlease.h"
#include "ofxGui.h"

#include "Params.h"

#include "FlowField.h"
#include "WildChar.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
private:

	void initGui();
	void drawQuote();
	void drawWithDistShader();
	void drawWithFlowShader();

	vector<std::string> quote;

	FlowField flowField;
	ofShader flowFieldShader;

	vector<WildChar*> chars;

	ofxPanel gui;

	ofShader tvShader;
	ofFbo virgin;
	ofFbo second;
	ofShader distShader;

	float time;

	void setupString(string str);

	ofEasyCam cam;
};