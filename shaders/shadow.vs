#version 410 core

layout (location = 0) in vec3 vertPosition; 
layout (location = 1) in vec3 vertNormal; 
layout (location = 2) in vec2 TextureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 EyePos;


out vec3 worldEyePos;
out vec3 worldPos;

void main() 
{ 

    worldEyePos = EyePos; //(view * vec4(EyePos,1.0)).xyz;

	worldPos = (model * vec4(vertPosition, 1.0)).xyz;
    
	gl_Position = proj * view * model * vec4(vertPosition, 1.0);

}
