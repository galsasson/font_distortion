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

uniform float time;
uniform vec2 resolution;
uniform float colorSeparation;
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
	position.y *=-1.0;
	vec3 color;

	//color separation
	vec2 texPos = position * resolution;
	// on x
	color.r = texture(tex0,vec2(texPos.x+colorSeparation,-texPos.y)).x;
	color.g = texture(tex0,vec2(texPos.x+0.000,-texPos.y)).y;
	color.b = texture(tex0,vec2(texPos.x-colorSeparation,-texPos.y)).z;

	// on y
//	color.r = texture(tex0,vec2(texPos.x,-texPos.y+colorSeparation)).x;
//	color.g = texture(tex0,vec2(texPos.x+0.000,-texPos.y)).y;
//	color.b = texture(tex0,vec2(texPos.x,-texPos.y-colorSeparation)).z;

	//contrast
	color = clamp(color*0.5+0.5*color*color*1.2,0.0,1.0);

	//circular vignette fade
	color *= 0.5 + 0.5*16.0*position.x*position.y*(1.0-position.x)*(-1.0-position.y);

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

	outColor = vec4(color,1.0);
}
