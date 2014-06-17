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
    
    quote.push_back("If it should turn out to be true that knowledge (in the modern sense of know-how)\n");
    quote.push_back("and thought have parted company for good, then we would indeed become\n");
    quote.push_back("the helpless slaves, not so much of our machines as of our know-how,\n");
    quote.push_back("thoughtless creatures at the mercy of every gadget which is technically possible,\n");
    quote.push_back("no matter how murderous it is.");
    
    virgin.allocate(ofGetWindowWidth(), ofGetWindowHeight(), GL_RGBA);
    virgin.begin();
    ofClear(0, 0, 0, 255);
    virgin.end();
    second.allocate(ofGetWindowWidth()*2, ofGetWindowHeight()*2, GL_RGBA);
    second.begin();
    ofClear(0, 0, 0, 255);
    second.end();
    tvShader.load("shaders/tv");
    distShader.load("shaders/dist");
    
    flowField.setup(ofGetWindowWidth(), ofGetWindowHeight());
    flowField.randomize();
//    setupString("neutral.");
}

void testApp::initGui()
{
    Params::distIntensity.setup("Distortion", 0, 0, 100);
    Params::distSpeed.setup("Distortion Speed", 0, 0, 0.2);
    
    Params::lineColor.setup("Line Color", ofColor(50), ofColor(0), ofColor(255));
    Params::backgroundColor.setup("Background Color", ofColor(200), ofColor(0), ofColor(255));
    Params::tvColorSeparation.setup("Color Separation", 0, 0, 50);
    Params::tvLinesIntensity.setup("TV Lines Intensity", 0.1, 0, 1);
    Params::tvFlickerIntensity.setup("TV Flicker Intensity", 0.03, 0, 1);
    
    Params::tempVar.setup("Temp Var", 0, 0, 1);
    
    gui.setup();
    gui.add(&Params::distIntensity);
    gui.add(&Params::distSpeed);
    gui.add(&Params::tvColorSeparation);
    gui.add(&Params::tvLinesIntensity);
    gui.add(&Params::tvFlickerIntensity);
    gui.add(&Params::lineColor);
    gui.add(&Params::backgroundColor);
    gui.add(&Params::tempVar);
    
    gui.loadFromFile("settings.xml");
    
    time = 0;
}

//--------------------------------------------------------------
void testApp::update(){
    
    time += Params::distSpeed;

    second.begin();
    ofSetColor(Params::backgroundColor);
    ofFill();
    ofRect(0, 0, second.getWidth(), second.getHeight());
    
    distShader.begin();
    distShader.setUniformTexture("fontTex", ResourceManager::getInstance().font.getFontTexture(), 1);
    distShader.setUniform2f("time2d", time, time+10000);
    ofFloatColor col = (ofFloatColor)(ofColor)Params::lineColor;
    distShader.setUniform4f("globalColor", col.r, col.g, col.b, col.a);
    distShader.setUniform1f("distIntensity", Params::distIntensity);

    drawQuote();

    distShader.end();
    second.end();
}

//--------------------------------------------------------------
void testApp::draw(){
    
    tvShader.begin();
    tvShader.setUniform1f("time", (float)ofGetFrameNum()/60);
    tvShader.setUniform2f("resolution", ofGetWindowWidth()*2, ofGetWindowHeight()*2);
    tvShader.setUniform1f("colorSeparation", Params::tvColorSeparation);
    tvShader.setUniform1f("linesIntensity", Params::tvLinesIntensity);
    tvShader.setUniform1f("flickerIntensity", Params::tvFlickerIntensity);

    ofSetColor(255);
    second.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    
    tvShader.end();
    
    virgin.draw(0, 0, 200, 130);
    second.draw(200, 0, 200, 130);
    flowField.draw(400, 0, 200, 130);
//    ResourceManager::getInstance().font.getFontTexture().draw(400, 0, 512, 512);
#if 0
    ofSetColor(Params::lineColor);
    ofSetLineWidth(Params::lineWidth);
    for (int i=0; i<chars.size(); i++)
    {
        chars[i]->draw();
    }
#endif
//    ResourceManager::getInstance().font.drawString("Everywhere we remain unfree and chained to technology", 0, 200);
    
//    virgin.draw(0, 0);
    
    gui.draw();
    
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

void testApp::drawQuote()
{
    ofVec2f resolution(1280*2, 720*2);
    float quoteHeight = 0;
    for (int i=0; i<quote.size(); i++)
    {
        quoteHeight += ResourceManager::getInstance().font.stringHeight(quote[i]);
    }
    
    float y = (resolution.y - quoteHeight)/2;
    for (int i=0; i<quote.size(); i++)
    {
        float x = (resolution.x - ResourceManager::getInstance().font.stringWidth(quote[i]))/2;
        ResourceManager::getInstance().font.getStringMesh(quote[i], x, y).draw();
        y+= ResourceManager::getInstance().font.stringHeight(quote[i]);
    }
    
}
