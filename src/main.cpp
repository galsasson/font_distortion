#include "ofMain.h"
#include "testApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);	// this kicks off the
//	ofSetupOpenGL(1280,720,OF_WINDOW);			// <-------- setup the GL context
	
	ofAppGLFWWindow win;
	win.setNumSamples(8);
	
	ofSetupOpenGL(&win, 1920, 1080, OF_FULLSCREEN);
//	ofSetupOpenGL(1920, 1080, OF_WINDOW);

	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());

}
