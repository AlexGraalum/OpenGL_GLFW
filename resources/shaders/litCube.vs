#version 330 compatibility
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 fragPos;
out vec3 normal;
out vec2 vST;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){
     vST = aTexCoord;

     fragPos = vec3(model * vec4(aPos, 1.0f));
     normal = mat3(transpose(inverse(model))) * aNormal;

     gl_Position = projection * view * vec4(fragPos, 1.0f);
}