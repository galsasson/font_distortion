/* Taken from: https://www.shadertoy.com/view/XsX3W8
 *
 * By: Kali
 */


#version 150

uniform float iGlobalTime;
uniform vec3 iResolution;
//uniform float iRotSpeed;
uniform sampler2DRect tex0;
uniform sampler2DRect iChannel0;
uniform float zoom;
uniform float colspeed;

in vec2 vTexCoord;
in vec4 vColor;
in vec2 fixedCoord;


out vec4 outColor;

//parameters
const int iterations=20;
const float scale=1.3;
const vec2 fold=vec2(.5);
const vec2 translate=vec2(1.5);
//const float zoom=.25;
const float brightness=7.;
const float saturation=.65;
const float texturescale=.15;
const float rotspeed=0.01;
//const float colspeed=.005;
const float antialias=2.;


vec2 rotate(vec2 p, float angle) {
	return vec2(p.x*cos(angle)-p.y*sin(angle),
				p.y*cos(angle)+p.x*sin(angle));
}

void main(void)
{
//	outColor = texture(iChannel0, vTexCoord.xy*iResolution.xy);
//	outColor = vec4(vTexCoord.xy, 0., 1.);
//	return;
	
	float myzoom = zoom;
	
	vec3 aacolor=vec3(0.);
	vec2 pos=vTexCoord.xy-0.5 + vec2(-0.34, -0.21);
	float aspect=iResolution.y/iResolution.x;
	pos.y*=aspect;
	pos/=myzoom;
	vec2 pixsize=max(1./myzoom,100.-iGlobalTime*50.)/iResolution.xy;
	pixsize.y*=aspect;
	for (float aa=0.; aa<25.; aa++) {
		if (aa+1.>antialias*antialias) break;
		vec2 aacoord=floor(vec2(aa/antialias,mod(aa,antialias)));
		vec2 p=pos+aacoord*pixsize/antialias;
		p+=fold;
		float expsmooth=0.;
		vec2 average=vec2(0.);
		float l=length(p);
		for (int i=0; i<iterations; i++) {
			p=abs(p-fold)+fold;
			p=p*scale-translate;
			if (length(p)>20.) break;
			p=rotate(p,iGlobalTime*rotspeed);
			average+=p;
		}
		average/=float(iterations);
		vec2 coord=average+vec2(iGlobalTime*colspeed);
		vec3 color=texture(iChannel0,coord*texturescale*(iResolution.xy/16)).xyz;
		color*=min(1.1,length(average)*brightness);
		color=mix(vec3(length(color)),color,saturation);
		aacolor+=color;
	}
//	outColor = vec4(1., 0., 0., 1.);
	outColor = vec4(aacolor/(antialias*antialias),1.0);
}