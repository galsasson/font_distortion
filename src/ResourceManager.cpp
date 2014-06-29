//
//  ResourceManager.cpp
//  Controlease
//
//  Created by Gal Sasson on 2/19/14.
//
//

#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
//	font.loadFont("futura.ttf", 60, true, true, true);
//	ofTrueTypeFont::setGlobalDpi(320);
	font.loadFont("fonts/Helvetica.ttf", 70, true, true, true);
	fontBold.loadFont("fonts/HelveticaBold.ttf", 70, true, true, true);
	fontOblique.loadFont("fonts/HelveticaOblique.ttf", 70, true, true, true);
}
