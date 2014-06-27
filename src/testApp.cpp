#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
//    control.setup("Font Distortion", 5000);
//    control.addInput("Resolution", &Params::resolution);
//    control.addInput("Distortion", &Params::distortion);
//    control.addInput("Line Width", &Params::lineWidth);
//    control.addInput("Line Color", &Params::lineColor);
//    control.addInput("Background Color", &Params::backgroundColor);
    /*
     ÒEverywhere we remain unfree and chained to technology, whether we passionately affirm or deny it. But we are delivered over to it in the worst possible way when we regard it as something neutral."
     Martin Heidegger
     */
	ofEnableAlphaBlending();

	initGui();

	quote.push_back("If it should turn out to be true that knowledge (in the modern sense");
	quote.push_back("of know-how) and thought have parted company for good, then we");
	quote.push_back("would indeed become the helpless slaves, not so much of our machines");
	quote.push_back("as of our know-how, thoughtless creatures at the mercy of every gadget");
	quote.push_back("which is technically possible, no matter how murderous it is.");
	quote.push_back("");
	quote.push_back("Hannah Arendt");
	quote.push_back("The Human Condition, 1958");

	font0Fbo.allocate(ofGetWindowWidth()*2, ofGetWindowHeight()*2, GL_RGBA);
	font0Fbo.begin();
	ofClear(0, 0, 0, 255);
	font0Fbo.end();
	font1Fbo.allocate(ofGetWindowWidth()*2, ofGetWindowHeight()*2, GL_RGBA);
	font1Fbo.begin();
	ofClear(0, 0, 0, 255);
	font1Fbo.end();
	fractalShaderFbo.allocate(ofGetWindowWidth()*2, ofGetWindowHeight()*2, GL_RGBA);
	fractalShaderFbo.begin();
	ofClear(0, 0, 0, 255);
	fractalShaderFbo.end();
	
	renderColorTexture();
	
//	quoteOnly.allocate(ofGetWindowWidth()*2, ofGetWindowHeight()*2, GL_RGBA);
//	quoteOnly.begin();
//	ofClear(0, 0, 0, 0);
//	ofSetColor(Params::lineColor, 100);
//	drawQuote(quoteOnly.getWidth(), quoteOnly.getHeight());
//	quoteOnly.end();
	
	tvShader.load("shaders/tv");
	distShader.load("shaders/dist");
	flowFieldShader.load("shaders/flowfield");
	normalFontShader.load("shaders/font_normal");

	flowField.setup(ofGetWindowWidth(), ofGetWindowHeight());
	flowField.randomizeForces();

//	time = 0;

	backgroundFbo.allocate(ofGetWindowWidth(), ofGetWindowHeight());
	backgroundShader.load("shaders/background");
	plane.set(ofGetWindowWidth(), ofGetWindowHeight());
	plane.setPosition(ofGetWindowWidth()/2, ofGetWindowHeight()/2, 0);
	plane.setResolution(2, 2);
}

void testApp::initGui()
{
	Params::distIntensity.setup("Distortion", 0, 0, 100);
	Params::distTime.setup("Distortion Time", 0, 0, 1000);
	Params::distSpeed.setup("Distortion Speed", 0, 0, 20);

	Params::flowFieldDistortion.setup("FlowField Distortion", 0, 0, 1);
	Params::zDistScale.setup("Flowfield z Distortion", 0, 0, 5);
	Params::flowFieldColor.setup("Flow field color", 0, 0, 1);

	Params::lineColor.setup("Line Color", ofColor(50), ofColor(0), ofColor(255));
	Params::backgroundColor.setup("Background Color", ofColor(200), ofColor(0), ofColor(255));
	Params::tvColorSeparation.setup("Color Separation", 0, 0, 50);
	Params::tvLinesIntensity.setup("TV Lines Intensity", 0.1, 0, 1);
	Params::tvFlickerIntensity.setup("TV Flicker Intensity", 0.03, 0, 1);

	Params::drawFixedQuote.setup("Draw fixed quote", false);
	Params::fixedQuoteAlpha.setup("Fixed quote alpha", 255, 0, 255);
	
	Params::mouseRepulsion.setup("Mouse Repulsion", false);
	
	Params::tempVar.setup("Temp Var", 0, 0, 1);
	
	Params::bShader.setup("B.Shader", false);
	Params::bShaderTime.setup("B.Shader Time", 0, 0, 1000);
	Params::bShaderSpeed.setup("B.Shader Speed", 0, -1, 1);
	Params::bShaderZoom.setup("B.Shader Zoom", 0.25, 0, 10);
	Params::bShaderColSpeed.setup("B.Shader C.Speed", 0.005, 0, 1);
	
	Params::globalTime.setup("Global Time", 0, 0, 1000);
	Params::globalSpeed.setup("Global Speed", 1, -1, 1);

	gui.setup();
	gui.add(&Params::globalTime);
	gui.add(&Params::globalSpeed);

	gui.add(&Params::bShader);
	gui.add(&Params::bShaderTime);
	gui.add(&Params::bShaderSpeed);
	gui.add(&Params::bShaderZoom);
	gui.add(&Params::bShaderColSpeed);
	
	gui.add(&Params::distIntensity);
	gui.add(&Params::distTime);
	gui.add(&Params::distSpeed);

	gui.add(&Params::flowFieldDistortion);
	gui.add(&Params::zDistScale);
	gui.add(&Params::flowFieldColor);

	gui.add(&Params::tvColorSeparation);
	gui.add(&Params::tvLinesIntensity);
	gui.add(&Params::tvFlickerIntensity);
	gui.add(&Params::lineColor);
	gui.add(&Params::backgroundColor);
	gui.add(&Params::drawFixedQuote);
	gui.add(&Params::fixedQuoteAlpha);
	gui.add(&Params::mouseRepulsion);
	gui.add(&Params::tempVar);

	gui.loadFromFile("settings.xml");

	bToggleGui = true;
}

void testApp::drawWithDistShader()
{
	distShader.begin();
	distShader.setUniformTexture("fontTex", ResourceManager::getInstance().font.getFontTexture(), 1);
	distShader.setUniform2f("time2d", Params::distTime, Params::distTime+10000);
	ofFloatColor col = (ofFloatColor)(ofColor)Params::lineColor;
	distShader.setUniform4f("globalColor", col.r, col.g, col.b, col.a);
	distShader.setUniform1f("distIntensity", Params::distIntensity);

	drawQuote(font0Fbo.getWidth(), font0Fbo.getHeight());

	distShader.end();
}

void testApp::drawWithFlowShader()
{
	ofClear(0, 0, 0, 0);

	flowFieldShader.begin();
	flowFieldShader.setUniformTexture("fontTex", ResourceManager::getInstance().font.getFontTexture(), 1);
#if 1
	flowFieldShader.setUniformTexture("flowFieldTex", flowField.getTextureRef(), 2);
//	flowFieldShader.setUniformTexture("flowFieldTex", background.getTextureReference(), 2);
	flowFieldShader.setUniformTexture("colorTex", backgroundFbo.getTextureReference(), 3);
#else
	flowFieldShader.setUniformTexture("flowFieldTex", flowField.getTextureRef(), 2);
	flowFieldShader.setUniformTexture("colorTex", colorTexture.getTextureReference(), 3);
#endif
	flowFieldShader.setUniform2f("time2d", Params::distTime, Params::distTime+10000);
	ofFloatColor col = (ofFloatColor)(ofColor)Params::lineColor;
	flowFieldShader.setUniform4f("globalColor", col.r, col.g, col.b, col.a);
	flowFieldShader.setUniform1f("distIntensity", Params::distIntensity);
	flowFieldShader.setUniform1f("flowFieldDistortion", Params::flowFieldDistortion);
	flowFieldShader.setUniform1f("distZScale", Params::zDistScale);
	flowFieldShader.setUniform1f("flowFieldColor", Params::flowFieldColor);

	drawQuote(font0Fbo.getWidth(), font0Fbo.getHeight());

	flowFieldShader.end();
	
//	if (Params::drawFixedQuote) {
//		ofSetColor(Params::lineColor, Params::fixedQuoteAlpha);
//		drawQuote(second.getWidth(), second.getHeight());
//	}
	

//	second.end();
}

//--------------------------------------------------------------
void testApp::update(){

	Params::globalTime += Params::globalSpeed;
	Params::bShaderTime += Params::bShaderSpeed*Params::globalSpeed;
	Params::distTime += Params::distSpeed*Params::globalSpeed ;
	
	flowField.update();

	fractalShaderFbo.begin();
	renderBackground();
	fractalShaderFbo.end();
	
	// render the background
	backgroundFbo.begin();
	if (Params::bShader) {
		fractalShaderFbo.draw(0, 0);
//		renderBackground();
	}
	else {
		ofSetColor(Params::backgroundColor);
		ofFill();
		ofRect(0, 0, backgroundFbo.getWidth(), backgroundFbo.getHeight());
	}
	backgroundFbo.end();
	
//	drawWithFlowShader();
	font0Fbo.begin();
	renderNormalQuote();
	font0Fbo.end();

	font1Fbo.begin();
	renderBoxedQuote();
	font1Fbo.end();
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofSetColor(255);
	backgroundFbo.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	if (bToggleFont0) {
		font0Fbo.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	}
	
	if (bToggleFont1) {
		font1Fbo.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	}
	
	if (bToggleGui) {
		ofSetColor(255);
		backgroundFbo.draw(0, 0, 200, 130);
		font0Fbo.draw(200, 0, 200, 130);
		font1Fbo.draw(400, 0, 200, 130);
		flowField.draw(600, 0, 200, 130);
		colorTexture.draw(800, 0, 200, 130);
		
		gui.draw();
	}
}

void testApp::setupString(string str)
{
	string str2(str);
	ofStringReplace(str2, " ", "-");
	float x=0;
	for (int i=0; i<str2.length(); i++)
	{
		if (i>0) {
			stringstream ss;
			ss << str2[i-1];
			x += ResourceManager::getInstance().font.stringWidth(ss.str()) + 5;
		}
		
		WildChar *wchar = new WildChar();
		wchar->setup(x, 100, str[i]);
		chars.push_back(wchar);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	cout << key << endl;

	if(key == 's') {
		gui.saveToFile("settings.xml");
	}
	else if(key == 'l') {
		gui.loadFromFile("settings.xml");
	}
	else if (key == 'h') {
		bToggleGui = !bToggleGui;
	}
	else if (key == '1') {
		bToggleFont0 = !bToggleFont0;
	}
	else if (key == '2') {
		bToggleFont1 = !bToggleFont1;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::drawQuote(float resx, float resy)
{
	ofVec2f resolution(resx, resy);
	float quoteHeight = 0;
	for (int i=0; i<quote.size(); i++)
	{
		quoteHeight += ResourceManager::getInstance().font.stringHeight(quote[i])+80;
	}

//	float y = (resolution.y - quoteHeight)/2 + 300;
	for (int i=0; i<quote.size(); i++)
	{
		float x = 250;
		float y = 600+quoteHeight / quote.size() * i;
		// (resolution.x - ResourceManager::getInstance().font.stringWidth(quote[i]))/2;
		ResourceManager::getInstance().font.drawString(quote[i], x, y);
//		ResourceManager::getInstance().font.getStringMesh(quote[i], x, y).draw();
	}
}

void testApp::renderBackground()
{
	backgroundShader.begin();
	backgroundShader.setUniform1f("iGlobalTime", Params::bShaderTime);
	backgroundShader.setUniform3f("iResolution", ofGetWindowWidth()*2, ofGetWindowHeight()*2, 0);
	backgroundShader.setUniform1f("iRotSpeed", Params::bShaderSpeed);
	backgroundShader.setUniformTexture("iChannel0", colorTexture.getTextureReference(), 1);
	backgroundShader.setUniform1f("zoom", Params::bShaderZoom);
	backgroundShader.setUniform1f("colspeed", Params::bShaderColSpeed);
//	backgroundShader.setUniformTexture("iChannel0", flowField.getTextureRef(), 1);
	//	ofSetColor(100, 0, 0);
	ofSetColor(Params::backgroundColor);
	plane.draw();
	backgroundShader.end();
}

//#define GENERATE_NEW_NOISE_TEX

void testApp::renderColorTexture()
{
#ifdef GENERATE_NEW_NOISE_TEX
	colorTexture.allocate(ofGetWindowWidth()*2, ofGetWindowHeight()*2, OF_IMAGE_COLOR_ALPHA);
	for (int y=0; y<colorTexture.getHeight(); y++)
	{
		for (int x=0; x<colorTexture.getWidth(); x++)
		{
			colorTexture.setColor(x, y, ofColor(ofRandom(255), ofRandom(255), ofRandom(255), 255));
		}
	}
	colorTexture.update();
	colorTexture.saveImage("noise.png");
#else
	colorTexture.loadImage("noise.png");
#endif
}

void testApp::renderBoxedQuote()
{
	ofClear(0, 0, 0, 0);
	
	distShader.begin();
	distShader.setUniformTexture("fontTex", ResourceManager::getInstance().font.getFontTexture(), 1);
	distShader.setUniformTexture("colorTex", fractalShaderFbo.getTextureReference(), 2);
	distShader.setUniform1f("flowFieldColor", Params::flowFieldColor);
	distShader.setUniform2f("time2d", Params::distTime, Params::distTime+10000);
	ofFloatColor col = (ofFloatColor)(ofColor)Params::lineColor;
	distShader.setUniform4f("globalColor", col.r, col.g, col.b, col.a);
	distShader.setUniform1f("distIntensity", Params::distIntensity);
	
	ofSetColor(Params::lineColor);
	drawQuote(font0Fbo.getWidth(), font0Fbo.getHeight());
	
	distShader.end();
}

void testApp::renderNormalQuote()
{
	ofClear(0, 0, 0, 0);
	normalFontShader.begin();
	normalFontShader.setUniformTexture("fontTex", ResourceManager::getInstance().font.getFontTexture(), 1);
	ofFloatColor col = (ofFloatColor)(ofColor)Params::lineColor;
	normalFontShader.setUniform4f("globalColor", col.r, col.g, col.b, col.a);
	
	ofSetColor(Params::lineColor);
	drawQuote(font0Fbo.getWidth(), font0Fbo.getHeight());
	
	normalFontShader.end();
}

void testApp::applyTvShader(const ofFbo &fbo)
{
	tvShader.begin();
	tvShader.setUniform1f("time", Params::globalTime);
	tvShader.setUniform2f("resolution", ofGetWindowWidth()*2, ofGetWindowHeight()*2);
	tvShader.setUniform1f("colorSeparation", Params::tvColorSeparation);
	tvShader.setUniform1f("linesIntensity", Params::tvLinesIntensity);
	tvShader.setUniform1f("flickerIntensity", Params::tvFlickerIntensity);

	tvShader.end();	

}
