#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform vec4 colorShift;
uniform sampler2D ourTexture;
uniform vec2 TexCoordShift;

void main()
{
vec4 texColor = texture(ourTexture, TexCoord);
FragColor = vec4(texColor.x * colorShift.x, texColor.y * colorShift.y, texColor.z * colorShift.z, texColor.w * colorShift.w);
}
