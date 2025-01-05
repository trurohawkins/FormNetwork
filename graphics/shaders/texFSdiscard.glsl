#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec2 TexCoordShift;

void main()
{
vec4 texColor = texture(ourTexture, TexCoord);
if(texColor.a < 0.1)
discard;
FragColor = texColor;
}
