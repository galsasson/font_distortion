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
	ofSetFrameRate(60);
//	ofDisableArbTex();
	
	initGui();
	initControlease();

	quote.push_back("If it should turn out to be true that knowledge (in the modern");
	quote.push_back("sense of know-how) and thought have parted company for good,");
	quote.push_back("then we would indeed become the helpless slaves, not so much");
	quote.push_back("of our machines as of our know-how, thoughtless creatures at");
	quote.push_back("the mercy of every gadget which is technically possible, no matter");
	quote.push_back("how murderous it is.");
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
	combinedFbo.allocate(ofGetWindowWidth()*2, ofGetWindowHeight()*2, GL_RGBA);
	combinedFbo.begin();
	ofClear(0, 0, 0, 255);
	combinedFbo.end();
	
	
	renderColorTexture();
	
	tvShader.load("shaders/tv");
	distShader.load("shaders/dist");
	normalFontShader.load("shaders/font_normal");

	backgroundFbo.allocate(ofGetWindowWidth(), ofGetWindowHeight());
	backgroundShader.load("shaders/background");
	plane.set(ofGetWindowWidth(), ofGetWindowHeight());
	plane.setPosition(ofGetWindowWidth()/2, ofGetWindowHeight()/2, 0);
	plane.setResolution(2, 2);
	
	prevSpeed = 0;
	
	textArea.setup(quote);
	cursorTime = 0;
	
	animation.setup();
	
//	recorder.setup(ofGetWindowWidth(), ofGetWindowHeight());
//	recorder.preAllocate(150);
//	bRecording = false;
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
	
	Params::mouseDistTime.setup(	"Mouse. Dist. Time", 0, 0, 1000);
	Params::mouseDistRange.setup(	"Mouse. Dist. Range", 0, 0, 2000);
	Params::mouseDistSpeed.setup(	"Mouse. Dist. Speed", 0, 0, 1);
	Params::mouseDistAmount.setup(	"Mouse. Dist. Amt.", 0, 0, 2000);
	
	Params::mouseBlowRange.setup(	"Mouse. Blow Range", 0, 0, 2000);
	Params::mouseBlowAmount.setup(	"Mouse. Blow Amt.", 0, 0, 8000);

	Params::shaderColor.setup(		"Shader Color", 0, 0, 1);

	Params::lineColor.setup(			"Line Color", ofColor(50, 50, 50, 255), ofColor(0, 0, 0 ,0), ofColor(255, 255, 255, 255));
	Params::backgroundColor.setup(		"Background Color", ofColor(200, 200, 200, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255));
	Params::tvColorSeparation.setup(	"Color Separation", 0, 0, 50);
	Params::tvLinesIntensity.setup(		"TV Lines Intensity", 0.1, 0, 1);
	Params::tvFlickerIntensity.setup(	"TV Flicker Intensity", 0.03, 0, 1);
	Params::tvVignetteIntensity.setup(	"TV Vignette Intensity", 16, 0, 256);

	Params::bShader.setup(				"B.Shader", false);
	Params::bShaderTime.setup(			"B.Shader Time", 0, 0, 1000);
	Params::bShaderSpeed.setup(			"B.Shader Speed", 0, 0, 0.2);
	Params::bShaderZoom.setup(			"B.Shader Zoom", 0.25, 0, 10);
	Params::bShaderColSpeed.setup(		"B.Shader C.Speed", 0.005, 0, 1);
	
	Params::globalTime.setup(			"Global Time", 0, 0, 1000);
	Params::globalSpeed.setup(			"Global Speed", 1, -1, 1);
	
	Params::linesDistFreq.setup(		"Lines Dist. Freq.", 0, 0, 2000);
	Params::linesDistAmount.setup(		"Lines Dist. Amt.", 0, 0, 100);
	
	Params::quoteLineSpace.setup(		"Quote line space", 160, 100, 200);
	Params::quoteX.setup(				"Quote X", 100, 0, 800);
	Params::quoteY.setup(				"Quote Y", 200, 0, 1000);

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

	gui.add(&Params::mouseDistAmount);
	gui.add(&Params::mouseDistRange);
	gui.add(&Params::mouseDistSpeed);
	gui.add(&Params::mouseDistTime);
	
	gui.add(&Params::mouseBlowAmount);
	gui.add(&Params::mouseBlowRange);
	
	gui.add(&Params::boxDistIntensity);
	gui.add(&Params::boxDistSpeed);
	gui.add(&Params::boxDistTime);
	
	gui.add(&Params::linesDistFreq);
	gui.add(&Params::linesDistAmount);

	gui.add(&Params::shaderColor);

	gui.add(&Params::tvColorSeparation);
	gui.add(&Params::tvVignetteIntensity);
	gui.add(&Params::tvLinesIntensity);
	gui.add(&Params::tvFlickerIntensity);
	gui.add(&Params::lineColor);
	gui.add(&Params::backgroundColor);
	
	gui.add(&Params::quoteLineSpace);
	gui.add(&Params::quoteX);
	gui.add(&Params::quoteY);

	gui.loadFromFile("settings.xml");

	bToggleGui = true;
}

void testApp::initControlease()
{
	control.setup("Font Distortion", 5000);
	for (int i=0; i<gui.getNumControls(); i++)
	{
		ofxBaseGui *c = gui.getControl(i);
//		cout<<"param "<<c->getName()<<" is of type "<<c->getParameter().type()<<endl;
		if (c->getParameter().type() == "11ofParameterIfE") {
			// float
			ofParameter<float> tmp = c->getParameter().cast<float>();
			control.addInput(c->getName(), (float*)&tmp.get());
		}
		else if (c->getParameter().type() == "11ofParameterI8ofColor_IhEE") {
			// color
			ofParameter<ofColor> tmp = c->getParameter().cast<ofColor>();
			control.addInput(c->getName(), (ofColor*)&tmp.get());
		}
	}
}

//--------------------------------------------------------------
void testApp::update()
{
//	animation.update(1.0f / 60);

	Params::globalTime += Params::globalSpeed;
	Params::bShaderTime += Params::bShaderSpeed*Params::globalSpeed;
	Params::distTime += Params::distSpeed*Params::globalSpeed;
	Params::boxDistTime += Params::boxDistSpeed*Params::globalSpeed;
	Params::mouseDistTime += Params::mouseDistSpeed*Params::globalSpeed;
	cursorTime += 1;
	
	fractalShaderFbo.begin();
	renderBackground();
	fractalShaderFbo.end();
	
	// render the background
	backgroundFbo.begin();
	if (Params::bShader) {
		fractalShaderFbo.draw(0, 0);
	}
	else {
		ofSetColor(Params::backgroundColor);
		ofFill();
		ofRect(0, 0, backgroundFbo.getWidth(), backgroundFbo.getHeight());
	}
	backgroundFbo.end();
	
//	font0Fbo.begin();
//	renderNormalQuote();
//	font0Fbo.end();

	font1Fbo.begin();
	renderBoxedQuote();
	font1Fbo.end();
	
//	combinedFbo.begin();
//	ofSetColor(255);
//	backgroundFbo.draw(0, 0);
//	if (bToggleFont0) {
//		font0Fbo.draw(0, 0);
//	}
//	
//	if (bToggleFont1) {
//		font1Fbo.draw(0, 0);
//	}
//	combinedFbo.end();
	
	
}

//--------------------------------------------------------------
void testApp::draw()
{
	tvShaderStart();
	
	ofSetColor(255);
	backgroundFbo.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

	tvShaderEnd();
	
	tvShaderStart();
	
	if (bToggleFont0) {
		font0Fbo.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	}
	
	if (bToggleFont1) {
		font1Fbo.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	}	
	tvShaderEnd();
	
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
			x += ResourceManager::getInstance().fontMedium.stringWidth(ss.str()) + 5;
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
//	else if (key == 'r') {
//		bRecording = !bRecording;
//	}
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

void testApp::drawQuote(const ofVec2f &p, ofShader& shader)
{
	float lineJump = Params::quoteLineSpace;
	
	for (int i=0; i<quote.size(); i++)
	{
		shader.setUniform1f("renderID", i);

		float x = p.x;
		float y = p.y + lineJump*i;
		
		if (i == quote.size()-2) {
//			shader.setUniformTexture("fontTex", ResourceManager::getInstance().fontBold.getFontTexture(), 1);
//			ResourceManager::getInstance().fontBold.drawString(quote[i], x, y);
			shader.setUniformTexture("fontTex", ResourceManager::getInstance().fontMedium.getFontTexture(), 1);
			ResourceManager::getInstance().fontMedium.drawString(quote[i], x, y);
		}
		else if (i == quote.size()-1) {
//			shader.setUniformTexture("fontTex", ResourceManager::getInstance().fontOblique.getFontTexture(), 1);
//			ResourceManager::getInstance().fontOblique.drawString(quote[i], x, y);
			shader.setUniformTexture("fontTex", ResourceManager::getInstance().fontMedium.getFontTexture(), 1);
			ResourceManager::getInstance().fontMedium.drawString(quote[i], x, y);
		}
		else {
//			shader.setUniformTexture("fontTex", ResourceManager::getInstance().font.getFontTexture(), 1);
//			ResourceManager::getInstance().font.drawString(quote[i], x, y);
			shader.setUniformTexture("fontTex", ResourceManager::getInstance().fontMedium.getFontTexture(), 1);
			ResourceManager::getInstance().fontMedium.drawString(quote[i], x, y);
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
//	distShader.setUniformTexture("fontTex", ResourceManager::getInstance().font.getFontTexture(), 1);	// moved to drawQuote
	distShader.setUniformTexture("colorTex", fractalShaderFbo.getTextureReference(), 2);
	distShader.setUniformTexture("textAreaTex", textArea.getTextureRef(), 3);
	distShader.setUniform1f("shaderColor", Params::shaderColor);
	distShader.setUniform2f("inResolution", 3840, 2160);
	distShader.setUniform2f("time2d", Params::distTime, Params::distTime+10000);
	ofFloatColor col = (ofFloatColor)(ofColor)Params::lineColor;
	distShader.setUniform4f("globalColor", col.r, col.g, col.b, col.a);
	distShader.setUniform1f("boxDistIntensity", Params::boxDistIntensity);
	distShader.setUniform1f("boxDistTime", Params::boxDistTime);
	
	distShader.setUniform1f("distIntensity", Params::distIntensity);
	distShader.setUniform1f("dispAmount", Params::dispAmount);
	
	distShader.setUniform2f("mousePoint", ofGetMouseX()*2, ofGetMouseY()*2);
	distShader.setUniform1f("mouseDistAmount", Params::mouseDistAmount);
	distShader.setUniform1f("mouseDistRange", Params::mouseDistRange);
	distShader.setUniform1f("mouseDistTime", Params::mouseDistTime);

	distShader.setUniform1f("mouseBlowAmount", Params::mouseBlowAmount);
	distShader.setUniform1f("mouseBlowRange", Params::mouseBlowRange);

	distShader.setUniform1f("linesDistFreq", Params::linesDistFreq);
	distShader.setUniform1f("linesDistAmount", Params::linesDistAmount);
	
	distShader.setUniform2f("cursorTime", cursorTime, cursorTime);
	distShader.setUniform1f("uncertainty", 1);
	
	ofSetColor(Params::lineColor);
	drawQuote(ofVec2f(Params::quoteX, Params::quoteY), distShader);
	
	distShader.end();
}

void testApp::renderNormalQuote()
{
	ofClear(0, 0, 0, 0);
	normalFontShader.begin();
	ofFloatColor col = (ofFloatColor)(ofColor)Params::lineColor;
	normalFontShader.setUniform4f("globalColor", col.r, col.g, col.b, col.a);
	
	ofSetColor(Params::lineColor);
	drawQuote(ofVec2f(Params::quoteX, Params::quoteY), normalFontShader);
	
	normalFontShader.end();
}

void testApp::tvShaderStart()
{
	tvShader.begin();
	tvShader.setUniform1f("time", Params::globalTime);
	tvShader.setUniform2f("resolution", ofGetWindowWidth()*2, ofGetWindowHeight()*2);
	tvShader.setUniform1f("colorSeparation", Params::tvColorSeparation);
	tvShader.setUniform1f("vignetteIntensity", Params::tvVignetteIntensity);
	tvShader.setUniform1f("linesIntensity", Params::tvLinesIntensity);
	tvShader.setUniform1f("flickerIntensity", Params::tvFlickerIntensity);
}

void testApp::tvShaderEnd()
{
	tvShader.end();
	
	cout<<ofGetFrameRate()<<endl;
}
