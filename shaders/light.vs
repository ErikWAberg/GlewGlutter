#version 330                                                                        

layout (location = 0) in vec3 vertPosition; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec4 LightPosition;
out vec4 worldLightPosition;

void main() {        
    gl_Position = proj * view * model * vec4(vertPosition, 1.0);
  	
    worldLightPosition = LightPosition;
}
