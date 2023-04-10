#version 330 compatibility
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 vST;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
     vST = aTexCoord;

     gl_Position = (projection * view * model) * gl_Vertex;
}