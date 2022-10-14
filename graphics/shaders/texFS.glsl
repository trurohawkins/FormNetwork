#version 330 core
out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;

uniform vec4 colorShift;
uniform sampler2D ourTexture;
uniform vec2 TexCoordShift;

void main()
{
vec4 texColor = texture(ourTexture, TexCoord);
FragColor = vec4(texColor.x * colorShift.x * ourColor.x, texColor.y * colorShift.y * ourColor.y, texColor.z * colorShift.z * ourColor.z, texColor.w * colorShift.w * ourColor.w);
}
