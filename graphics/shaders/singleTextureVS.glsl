#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 tMat;
uniform mat4 rMat;
uniform mat4 sMat;
uniform mat4 Cam;
uniform vec3 camPos;
uniform vec3 camSize;
uniform mat3 tcScale;
uniform mat3 tcTrans;
uniform vec2 texCoordShift;


void main()
{
mat4 tm = tMat;
tm[3][0] = tMat[3][0] + camPos.x;
tm[3][1] = tMat[3][1] + camPos.y;
mat4 sm = sMat;
sm[0][0] = sm[0][0];
sm[1][1] = sm[1][1];
vec4 pos = (Cam * tm * sm * rMat * (vec4(aPos, 1.0)));
gl_Position = pos;
ourColor = aColor;
vec3 Tex =  tcTrans * tcScale * vec3(aTexCoord, 1.0);
TexCoord = vec2(Tex.x, Tex.y);
}
