#version 330 

layout (location = 0) in vec3 vPos; 
layout (location = 1) in vec3 vertNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform int envMapping;

flat out int EnvMapping;
smooth out vec3 eyeDirection;
out vec3 pos_eye;
out vec3 n_eye;


void main() 
{ 
	mat4 inverseProj = inverse(proj);
    mat3 inverseModelview = transpose(mat3(view ));
    
    vec3 unprojected = (inverseProj * vec4(vPos,1.0)).xyz;
    eyeDirection = inverseModelview * unprojected;
	

    EnvMapping = envMapping;
    if(envMapping == 1) {
        pos_eye = vec3 (view * model * vec4 (vPos, 1.0));
        n_eye = vec3 (view * model * vec4 (vertNormal, 0.0));
        gl_Position =  proj * view * model * vec4(vPos,1.0);
    } else {
        gl_Position = vec4(vPos,1.0);
        gl_Position.z = gl_Position.w -0.00001; //fix to far plane.
    }
    

}

