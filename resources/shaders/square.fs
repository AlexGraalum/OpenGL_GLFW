#version 330 compatibility

in vec2 vST;

uniform sampler2D tex;

void main(){
     gl_FragColor = texture(tex, vST);
}