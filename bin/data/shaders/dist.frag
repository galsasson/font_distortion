#version 150

uniform vec4 globalColor;
uniform sampler2D fontTex;
uniform sampler2DRect colorTex;
uniform float flowFieldColor;
uniform vec2 inResolution;
uniform float linesDistFreq;
uniform float linesDistAmount;
uniform float partDistTime;
uniform vec2 cursorTime;
uniform float uncertainty;

in vec2 vTexCoord;
in vec2 fixedCoord;
in vec4 vColor;
in float focalAmount;
in float letterLineDistAmount;
in float isCursor;
in float greenDist;

out vec4 outColor;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(void) {
	
	// add line distortion to vTexCoord
	
	// normalize fixedCoord.y
	float normFCoordY = fixedCoord.y / inResolution.y;
	// set frequency
	float val = sin(partDistTime*300 + normFCoordY*linesDistFreq);
	vec2 lineSeed = vec2(normFCoordY, 0.);
	val = (val>0)?linesDistAmount*rand(lineSeed+cursorTime):-linesDistAmount*rand(lineSeed+cursorTime);
	val *= letterLineDistAmount;
//	val = clamp(val, -5, 5);
	
	vec2 distTexCoords = vec2(val/1000, 0.);
	
	vec4 texel = texture(fontTex, vTexCoord + distTexCoords);

	
	vec3 addNoise = vec3(0, 0, 0);;
	
	if (isCursor > 0 || letterLineDistAmount>0) {
//		vec2 texCoordBig = vec2(step(sin(vTexCoord.x*1000), 0), step(sin(vTexCoord.y*1000), 0));
		addNoise = vec3(rand(vTexCoord+cursorTime), rand(vTexCoord+cursorTime+vec2(0, 2)), rand(vTexCoord+cursorTime+vec2(0, 4)));
//		addNoise = vec3(rand(texCoordBig+cursorTime), rand(texCoordBig+cursorTime+vec2(0, 1)), rand(texCoordBig+cursorTime+vec2(0, 2)));
	}

	// merge texture color 'colorTex' and 'globalColor' according to 'flowFieldColor' parameter
	vec3 rgb = (flowFieldColor*vColor.rgb) + ((1-flowFieldColor)*globalColor.rgb) + addNoise;
	
//	if (greenDist > 0.) {
//		rgb.r -= rand(vTexCoord)/2.;
//		rgb.b -= rand(vTexCoord);
//	}
	
	outColor = vec4(rgb, texel.a);

	// fill all the box
//	outColor = vec4(rgb, 1.0);
}
