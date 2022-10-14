#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 tMat;
uniform mat4 rMat;
uniform mat4 sMat;
unfirom mat4 Cam;
uniform mat3 tcScale;
uniform mat3 tcTrans;
uniform vec2 texCoordShift;


void main()
{
gl_Position = Cam *tMat * sMat * rMat * vec4(aPos, 1.0);
ourColor = aColor;
vec3 Tex =  tcTrans * tcScale * vec3(aTexCoord, 1.0);
TexCoord = vec2(Tex.x, Tex.y);
}
