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
//	ofDisableArbTex();
	
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
	
	prevSpeed = 0;
	
	textArea.setup(quote);
}

void testApp::initGui()
{
	Params::boxDistIntensity.setup(		"Box Distortion", 0, 0, 1000);
	Params::boxDistTime.setup(			"Box Dist. Time", 0, 0, 1000);
	Params::boxDistSpeed.setup(			"Box Dist. Speed", 0, 0, 1);
	
	Params::distIntensity.setup(		"All Dist. Amt.", 0, 0, 200);
	Params::dispAmount.setup(			"All Displacement", 0, 0, 200);
	Params::distTime.setup(				"All Dist. Time", 0, 0, 1000);
	Params::distSpeed.setup(			"All Dist. Speed", 0, 0, 20);
	
	Params::particlesDistTime.setup(	"Part. Dist. Time", 0, 0, 1000);
	Params::particlesDistSpeed.setup(	"Part. Dist. Speed", 0, 0, 1);
	Params::particlesDistAmount.setup(	"Part. Dist. Amt.", 0, 0, 200);

	Params::flowFieldDistortion.setup(	"FlowField Distortion", 0, 0, 1);
	Params::zDistScale.setup(			"Flowfield z Distortion", 0, 0, 5);
	Params::flowFieldColor.setup(		"Flow field color", 0, 0, 1);

	Params::lineColor.setup(			"Line Color", ofColor(50, 50, 50, 255), ofColor(0, 0, 0 ,0), ofColor(255, 255, 255, 255));
	Params::backgroundColor.setup(		"Background Color", ofColor(200, 200, 200, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255));
	Params::tvColorSeparation.setup(	"Color Separation", 0, 0, 50);
	Params::tvLinesIntensity.setup(		"TV Lines Intensity", 0.1, 0, 1);
	Params::tvFlickerIntensity.setup(	"TV Flicker Intensity", 0.03, 0, 1);

	Params::drawFixedQuote.setup(		"Draw fixed quote", false);
	Params::fixedQuoteAlpha.setup(		"Fixed quote alpha", 255, 0, 255);
	
	Params::mouseRepulsion.setup(		"Mouse Repulsion", false);
	
	Params::tempVar.setup(				"Temp Var", 0, 0, 1);
	
	Params::bShader.setup(				"B.Shader", false);
	Params::bShaderTime.setup(			"B.Shader Time", 0, 0, 1000);
	Params::bShaderSpeed.setup(			"B.Shader Speed", 0, 0, 0.2);
	Params::bShaderZoom.setup(			"B.Shader Zoom", 0.25, 0, 10);
	Params::bShaderColSpeed.setup(		"B.Shader C.Speed", 0.005, 0, 1);
	
	Params::globalTime.setup(			"Global Time", 0, 0, 1000);
	Params::globalSpeed.setup(			"Global Speed", 1, -1, 1);
	
	Params::linesDistFreq.setup(		"Lines Dist. Freq.", 0, 0, 1000);
	Params::linesDistAmount.setup(		"Lines Dist. Amt.", 0, 0, 100);

	gui.setup();
	gui.add(&Params::globalTime);
	gui.add(&Params::globalSpeed);

	gui.add(&Params::bShader);
	gui.add(&Params::bShaderTime);
	gui.add(&Params::bShaderSpeed);
	gui.add(&Params::bShaderZoom);
	gui.add(&Params::bShaderColSpeed);

	gui.add(&Params::distIntensity);
	gui.add(&Params::distSpeed);
	gui.add(&Params::distTime);
	gui.add(&Params::dispAmount);

	gui.add(&Params::particlesDistAmount);
	gui.add(&Params::particlesDistSpeed);
	gui.add(&Params::particlesDistTime);
	
	gui.add(&Params::boxDistIntensity);
	gui.add(&Params::boxDistSpeed);
	gui.add(&Params::boxDistTime);
	
	gui.add(&Params::linesDistFreq);
	gui.add(&Params::linesDistAmount);

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
	distShader.setUniform1f("distIntensity", Params::boxDistIntensity);

	drawQuote(ofVec2f(250, 600), distShader);

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
	flowFieldShader.setUniform1f("distIntensity", Params::boxDistIntensity);
	flowFieldShader.setUniform1f("flowFieldDistortion", Params::flowFieldDistortion);
	flowFieldShader.setUniform1f("distZScale", Params::zDistScale);
	flowFieldShader.setUniform1f("flowFieldColor", Params::flowFieldColor);

	drawQuote(ofVec2f(250, 600), flowFieldShader);

	flowFieldShader.end();
}

//--------------------------------------------------------------
void testApp::update(){

	Params::globalTime += Params::globalSpeed;
	Params::bShaderTime += Params::bShaderSpeed*Params::globalSpeed;
	Params::distTime += Params::distSpeed*Params::globalSpeed;
	Params::boxDistTime += Params::boxDistSpeed*Params::globalSpeed;
	Params::particlesDistTime += Params::particlesDistSpeed*Params::globalSpeed;
	
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
		textArea.getTextureRef().draw(0, 0);
//		backgroundFbo.draw(0, 0, 200, 130);
//		font0Fbo.draw(200, 0, 200, 130);
//		font1Fbo.draw(400, 0, 200, 130);
//		flowField.draw(600, 0, 200, 130);
//		colorTexture.draw(800, 0, 200, 130);
		
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
	else if (key == ' ') {
		if (Params::globalSpeed == 0) {
			Params::globalSpeed = prevSpeed;
		}
		else {
			prevSpeed = Params::globalSpeed;
			Params::globalSpeed = 0;
		}
	}
	else {
		textArea.keyPressed(key);
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

//void testApp::drawQuote(const ofVec2f& p)
//{
//	float quoteHeight = 0;
//	for (int i=0; i<quote.size(); i++)
//	{
//		quoteHeight += ResourceManager::getInstance().font.stringHeight(quote[i])+80;
//	}
//
//	for (int i=0; i<quote.size(); i++)
//	{
//		float x = p.x;
//		float y = p.y+quoteHeight / quote.size() * i;
//
//		if (i == quote.size()-2) {
//			ResourceManager::getInstance().fontBold.drawString(quote[i], x, y);
//		}
//		else if (i == quote.size()-1) {
//			ResourceManager::getInstance().fontOblique.drawString(quote[i], x, y);
//		}
//		else {
//			ResourceManager::getInstance().font.drawString(quote[i], x, y);
//		}
//	}
//}

void testApp::drawQuote(const ofVec2f &p, ofShader& shader)
{
	float lineJump = 140;
	
	for (int i=0; i<quote.size(); i++)
	{
		shader.setUniform1f("renderID", i);

		float x = p.x;
		float y = p.y + lineJump*i;
		
		if (i == quote.size()-2) {
			shader.setUniformTexture("fontTex", ResourceManager::getInstance().fontBold.getFontTexture(), 1);
			ResourceManager::getInstance().fontBold.drawString(quote[i], x, y);
		}
		else if (i == quote.size()-1) {
			shader.setUniformTexture("fontTex", ResourceManager::getInstance().fontOblique.getFontTexture(), 1);
			ResourceManager::getInstance().fontOblique.drawString(quote[i], x, y);
		}
		else {
			shader.setUniformTexture("fontTex", ResourceManager::getInstance().font.getFontTexture(), 1);
			ResourceManager::getInstance().font.drawString(quote[i], x, y);
		}
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NONE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NONE);
//	distShader.setUniformTexture("fontTex", ResourceManager::getInstance().font.getFontTexture(), 1);
	distShader.setUniformTexture("colorTex", fractalShaderFbo.getTextureReference(), 2);
	distShader.setUniformTexture("textAreaTex", textArea.getTextureRef(), 3);
	distShader.setUniform1f("flowFieldColor", Params::flowFieldColor);
	distShader.setUniform2f("inResolution", 3840, 2160);
	distShader.setUniform2f("time2d", Params::distTime, Params::distTime+10000);
	ofFloatColor col = (ofFloatColor)(ofColor)Params::lineColor;
	distShader.setUniform4f("globalColor", col.r, col.g, col.b, col.a);
	distShader.setUniform1f("boxDistIntensity", Params::boxDistIntensity);
	distShader.setUniform1f("boxDistTime", Params::boxDistTime);
	
	distShader.setUniform1f("distIntensity", Params::distIntensity);
	distShader.setUniform1f("dispAmount", Params::dispAmount);
	distShader.setUniform2f("distPoint", ofGetMouseX()*2, ofGetMouseY()*2);
	
	distShader.setUniform1f("partDistAmount", Params::particlesDistAmount);
	distShader.setUniform1f("partDistTime", Params::particlesDistTime);
	
	distShader.setUniform1f("linesDistFreq", Params::linesDistFreq);
	distShader.setUniform1f("linesDistAmount", Params::linesDistAmount);
	
	ofSetColor(Params::lineColor);
	drawQuote(ofVec2f(250, 600), distShader);
	
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
	drawQuote(ofVec2f(250, 600), normalFontShader);
	
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
