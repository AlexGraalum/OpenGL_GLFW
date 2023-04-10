#version 330 compatibility
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 vST;

uniform mat4 model;

void main(){
     vST = aTexCoord;

     gl_Position = model * gl_Vertex;
}