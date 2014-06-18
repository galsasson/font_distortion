//
//  ResourceManager.h
//  Controlease
//
//  Created by Gal Sasson on 2/19/14.
//
//

#ifndef __Controlease__ResourceManager__
#define __Controlease__ResourceManager__

#include <iostream>
#include "ofMain.h"

class ResourceManager
{
public:
	static ResourceManager& getInstance()
	{
		static ResourceManager instance;
		// Instantiated on first use.
		return instance;
	}

	ofTrueTypeFont font;

private:
	ResourceManager();

	// make sure there are no copies
	ResourceManager(ResourceManager const&);
	void operator=(ResourceManager const&);

};
#endif /* defined(__tunnelvars__ResourceManager__) */
