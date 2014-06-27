#version 150

uniform vec4 globalColor;
uniform sampler2D fontTex;
uniform sampler2DRect colorTex;
uniform float flowFieldColor;

in vec2 vTexCoord;
in vec2 fixedCoord;
in vec4 vColor;
in float bPaint;

out vec4 outColor;

void main(void) {
	vec4 texel = texture(fontTex, vTexCoord);
//	outColor = vec4(vColor.rgb, texel.a);
	if (bPaint == 1)
	{
		outColor = globalColor;
//		outColor = vec4(globalColor.rgb, texel.a);
		
//		vec3 rgb = (flowFieldColor*vColor.rgb) + ((1-flowFieldColor)*globalColor.rgb);
//		outColor = vec4(rgb, texel.a);

	}
	else {
		outColor = vec4(0., 0., 0., 0.);
	}
}
