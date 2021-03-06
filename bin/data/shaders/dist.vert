#version 150

// these are for the programmable pipeline system and are passed in
// by default from OpenFrameworks
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec4 color;
in vec4 normal;
in vec2 texcoord;
// this is the end of the default functionality

// this is something we're creating for this shader
uniform sampler2DRect colorTex;

out vec2 vTexCoord;
out vec2 fixedCoord;
out vec4 vColor;
out float bPaint;

// this is coming from our C++ code
uniform vec2 inResolution;
uniform vec2 time2d;
uniform float boxDistTime;
uniform float boxDistIntensity;
uniform float distIntensity;
uniform float dispAmount;
uniform vec2 distPoint;
uniform float partDistAmount;
uniform float partDistTime;
uniform float renderID;

/**************************************************************/
//
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
//

vec3 mod289(vec3 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
	return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
{
	const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
						0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
						-0.577350269189626,  // -1.0 + 2.0 * C.x
						0.024390243902439); // 1.0 / 41.0
	// First corner
	vec2 i  = floor(v + dot(v, C.yy) );
	vec2 x0 = v -   i + dot(i, C.xx);

	// Other corners
	vec2 i1;
	//i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
	//i1.y = 1.0 - i1.x;
	i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
	// x0 = x0 - 0.0 + 0.0 * C.xx ;
	// x1 = x0 - i1 + 1.0 * C.xx ;
	// x2 = x0 - 1.0 + 2.0 * C.xx ;
	vec4 x12 = x0.xyxy + C.xxzz;
	x12.xy -= i1;

	// Permutations
	i = mod289(i); // Avoid truncation effects in permutation
	vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
					 + i.x + vec3(0.0, i1.x, 1.0 ));

	vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
	m = m*m ;
	m = m*m ;

	// Gradients: 41 points uniformly over a line, mapped onto a diamond.
	// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

	vec3 x = 2.0 * fract(p * C.www) - 1.0;
	vec3 h = abs(x) - 0.5;
	vec3 ox = floor(x + 0.5);
	vec3 a0 = x - ox;

	// Normalise gradients implicitly by scaling m
	// Approximation of: m *= inversesqrt( a0*a0 + h*h );
	m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

	// Compute final noise value at P
	vec3 g;
	g.x  = a0.x  * x0.x  + h.x  * x0.y;
	g.yz = a0.yz * x12.xz + h.yz * x12.yw;
	return 130.0 * dot(m, g);
}
/**************************************************************/

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


void main()
{
	// here we move the texture coordinates
	vTexCoord = vec2(texcoord.x, texcoord.y);
	fixedCoord = vec2(position.x, position.y);
	vColor = texture(colorTex, position.xy/2.0);
	
	int letterIndex = gl_VertexID/4;
	int vertIndex = gl_VertexID%4;

//	int phase = int(floor(boxDistTime));

	// change rect porportions

	// symetrical version (cartoonish)
	float noise1 = (snoise(vec2(renderID*100 + letterIndex*100.+500., boxDistTime)))*boxDistIntensity;
	float noise2 = (snoise(vec2(renderID*100 + letterIndex*100.+250., boxDistTime)))*boxDistIntensity;
	vec4 tlOffset = vec4(-noise1, -noise2, 0., 0.);
	vec4 brOffset = vec4(noise1, noise2, 0., 0.);
	
	// free version
//	float noise1 = (snoise(vec2(letterIndex*100.+500., phase))-0.5)*boxDistIntensity;
//	float noise2 = (snoise(vec2(letterIndex*100.+250., phase))-0.5)*boxDistIntensity;
//	float noise3 = (snoise(vec2(letterIndex*100.+750., phase))-0.5)*boxDistIntensity;
//	float noise4 = (snoise(vec2(letterIndex*100.+1000., phase))-0.5)*boxDistIntensity;
//	vec4 tlOffset = vec4(noise1, noise2, 0., 0.);
//	vec4 brOffset = vec4(noise3, noise4, 0., 0.);
	
	// add noise to each vertex
	vec4 vertNoise = vec4(0., 0., 0., 0.);
	if (length(distPoint - position.xy) < 200) {
		vertNoise = vec4((snoise(vec2(partDistTime, 0.) + position.xy))*partDistAmount,
						  (snoise(vec2(partDistTime, 10.) + position.xy))*partDistAmount,
							0.0, 0.0);
	}
	
	// all distortion
	if (vertIndex>1) {
		vertNoise += vec4((snoise(vec2(time2d.x, 0.) + position.xy))*distIntensity,
						 (snoise(vec2(0, time2d.y) + position.xy))*distIntensity,
						 0.0, 0.0);
		
	}
	
	// per letter
	vec4 movement = vec4(0);
	if (true)//(letterIndex+phase)%4==0)//(phase)%3==0)
	{
		float tx = snoise(vec2(letterIndex*10., time2d.x));
		float ty = snoise(vec2(letterIndex*10.+1000, time2d.x));

		// add displacement
		movement = vec4(tx*dispAmount, ty*dispAmount, 0., 0.);
		
		if (vertIndex==0) {
			movement += tlOffset;
		}
		else if (vertIndex==1) {
			movement.x += brOffset.x;
			movement.y += tlOffset.y;
		}
		else if (vertIndex==2) {
			movement += brOffset;
		}
		else {
			movement.x += tlOffset.x;
			movement.y += brOffset.y;
		}
		bPaint = 1;
	}
	else {
		bPaint = 0;
	}

	// send the vertices to the fragment shader
	gl_Position = modelViewProjectionMatrix * position + movement + vertNoise;
}
