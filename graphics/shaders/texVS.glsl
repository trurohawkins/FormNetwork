#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aOffset;
layout (location = 4) in vec4 rotations;
layout (location = 5) in vec2 texOffset;

out vec4 ourColor;
out vec2 TexCoord;

uniform mat4 tMat;
uniform mat4 rMat;
uniform mat4 sMat;
uniform mat3 tcScale;
uniform mat3 tcTrans;
uniform vec2 texCoordShift;

void main()
{
if(aOffset.z == 1) {
mat4 tm = tMat;
tm[3][0] = aOffset.x;
tm[3][1] = aOffset.y;
mat4 rm = rMat;
rm[0][0] = rotations.x;
rm[1][0] = rotations.y;
rm[0][1] = rotations.z;
rm[1][1] = rotations.w;
gl_Position = tm * sMat * rm * vec4(aPos, 1.0);
mat3 tc = tcTrans;
tc[2][0] = texOffset.x;
tc[2][1] = texOffset.y;
vec3 Tex =  tc * tcScale * vec3(aTexCoord, 1.0);
TexCoord = vec2(Tex.x, Tex.y);
ourColor = aColor;
}
}
