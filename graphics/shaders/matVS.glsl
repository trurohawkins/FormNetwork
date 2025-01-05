#version 330
uniform mat4 tMat;
uniform mat4 sMat;
uniform mat4 rMat;
uniform mat4 Cam;

in vec2 vp;
void main() {	
gl_Position = Cam * (tMat * rMat * sMat * vec4(vp.x, vp.y, 0.0, 1.0));
}
