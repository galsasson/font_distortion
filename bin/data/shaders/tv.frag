/**
 **   __ __|_  ___________________________________________________________________________  ___|__ __
 **  //    /\                                           _                                  /\    \\
 ** //____/  \__     __ _____ _____ _____ _____ _____  | |     __ _____ _____ __        __/  \____\\
 **  \    \  / /  __|  |     |   __|  _  |     |  _  | | |  __|  |     |   __|  |      /\ \  /    /
 **   \____\/_/  |  |  |  |  |  |  |     | | | |   __| | | |  |  |  |  |  |  |  |__   "  \_\/____/
 **  /\    \     |_____|_____|_____|__|__|_|_|_|__|    | | |_____|_____|_____|_____|  _  /    /\
 ** /  \____\                       http://jogamp.org  |_|                              /____/  \
 ** \  /   "' _________________________________________________________________________ `"   \  /
 **  \/____.                                                                             .____\/
 **
 ** Postprocessing filter implementing a couple of "analog distortion" effects: color separation,
 ** contrast, vigneting,  color shift, tv-lines and tv-flicker. Mainly inspired by IQ's "Shader Toy"
 ** postprocessing filter wich can be found here: http://www.iquilezles.org/apps/shadertoy/
 **
 **/

#version 150

#extension GL_ARB_gpu_shader_fp64 : enable
#pragma optionNV(fastmath off)
#pragma optionNV(fastprecision off)


uniform float time;
uniform vec2 resolution;
uniform float colorSeparation;
uniform float vignetteIntensity;
uniform float linesIntensity;
uniform float flickerIntensity;
//uniform sampler2D sampler0;
uniform sampler2DRect tex0;
//uniform sampler2DRect ttt;

in vec2 vTexCoord;
in vec2 fixedCoord;
in vec4 vColor;


out vec4 outColor;

void main(void) {

	vec2 position = vTexCoord / resolution.xy;
	vec3 color;

	//color separation
	vec2 texPos = position * resolution;
	vec2 texPosRight = vec2(texPos.x+colorSeparation,texPos.y);
	vec2 texPosMiddle = texPos;
	vec2 texPosLeft = vec2(texPos.x-colorSeparation,texPos.y);
	
	// on x
	vec4 colorRight = texture(tex0,texPosRight);
	vec4 colorMiddle = texture(tex0,texPosMiddle);
	vec4 colorLeft = texture(tex0,texPosLeft);
	
	color.r = colorLeft.r;
	color.g = colorMiddle.g;
	color.b = colorRight.b;

	float alpha = clamp(colorLeft.a + colorMiddle.a + colorRight.a, 0.0, 1.0);
	if (colorMiddle.a == 1.0) {
		color = colorMiddle.rgb;
		alpha = 1.0;
	}

	//contrast
	color = clamp(color*0.5+0.5*color*color*1.2,0.0,1.0);

	//circular vignette fade
	color *= 0.5 + 0.5*vignetteIntensity*position.x*2*position.y*2*(1.0-position.x*2)*(1.0-position.y*2);

	//color shift
	//color *= vec3(0.8,1.0,0.7); //green
//	color *= vec3(0.95,0.85,1.0); //blue
	//color *= vec3(1.0,0.8,0.1); //red
	//color *= vec3(1.0,0.7,1.0); //purple
	//color *= vec3(0.7,1.0,1.0); //cyan
	//color *= vec3(1.0,1.0,0.7); //yellow
	//    float gray = dot(color, vec3(0.299, 0.587, 0.114));
	//    color = vec3(gray, gray, gray); //gray

	//tvlines effect
	color *= (1.0-linesIntensity)+linesIntensity*sin(10.0*time+position.y*1000.0);

	//tv flicker effect
	color *= (1.0-flickerIntensity) + flickerIntensity*sin(110.0*time);

	outColor = vec4(color,alpha);
}
