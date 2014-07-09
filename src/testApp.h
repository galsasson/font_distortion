#pragma once

#include "ofMain.h"
#include "ofxControlease.h"
#include "ofxGui.h"
#include "ofxControlease.h"

#include "Params.h"

#include "FlowField.h"
#include "WildChar.h"
#include "TextArea.h"

#include "Animation.h"

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
	void initControlease();
	void drawQuote(const ofVec2f& p, ofShader& shader);
	
	
	void drawWithDistShader();
	void drawWithFlowShader();

	vector<std::string> quote;

	vector<WildChar*> chars;

	ofxPanel gui;
	bool bToggleGui;

	ofFbo backgroundFbo;
	ofFbo fractalShaderFbo;
	ofFbo font0Fbo;
	ofFbo font1Fbo;
	
	ofShader tvShader;
	ofImage colorTexture;
	ofShader distShader;

	float time;

	void setupString(string str);

	ofEasyCam cam;
	
	ofShader backgroundShader;
	ofPlanePrimitive plane;
//	float bShaderTime;
	void renderBackground();
	
	void renderColorTexture();
	
	void renderBoxedQuote();
	
	ofShader normalFontShader;
	void renderNormalQuote();
	
	bool bToggleFont0 = false;
	bool bToggleFont1 = true;
	
	void applyTvShader(const ofFbo& fbo);
	
	float prevSpeed;
	
	TextArea textArea;
	float cursorTime;
	
	Animation animation;
	
	ofxControlease control;
};
