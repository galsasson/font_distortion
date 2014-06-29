#version 150

uniform vec4 globalColor;
uniform sampler2D fontTex;
uniform sampler2DRect colorTex;
uniform float flowFieldColor;
uniform vec2 inResolution;
uniform float linesDistFreq;
uniform float linesDistAmount;
uniform float partDistTime;

in vec2 vTexCoord;
in vec2 fixedCoord;
in vec4 vColor;
in float bPaint;

out vec4 outColor;

void main(void) {
	// add line distortion to vTexCoord
	
	// normalize fixedCoord.y
	float normFCoordY = fixedCoord.y / inResolution.y;
	// set frequency
	float val = sin(partDistTime*300 + normFCoordY*linesDistFreq)*linesDistAmount;
	val = (val>0)?linesDistAmount:-linesDistAmount;
//	val = clamp(val, -5, 5);
	
	vec2 distTexCoords = vec2(val/1000, 0.);
	
//	outColor = vec4(0., fixedCoord.y/inResolution.y, 0., 1.);
//	return;
//	vec2 texCoords =
	
	vec4 texel = texture(fontTex, vTexCoord + distTexCoords);
//	outColor = vec4(vColor.rgb, texel.a);
	if (bPaint == 1)
	{
		// merge texture color 'colorTex' and 'globalColor' according to 'flowFieldColor' parameter
		vec3 rgb = (flowFieldColor*vColor.rgb) + ((1-flowFieldColor)*globalColor.rgb);
		
		
		
		outColor = vec4(rgb, texel.a);

		// fill all the box
//		outColor = vec4(rgb, 1.0);
	}
	else {
		outColor = vec4(0., 0., 0., 0.);
	}
}
