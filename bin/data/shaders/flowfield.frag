#version 150

uniform vec4 globalColor;
uniform sampler2D fontTex;
uniform sampler2DRect flowFieldTex;
uniform float flowFieldColor;

in vec2 vTexCoord;
in vec4 vColor;

out vec4 outColor;

void main(void) {
	vec4 texel = texture(fontTex, vTexCoord);

	//    outColor = outPosition;//vec4(outPosition, 1.0, 1.0);
	//    outColor = vec4(vTexCoord, 1.0, 1.0);
	//    outColor = vec4(tt.rgb, 1.0);
	//    outColor = vec4(fixedCoord, 1.0, 1.0);

	// version 0: the font texture
	//    outColor = vec4(globalColor.rgb, texel.a);
	// version 1: the color of the flow field
	//    outColor = vColor;
	// version 3: the color of the flow field
	//    outColor = vec4(vColor.rgb, texel.a);
	// version 4: color of the flowfield blended with global color
	vec3 rgb = (flowFieldColor*vColor.rgb) + ((1-flowFieldColor)*globalColor.rgb);
	outColor = vec4(rgb, texel.a);
}
