#version 150

uniform vec4 globalColor;
uniform sampler2D fontTex;

in vec2 vTexCoord;
in vec2 fixedCoord;
in vec4 vColor;

out vec4 outColor;

void main(void) {
    vec4 texel = texture(fontTex, vTexCoord);
    outColor = vec4(globalColor.rgb, texel.a);
}
