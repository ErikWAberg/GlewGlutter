#version 410 core

in vec3 worldPos;
in vec3 worldEyePos;

out float ShadowColor;

void main() 
{ 
    ShadowColor = length(worldPos - worldEyePos);
    //ShadowColor = gl_FragCoord.z;
}