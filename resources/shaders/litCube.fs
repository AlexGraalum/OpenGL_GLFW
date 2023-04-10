#version 330 compatibility

in vec3 fragPos;
in vec3 normal;
in vec2 vST;

uniform sampler2D tex;

uniform vec3 lightPos;
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

vec3 objectColor = vec3(1.0f, 0.0f, 0.0f);

uniform vec3 viewPos;

void main(){
     //Ambient
     float ambientStrength = 0.1f;
     vec3 ambient = ambientStrength * lightColor;

     //Diffuse
     vec3 norm = normalize(normal);
     vec3 lightDir = normalize(lightPos - fragPos);
     float diff = max(dot(normal, lightDir), 0.0f);
     vec3 diffuse = diff * lightColor;
     
     //Specular
     float specularStrength = 0.5f;
     vec3 viewDir = normalize(viewPos - fragPos);
     vec3 reflectDir = reflect(-lightDir, norm);
     float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
     vec3 specular = specularStrength * spec * lightColor;

     vec3 result = (ambient + diffuse + specular) * objectColor;
     gl_FragColor = mix(texture(tex, vST),vec4(result, 1.0f), 0.5f);
     //gl_FragColor = vec4(result, 1.0f);
}